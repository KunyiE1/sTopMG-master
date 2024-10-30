//
// Created by kunyili on 9/20/24.
//

#include "scaffold_processor.hpp"

#include <iostream>
#include <unordered_map>
#include "common/util/file_util.hpp"
#include "common/base/mod_util.hpp"
#include "common/thread/simple_thread_pool.hpp"
#include "seq/db_block.hpp"
#include "seq/proteoform_factory.hpp"
#include "ms/spec/msalign_util.hpp"
#include "ms/factory/prm_ms_factory.hpp"
#include "ms/factory/spectrum_set_factory.hpp"
#include "prsm/simple_prsm_xml_writer.hpp"
#include "prsm/simple_prsm_xml_writer_util.hpp"
#include "prsm/simple_prsm_str_merge.hpp"
#include "seq/proteoform.hpp"

#include "scaffold/scaffold.hpp"
#include "scaffold/scaffold_processor.hpp"
#include <fstream>
#include "ms/factory/prm_ms_util.hpp"

#include <time.h>

namespace toppic {


    inline void scaffoldBlock(ProteoformPtrVec & raw_forms,
                            int block_idx,
                            ScaffoldMngPtr mng_ptr,
                            const std::vector<double> & mod_mass_list) {
        std::string block_str = str_util::toString(block_idx);
        //ScaffoldPtr filter_ptr = std::make_shared<Scaffold>(raw_forms, mng_ptr, block_str);


        PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
        SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
        std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
        int group_spec_num = mng_ptr->prsm_para_ptr_->getGroupSpecNum();

        std::string db_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder();
        FastaIndexReaderPtr fasta_reader_ptr = std::make_shared<FastaIndexReader>(db_file_name);

        SimpleMsAlignReaderPtr reader_ptr = std::make_shared<SimpleMsAlignReader>(sp_file_name,
                                                                                  group_spec_num,
                                                                                  sp_para_ptr->getActivationPtr());


        std::string input_file_name
                = file_util::basename(sp_file_name) + "." + mng_ptr->input_file_ext_;


        DeconvMsPtrVec deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();
        std::vector<double> prec_error_vec = sp_para_ptr->getMultiShiftSearchPrecErrorVec();


        while (deconv_ms_ptr_vec.size() != 0) {
            // allow one dalton error
            SpectrumSetPtrVec spec_set_vec
                    = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(deconv_ms_ptr_vec,
                                                                               sp_para_ptr, prec_error_vec);

            for (size_t k = 0; k < spec_set_vec.size(); k++) {
                SpectrumSetPtr spec_set_ptr = spec_set_vec[k];
                //std::cout<<"PrecMonoMass:"<<spec_set_ptr->getPrecMonoMass()<<std::endl;
                if (spec_set_ptr->isValid()) {
                    DeconvMsPtrVec deconv_ms_ptr_vec = spec_set_ptr->getDeconvMsPtrVec();
                    double prec_mono_mass = spec_set_ptr->getPrecMonoMass();
                    std::vector<double> mod_mass(0);
                    int spec_id = spec_set_ptr->getSpectrumId();
                    std::vector<SimplePrsmStrPtr> selected_prsm_ptrs;
                    PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
                    mng_ptr->sp_id = spec_set_ptr->getSpectrumId();
                    PrmPeakPtrVec peak_vec = prm_ms_util::getPrmPeakPtrs(ms_ptr_vec,
                                                                         sp_para_ptr->getPeakTolerancePtr());


                }
            }
        }
    }
    static std::function<void()> geneTask(int block_idx,
                                          const std::vector<double> &mod_mass_list,
                                          ScaffoldMngPtr mng_ptr) {
        return[block_idx, mod_mass_list, mng_ptr] () {
            PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
            std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
            std::string db_block_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder()
                                             + "_" + str_util::toString(block_idx);

//    std::cout<<"=====FIX MOD: "<<prsm_para_ptr->getFixModPtrVec()[0]->getShift()<<std::endl;

            ProteoformPtrVec raw_forms
                    = proteoform_factory::readFastaToProteoformPtrVec(db_block_file_name,
                                                                      prsm_para_ptr->getFixModPtrVec(),
                                                                      false);
            scaffoldBlock(raw_forms, block_idx, mng_ptr, mod_mass_list);
        };
    }

    void ScaffoldProcessor::process() {
        std::string db_file_name = mng_ptr_->prsm_para_ptr_->getSearchDbFileNameWithFolder();
        DbBlockPtrVec db_block_ptr_vec = DbBlock::readDbBlockIndex(db_file_name);

        std::vector<double> mod_mass_list;
        if (mng_ptr_->residue_mod_file_name_ != "") {
            mod_mass_list
                    = mod_util::getModMassVec(mod_util::readModTxt(mng_ptr_->residue_mod_file_name_)[2]);
        }

        int spec_num = msalign_util::getSpNum(mng_ptr_->prsm_para_ptr_->getSpectrumFileName());
        mng_ptr_->n_spec_block_ = spec_num * db_block_ptr_vec.size();

        SimpleThreadPoolPtr pool_ptr = std::make_shared<SimpleThreadPool>(mng_ptr_->thread_num_);
        int block_num = db_block_ptr_vec.size();
        mng_ptr_->cnts_.resize(block_num, 0);

        for (size_t i = 0; i < db_block_ptr_vec.size(); i++) {
            while (pool_ptr->getQueueSize() > 0 || pool_ptr->getIdleThreadNum() == 0) {
                boost::this_thread::sleep(boost::posix_time::milliseconds(10));
            }
            pool_ptr->Enqueue(geneTask(db_block_ptr_vec[i]->getBlockIdx(), mod_mass_list, mng_ptr_));
        }
        pool_ptr->ShutDown();
        std::cout << std::endl;

        std::cout << "Multiple PTM filtering - combining blocks started." << std::endl;
        std::string sp_file_name = mng_ptr_->prsm_para_ptr_->getSpectrumFileName();
        SimplePrsmStrMergePtr merge_ptr
                = std::make_shared<SimplePrsmStrMerge>(sp_file_name, mng_ptr_->output_file_ext_,
                                                       block_num, mng_ptr_->output_file_ext_,
                                                       mng_ptr_->filter_result_num_);
        merge_ptr->process();
        merge_ptr = nullptr;
        //Remove temporary files
        file_util::cleanTempFiles(sp_file_name, mng_ptr_->output_file_ext_ + "_");
        std::cout << "Multiple PTM filtering - combining blocks finished." << std::endl;
    }

} // toppic