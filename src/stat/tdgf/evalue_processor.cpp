//Copyright (c) 2014 - 2020, The Trustees of Indiana University.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.



#include <climits>
#include <iomanip>
#include "common/util/logger.hpp"
#include "common/util/file_util.hpp"
#include "common/thread/simple_thread_pool.hpp"
#include "ms/spec/msalign_util.hpp"
#include "ms/factory/spectrum_set_factory.hpp"
#include "prsm/prsm.hpp"
#include "prsm/prsm_reader.hpp"
#include "prsm/prsm_str_merge.hpp"
#include "prsm/prsm_xml_writer.hpp"
#include "prsm/prsm_xml_writer_util.hpp"
#include "stat/tdgf/evalue_processor.hpp"

#include "prsm/evalue_prsm.hpp"

namespace toppic {

void EValueProcessor::init() {
  test_num_ptr_ = std::make_shared<CountTestNum>(mng_ptr_->convert_ratio_,
                                                 mng_ptr_->max_ptm_mass_,
                                                 mng_ptr_->max_prec_mass_,
                                                 mng_ptr_->prsm_para_ptr_);

  ResFreqPtrVec residue_freqs = test_num_ptr_->getResFreqPtrVec();
  if (!mng_ptr_->use_gf_) {
    comp_pvalue_table_ptr_ = std::make_shared<CompPValueLookupTable>(mng_ptr_);
  }

  comp_pvalue_ptr_ = std::make_shared<CompPValueArray>(test_num_ptr_, mng_ptr_);

}


    bool E_value_compare(Evalue_PrsmPtr a, Evalue_PrsmPtr b) {
        return a->getEValue() > b->getEValue();
    }

    std::function<void()> geneTask_1(SpectrumSetPtr spec_set_ptr,
                                   const Evalue_PrsmPtr & e_prsm,
                                   double ppo, bool is_separate,
                                   TdgfMngPtr mng_ptr, CountTestNumPtr test_num_ptr,
                                   SimpleThreadPoolPtr pool_ptr){
        return [spec_set_ptr, e_prsm, ppo, is_separate, mng_ptr, test_num_ptr, pool_ptr](){
            std::vector<Evalue_PrsmPtr> e_prsm_vec; // copy sele_prsm_ptrs
            e_prsm_vec.push_back(e_prsm);
            CompPValueArrayPtr comp_pvalue_ptr = std::make_shared<CompPValueArray>(test_num_ptr, mng_ptr);
            comp_pvalue_ptr->process_stopmg(spec_set_ptr, e_prsm_vec, ppo, is_separate);

//            std::cout<<std::endl;
//            std::cout<<"E-value"<< std::scientific << std::setprecision(3) << e_prsm_vec[0]->getEValue()<<std::endl;
            for (unsigned i = 0; i < e_prsm_vec.size(); i++) {
                if (std::round(e_prsm_vec[i]->alignment_score_) <= std::round(mng_ptr->comp_evalue_min_match_frag_num_)) {
                    e_prsm_vec[i]->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());
                    e_prsm->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());

                } else {
                    if (e_prsm_vec[i]->getEValue() == 0.0) {
                        LOG_WARN("Invalid e value!");
                        e_prsm_vec[i]->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());
                        e_prsm->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());
//                        std::cout<<"EEE"<<e_prsm_vec[i]->getEValue()<<std::endl;
                    }
                }

                std::ofstream outFile;
                std::string result_file = mng_ptr->result_path + "LCSA_scoreboard_Evalue.txt";
                outFile.open(result_file, std::ios::app);
                outFile << std::to_string(spec_set_ptr->getSpectrumId()) + "\t" + e_prsm_vec[i]->prot_name_ + "\t" +

                           //                                  std::to_string(best_start_pos) + "\t" +
                           std::to_string(e_prsm_vec[i]->alignment_score_) + "\t" << std::scientific << std::setprecision(3)<<
                           e_prsm_vec[i]->getEValue() << std::endl;
//                e_prsm->E_value_ = e_prsm_vec[i]->getEValue();
                outFile.close();
            }


        };

    }




std::function<void()> geneTask(SpectrumSetPtr spec_set_ptr, 
                               const PrsmPtrVec & sele_prsm_ptrs,
                               double ppo, bool is_separate,
                               TdgfMngPtr mng_ptr, CountTestNumPtr test_num_ptr,
                               SimpleThreadPoolPtr pool_ptr,
                               PrsmXmlWriterPtrVec writer_ptr_vec) {

  return [spec_set_ptr, sele_prsm_ptrs, ppo, is_separate, mng_ptr, test_num_ptr, pool_ptr, writer_ptr_vec]() {
    PrsmPtrVec prsm_vec; // copy sele_prsm_ptrs
    for (size_t i = 0; i < sele_prsm_ptrs.size(); i++) {
      prsm_vec.push_back(std::make_shared<Prsm>(*sele_prsm_ptrs[i].get()));
    }
    CompPValueArrayPtr comp_pvalue_ptr = std::make_shared<CompPValueArray>(test_num_ptr, mng_ptr);
    comp_pvalue_ptr->process(spec_set_ptr, prsm_vec, ppo, is_separate);

    for (unsigned i = 0; i < prsm_vec.size(); i++) {
      if (std::round(prsm_vec[i]->getMatchFragNum()) <= std::round(mng_ptr->comp_evalue_min_match_frag_num_)) {
        prsm_vec[i]->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());
      } else {
        if (prsm_vec[i]->getEValue() == 0.0) {
          LOG_WARN("Invalid e value!");
          prsm_vec[i]->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());
        }
      }
    }

    boost::thread::id thread_id = boost::this_thread::get_id();
    int writer_id = pool_ptr->getId(thread_id);
    for (size_t i = 0; i < prsm_vec.size(); i++) {
      writer_ptr_vec[writer_id]->write(prsm_vec[i]);
    }
  };
}


void EValueProcessor::process_1(bool is_separate, std::vector<Evalue_PrsmPtr> e_prsm_vec){
    PrsmParaPtr prsm_para_ptr = mng_ptr_->prsm_para_ptr_;
    std::vector<double> prec_error_vec = {0};
    SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
    double ppo = sp_para_ptr->getPeakTolerancePtr()->getPpo();
    int group_spec_num = prsm_para_ptr->getGroupSpecNum();
    SimpleThreadPoolPtr pool_ptr = std::make_shared<SimpleThreadPool>(mng_ptr_->thread_num_);
    for(int i = 0 ; i < e_prsm_vec.size(); i++){
        SpectrumSetPtrVec spec_set_vec
                = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(e_prsm_vec[i]->deconv_ms_ptr_vec_,
                                                                           sp_para_ptr, prec_error_vec);
        SpectrumSetPtr spec_set_ptr = spec_set_vec[0];
        while (pool_ptr->getQueueSize() >= mng_ptr_->thread_num_ + 2) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        }
        pool_ptr->Enqueue(geneTask_1(spec_set_ptr, e_prsm_vec[i], ppo, is_separate,
                                   mng_ptr_, test_num_ptr_, pool_ptr));
    }



    pool_ptr->ShutDown();


    std::sort(e_prsm_vec.begin(), e_prsm_vec.end(), E_value_compare);
    int total_decoy_num = 0;
    for (unsigned i = 0; i < e_prsm_vec.size(); i++){
        std::string seq_name = e_prsm_vec[i]->prot_name_;
        if(seq_name.find("DECOY_")==0){
            total_decoy_num = total_decoy_num + 1;
        }
    }
    int decoy = total_decoy_num;
    int preserve = e_prsm_vec.size();
    double thresh;
    for (unsigned i = 0; i < e_prsm_vec.size(); i++){
        double cur_thresh = e_prsm_vec[i]->getEValue();
        double fdr = float(decoy) / float(preserve);
        if(fdr <= mng_ptr_->obj_fdr){
            thresh = cur_thresh;
            break;
        }else{
            preserve = preserve - 1;
            if(e_prsm_vec[i]->prot_name_.find("DECOY_")==0){
                decoy = decoy - 1;
            }
        }
    }
    for (unsigned i = 0; i < e_prsm_vec.size(); i++) {
        if(e_prsm_vec[i]->getEValue() <= thresh) {
            SpectrumSetPtrVec spec_set_vec
                    = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(e_prsm_vec[i]->deconv_ms_ptr_vec_,
                                                                               sp_para_ptr, prec_error_vec);
            SpectrumSetPtr spec_set_ptr = spec_set_vec[0];
            std::ofstream outFile;
            std::string result_file = mng_ptr_->result_path + "LCSA_scoreboard_FDR.txt";
            outFile.open(result_file, std::ios::app);
            outFile << std::to_string(spec_set_ptr->getSpectrumId()) + "\t" + e_prsm_vec[i]->prot_name_ + "\t" +

                       //                                  std::to_string(best_start_pos) + "\t" +
                       std::to_string(e_prsm_vec[i]->alignment_score_) + "\t" << std::scientific
                    << std::setprecision(3) <<
                    e_prsm_vec[i]->getEValue() << std::endl;

            outFile.close();
        }
    }

}


// Compute E-value. Separate: compute E-values for PrSMs separately or not
void EValueProcessor::process(bool is_separate) {
  PrsmParaPtr prsm_para_ptr = mng_ptr_->prsm_para_ptr_;
  std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
  std::string output_file_name = file_util::basename(sp_file_name) + "." + mng_ptr_->output_file_ext_;
  PrsmXmlWriter writer(output_file_name);

  std::string db_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder();
  FastaIndexReaderPtr seq_reader = std::make_shared<FastaIndexReader>(db_file_name);
  ModPtrVec fix_mod_ptr_vec = prsm_para_ptr->getFixModPtrVec();
  std::string input_file_name = file_util::basename(sp_file_name) + "." + mng_ptr_->input_file_ext_;
  PrsmReader prsm_reader(input_file_name);
  PrsmPtr prsm_ptr = prsm_reader.readOnePrsm(seq_reader, fix_mod_ptr_vec);

  // init variables
  int spectrum_num = msalign_util::getSpNum(sp_file_name);
  SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
  double ppo = sp_para_ptr->getPeakTolerancePtr()->getPpo();
  int group_spec_num = prsm_para_ptr->getGroupSpecNum();
  SimpleMsAlignReaderPtr reader_ptr = std::make_shared<SimpleMsAlignReader>(sp_file_name, group_spec_num,
                                                                            sp_para_ptr->getActivationPtr());

  PrsmXmlWriterPtrVec writer_ptr_vec = 
      prsm_xml_writer_util::geneWriterPtrVec(output_file_name, mng_ptr_->thread_num_);

  SimpleThreadPoolPtr pool_ptr = std::make_shared<SimpleThreadPool>(mng_ptr_->thread_num_);

  int cnt = 0;
  SpectrumSetPtr spec_set_ptr;

  while((spec_set_ptr = spectrum_set_factory::readNextSpectrumSetPtr(reader_ptr, sp_para_ptr))!= nullptr){
    cnt += group_spec_num;
    if(spec_set_ptr->isValid()){
      PrsmPtrVec selected_prsm_ptrs;
      while (prsm_ptr != nullptr && prsm_ptr->getSpectrumId() == spec_set_ptr->getSpectrumId()) {
        selected_prsm_ptrs.push_back(prsm_ptr);
        prsm_ptr = prsm_reader.readOnePrsm(seq_reader, fix_mod_ptr_vec);
      }
      if (!mng_ptr_->use_gf_) {
        processOneSpectrum(spec_set_ptr, selected_prsm_ptrs, ppo, is_separate, writer);
      } else if (checkPrsms(selected_prsm_ptrs)) {
        while (pool_ptr->getQueueSize() >= mng_ptr_->thread_num_ + 2) {
          boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        }
        pool_ptr->Enqueue(geneTask(spec_set_ptr, selected_prsm_ptrs, ppo, is_separate,
                                   mng_ptr_, test_num_ptr_, pool_ptr, writer_ptr_vec));
      }
    }

    std::cout << std::flush << "E-value computation - processing " << cnt << " of " 
        << spectrum_num << " spectra.\r";
  }
  int remainder = spectrum_num - cnt;
  if (prsm_para_ptr->getGroupSpecNum() > remainder && remainder > 0){
      //if there are spectrum remaining because they were not combined due to not having enough pairs
      //fix the message as the processing is completed.
      //this code avoids error when no combined spectra is used but a scan is remaining unprocessed
      //because then it will not satisfy the first condition
      std::cout << std::flush <<  "E-value computation - processing " << spectrum_num
        << " of " << spectrum_num << " spectra.\r";
  } 
  pool_ptr->ShutDown();
  std::cout << std::endl;
  prsm_reader.close();
  prsm_xml_writer_util::closeWriterPtrVec(writer_ptr_vec);
  writer.close();

  if (mng_ptr_->use_gf_) {
    int prsm_top_num = INT_MAX; 
    std::vector<std::string> input_exts;
    for (int t = 0; t < mng_ptr_->thread_num_; t++) {
      input_exts.push_back(mng_ptr_->output_file_ext_ + "_" + str_util::toString(t));
    }
    PrsmStrMergePtr merge_ptr
        = std::make_shared<PrsmStrMerge>(sp_file_name, input_exts, 
                                         mng_ptr_->output_file_ext_, prsm_top_num);
    merge_ptr->process();
    merge_ptr = nullptr;
  }

  // remove tempory files
  file_util::cleanTempFiles(sp_file_name, mng_ptr_->output_file_ext_ + "_");
}

bool EValueProcessor::checkPrsms(const PrsmPtrVec &prsm_ptrs) {
  for (size_t i = 0; i < prsm_ptrs.size(); i++) {
    ExpectedValuePtr extreme_value_ptr = prsm_ptrs[i]->getExpectedValuePtr();
    if (extreme_value_ptr != nullptr) {
      double evalue = extreme_value_ptr->getEValue();
      double frag_num = prsm_ptrs[i]->getMatchFragNum();
      if (evalue <= mng_ptr_->computation_evalue_cutoff
          && frag_num >= mng_ptr_->computation_frag_num_cutoff) {
        return false;
      }
    }
  }
  return true;
}

void EValueProcessor::compEvalues(SpectrumSetPtr spec_set_ptr, PrsmPtrVec &sele_prsm_ptrs,
                                  double ppo, bool is_separate) {
  if (!mng_ptr_->use_gf_ 
      && comp_pvalue_table_ptr_->inTable(spec_set_ptr->getDeconvMsPtrVec(), sele_prsm_ptrs)) {
    comp_pvalue_table_ptr_->process(spec_set_ptr->getDeconvMsPtrVec(), sele_prsm_ptrs, ppo);
    //LOG_DEBUG("Using table");
  } else {
    comp_pvalue_ptr_->process(spec_set_ptr, sele_prsm_ptrs, ppo, is_separate);
  }

  // if matched peak number is too small or E-value is 0, replace it
  // with a max evalue.
  for (unsigned i = 0; i < sele_prsm_ptrs.size(); i++) {
    //LOG_DEBUG("Fragment number " << sele_prsm_ptrs[i]->getMatchFragNum());
    if (std::round(sele_prsm_ptrs[i]->getMatchFragNum()) <= std::round(mng_ptr_->comp_evalue_min_match_frag_num_)) {
      //LOG_DEBUG("Set max e value ");
      sele_prsm_ptrs[i]->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());
    } else {
      if (sele_prsm_ptrs[i]->getEValue() == 0.0) {
        LOG_WARN("Invalid e value!");
        sele_prsm_ptrs[i]->setExpectedValuePtr(ExpectedValue::getMaxEvaluePtr());
      }
    }
  }
}

void EValueProcessor::processOneSpectrum(SpectrumSetPtr spec_set_ptr,
                                         PrsmPtrVec &sele_prsm_ptrs,
                                         double ppo, bool is_separate,
                                         PrsmXmlWriter &writer) {
  if (spec_set_ptr->isValid()) {

    bool need_comp = checkPrsms(sele_prsm_ptrs);

    if (need_comp) {
      compEvalues(spec_set_ptr, sele_prsm_ptrs, ppo, is_separate);
    }

    std::sort(sele_prsm_ptrs.begin(), sele_prsm_ptrs.end(), Prsm::cmpEValueInc);
    writer.writeVector(sele_prsm_ptrs);
  }
}

}

