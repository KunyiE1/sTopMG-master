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

#include <algorithm>
#include <cmath>

#include "common/util/logger.hpp"
#include "common/util/file_util.hpp"
#include "common/base/prot_mod_util.hpp"
#include "seq/proteoform.hpp"
#include "seq/proteoform_factory.hpp"
#include "ms/spec/msalign_util.hpp"
#include "ms/spec/simple_msalign_reader.hpp"
#include "ms/factory/spectrum_set_factory.hpp"
#include "prsm/simple_prsm.hpp"
#include "prsm/simple_prsm_reader.hpp"
#include "prsm/prsm_xml_writer.hpp"
#include "search/varptmsearch/var_ptm_slow_match.hpp"
#include "search/varptmsearch/var_ptm_search_processor.hpp"

namespace toppic {

ProtModPtr findMatchedProtModPtr(ProteoformPtr db_proteo_ptr, 
                                 ProtModPtrVec &mod_ptr_vec, 
                                 double n_term_shift) {
  ResSeqPtr db_res_seq_ptr = db_proteo_ptr->getResSeqPtr();
  ResiduePtrVec residues = db_res_seq_ptr->getResidues();
  for (size_t i = 0; i < mod_ptr_vec.size(); i++) {
    double mod_shift = mod_ptr_vec[i]->getProtShift();
    // allow small errors introduced in double addition
    if (abs(mod_shift - n_term_shift) < 0.0001) {
      bool valid_mod = prot_mod_util::allowMod(mod_ptr_vec[i], residues); 
      if (valid_mod) {
        return mod_ptr_vec[i];
      }
    }
  }
  LOG_ERROR("Failed to find protein modification!" << n_term_shift);
  exit(EXIT_FAILURE);
}

int findMatchedPos(ProteoformPtr proteo_ptr, double n_term_trunc) {
  std::vector<double> prms = proteo_ptr->getBpSpecPtr()->getPrmMasses();
  for (size_t i = 0; i < prms.size(); i++) {
    // allow small errors introduced in double addition
    if (abs(prms[i] - n_term_trunc) < 0.0001) {
      return i;
    }
  }
  LOG_ERROR("Failed to find start posisiton!" << n_term_trunc);
  exit(EXIT_FAILURE);
}

PrsmPtrVec VarPtmSearchProcessor::varPtmSearchOneSpec(SpectrumSetPtr spec_set_ptr,
                                                      const SimplePrsmPtrVec &simple_prsm_ptr_vec,
                                                      FastaIndexReaderPtr reader_ptr,
                                                      VarPtmSearchMngPtr mng_ptr,
                                                      ProteoformTypePtr type_ptr) {
  ModPtrVec fix_mod_list = mng_ptr->prsm_para_ptr_->getFixModPtrVec();
  ProtModPtrVec prot_mod_ptr_vec = mng_ptr->prsm_para_ptr_->getProtModPtrVec();
  ProteoformPtrVec proteoform_ptr_vec;

    clock_t getsubseq_s, getsubseq_e;
    getsubseq_s = clock();

  for (size_t i = 0; i < simple_prsm_ptr_vec.size(); i++) {
    if (std::abs(spec_set_ptr->getPrecMonoMass() - simple_prsm_ptr_vec[i]->getPrecMass()) 
        > std::pow(10, -4)) {
      // When precursor error is allowed, if the adjusted precursor of the
      // spectrum set does not match the adjusted precursor mass in the
      // filtering result, the spectrum proteoform match is ignored. 
      // A small error is allowed for errors introduced in writing real numbers
      // to xml files. 
      continue;
    }

    SimplePrsmPtr simple_prsm_ptr = simple_prsm_ptr_vec[i];
    std::string seq_name = simple_prsm_ptr->getSeqName();
    std::string seq_desc = simple_prsm_ptr->getSeqDesc();
    ProteoformPtr db_proteo_ptr
        = proteoform_factory::readFastaToProteoformPtr(reader_ptr, seq_name, seq_desc, fix_mod_list);
    double n_term_shift = simple_prsm_ptr->getNTermShifts()[0];
    double c_term_shift = simple_prsm_ptr->getCTermShifts()[0];
    LOG_DEBUG("type " << type_ptr->getName() << " proteoform mass " << db_proteo_ptr->getMass()
              << " n_term_shift " << n_term_shift << " c term shift " << c_term_shift);
    
    ProteoformPtr proteo_with_prot_mod_ptr = db_proteo_ptr;
    // start position is relative to the database sequence
    int start_pos = 0;
    if (type_ptr == ProteoformType::COMPLETE || type_ptr == ProteoformType::PREFIX) {
      if (n_term_shift != 0) {
        // Get the proteoform with N-terminal modificaiton
        ProtModPtr prot_mod_ptr = findMatchedProtModPtr(db_proteo_ptr, prot_mod_ptr_vec, n_term_shift);
        LOG_DEBUG("Db seq: " << db_proteo_ptr->getProteoformMatchSeq());
        proteo_with_prot_mod_ptr = proteoform_factory::geneProtModProteoform(db_proteo_ptr, prot_mod_ptr);
        if (proteo_with_prot_mod_ptr == nullptr) {
          LOG_ERROR("Null proteoform is reported!");
          exit(EXIT_FAILURE);
        }
        start_pos = proteo_with_prot_mod_ptr->getStartPos();
        LOG_DEBUG("Start pos with N terminal mod: " << start_pos);
      }
    }
    else {
      double n_term_trunc = - n_term_shift;
      start_pos = findMatchedPos(db_proteo_ptr, n_term_trunc);
    }
    int end_pos = db_proteo_ptr->getEndPos();
    if (type_ptr == ProteoformType::PREFIX || type_ptr == ProteoformType::INTERNAL) {
      double proteo_minus_water_mass = db_proteo_ptr->getMinusWaterMass();
      double n_term_trunc = proteo_minus_water_mass + c_term_shift;
      // the reported position is the position of matched breakpoint
      // the sequence end position is breakpoint position - 1
      end_pos = findMatchedPos(db_proteo_ptr, n_term_trunc) - 1; 
    }
    LOG_DEBUG("Start pos " << start_pos << " end position " << end_pos); 
    
    // get the start and end positions relative to the proteoform with
    // N-terminal modification
    int local_start_pos = start_pos - proteo_with_prot_mod_ptr->getStartPos();
    //std::cout<<"spos"<<local_start_pos<<std::endl;
    int local_end_pos = end_pos - proteo_with_prot_mod_ptr->getStartPos();
     // std::cout<<"epos"<<local_end_pos<<std::endl;
    FastaSeqPtr fasta_seq_ptr = db_proteo_ptr->getFastaSeqPtr();
    ProteoformPtr sub_proteo_ptr = proteoform_factory::geneSubProteoform(proteo_with_prot_mod_ptr,
                                                                         fasta_seq_ptr,
                                                                         local_start_pos, 
                                                                         local_end_pos);
    LOG_DEBUG("sub seq: " << sub_proteo_ptr->getProteoformMatchSeq());
    LOG_DEBUG("protein mass " << sub_proteo_ptr->getMass() 
              << "proteoform mass diff " << (simple_prsm_ptr->getPrecMass() - sub_proteo_ptr->getMass()));
    proteoform_ptr_vec.push_back(sub_proteo_ptr);

  }
    getsubseq_e = clock();
  //std::cout<<"get sub seq time:"<<double(getsubseq_e - getsubseq_s)/CLOCKS_PER_SEC<<"s"<<std::endl;
  //std::cout<<"subseq"<<proteoform_ptr_vec[0]->getResSeqPtr()->toString()<<std::endl;
  PrsmPtrVec prsms;
  for (size_t i = 0; i < proteoform_ptr_vec.size(); i++) {
    VarPtmSlowMatch slow_match(proteoform_ptr_vec[i], spec_set_ptr, mng_ptr);
    if (slow_match.isSuccessInit()) {
      PrsmPtr tmp = slow_match.compute();
      LOG_DEBUG("Slow match computation finish!");
      if (tmp != nullptr) {
        prsms.push_back(tmp);
      }
    }
  }
  std::sort(prsms.begin(), prsms.end(), Prsm::cmpMatchFragmentDecMatchPeakDec);
  if (prsms.size() > 0) {
    prsms.erase(prsms.begin() + mng_ptr->n_report_, prsms.end());
  }
  return prsms;
}

void VarPtmSearchProcessor::process() {


  PrsmParaPtr prsm_para_ptr = mng_ptr_->prsm_para_ptr_;
    std::string db_block_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder();
    std::cout<<"db block file name: "<< db_block_file_name << std::endl;
    ProteoformPtrVec raw_forms
            = proteoform_factory::readFastaToProteoformPtrVec(db_block_file_name,
                                                              prsm_para_ptr->getFixModPtrVec(),
                                                              false);
    std::cout<<"rawforms size: "<<raw_forms.size()<<std::endl;
  std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
  std::string input_file_name = file_util::basename(sp_file_name) + "." + mng_ptr_->input_file_ext_;
  SimplePrsmReader comp_prsm_reader(input_file_name + "_" + ProteoformType::COMPLETE->getName());
  SimplePrsmReader pref_prsm_reader(input_file_name + "_" + ProteoformType::PREFIX->getName());
  SimplePrsmReader suff_prsm_reader(input_file_name + "_" + ProteoformType::SUFFIX->getName());
  SimplePrsmReader internal_prsm_reader(input_file_name + "_" + ProteoformType::INTERNAL->getName());
  SimplePrsmPtr comp_prsm_ptr = comp_prsm_reader.readOnePrsm();
  SimplePrsmPtr pref_prsm_ptr = pref_prsm_reader.readOnePrsm();
  SimplePrsmPtr suff_prsm_ptr = suff_prsm_reader.readOnePrsm();
  SimplePrsmPtr internal_prsm_ptr = internal_prsm_reader.readOnePrsm();

  std::string output_file_name = file_util::basename(prsm_para_ptr->getSpectrumFileName())
      + "." + mng_ptr_->output_file_ext_;
  PrsmXmlWriter comp_writer(output_file_name + "_" + ProteoformType::COMPLETE->getName());
  PrsmXmlWriter pref_writer(output_file_name + "_" + ProteoformType::PREFIX->getName());
  PrsmXmlWriter suff_writer(output_file_name + "_" + ProteoformType::SUFFIX->getName());
  PrsmXmlWriter internal_writer(output_file_name + "_" + ProteoformType::INTERNAL->getName());

  // init variables
  std::string db_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder();

  FastaIndexReaderPtr reader_ptr = std::make_shared<FastaIndexReader>(db_file_name);
  int spectrum_num = msalign_util::getSpNum(sp_file_name);
  SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
  ModPtrVec fix_mod_ptr_vec = prsm_para_ptr->getFixModPtrVec();

  int group_spec_num = prsm_para_ptr->getGroupSpecNum();
  SimpleMsAlignReaderPtr msalign_reader_ptr = std::make_shared<SimpleMsAlignReader>(sp_file_name, 
                                                                                    group_spec_num,
                                                                                    sp_para_ptr->getActivationPtr());
  int cnt = 0;
  DeconvMsPtrVec deconv_ms_ptr_vec = msalign_reader_ptr->getNextMsPtrVec(); 
  std::vector<double> prec_error_vec = sp_para_ptr->getVarPtmSearchPrecErrorVec();


    std::vector<std::vector<int>> node_mass_list_vec;
    for(int j = 0; j < raw_forms.size(); j ++){
        ResSeqPtr seq = raw_forms[j]->getResSeqPtr();
        std::vector<int> node_mass_list;
        int res_int_total_mass = 0;
        double res_double_total_mass = 0;
        node_mass_list.push_back(0);
        for(int i = 0; i < raw_forms[j]->getLen(); i++){
            res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
            res_int_total_mass = round(res_double_total_mass * mng_ptr_->convert_ratio);
            node_mass_list.push_back(res_int_total_mass);
        }
        node_mass_list_vec.push_back(node_mass_list);
    }

  //LOG_ERROR("start ");
  while (deconv_ms_ptr_vec.size() > 0) {
      clock_t one_sp_s = clock();
    std::vector<SpectrumSetPtr> spec_set_vec 
        = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(deconv_ms_ptr_vec, 
                                                                   sp_para_ptr,
                                                                   prec_error_vec);
      int spec_id = spec_set_vec[0]->getSpectrumId();
    if (spec_set_vec.size() == 0) {
      LOG_ERROR("Spectrum set size is 0!");
    }

    //LOG_ERROR("Start search");
    cnt+= group_spec_num;
    if (spec_set_vec[0]->isValid()) {
        for (size_t k = 0; k < spec_set_vec.size(); k++) {
        SpectrumSetPtr spec_set_ptr = spec_set_vec[k];

        int delta = 27;

        std::vector<int> mm_list;
        for (int i = 0; i < mng_ptr_->T_mass_comb_.size(); i++) {
            mm_list.push_back(std::get<0>(mng_ptr_->T_mass_comb_[i]));
        }
        mm_list.push_back(0);
        mm_list.push_back(delta);
        std::sort(mm_list.begin(), mm_list.end());
        int mm_min = mm_list[0];
        int mm_max = mm_list.back();

        int match_num = 0;


        clock_t mass_filter_s, mass_filter_e;
        mass_filter_s = clock();
        //for(int i = 0; i < prm_spec_vec.size(); i ++){
        std::vector<std::vector<std::pair<int, int>>> filtered_seg;
        std::vector<std::vector<std::vector<int>>> bgn_node_lists;
        int prec_mass_minus_water = round(
                (spec_set_ptr->getPrecMonoMass() - mass_constant::getWaterMass()) * mng_ptr_->convert_ratio);
        int proteo_num = raw_forms.size();

        for (int j = 0; j < proteo_num; j++) {
            //if(raw_forms[j]->getSeqName() == "sp|P0AFB8|NTRC_ECOLI") {
            std::vector<std::pair<int, int>> seg_list;
            std::vector<int> node_mass_list;

            node_mass_list = node_mass_list_vec[j];
            ResSeqPtr seq = raw_forms[j]->getResSeqPtr();
            int y;
            int range_left = 1;
            for (int x = 0; x < node_mass_list.size(); x++) {
                int end_pos = -1;
                y = range_left;
                while (y < node_mass_list.size() &&
                       node_mass_list[y] - node_mass_list[x] - prec_mass_minus_water <
                       -mm_max - delta) {
                    y++;
                }
                //std::cout<<x<<","<<y<<std::endl;
                //std::cout<<node_mass_list.size()<<std::endl;
                if (y < node_mass_list.size()) {
                    range_left = y;
                    while (y < node_mass_list.size() &&
                           node_mass_list[y] - node_mass_list[x] - prec_mass_minus_water <=
                           -mm_min + delta) {
                        int mass_diff =
                                prec_mass_minus_water - (node_mass_list[y] - node_mass_list[x]);
                        auto it = std::lower_bound(mm_list.begin(), mm_list.end(), mass_diff);
                        if (it != mm_list.end()) {
                            int z = std::distance(mm_list.begin(), it);
                            if (mm_list[z] - mass_diff <= delta) {
                                end_pos = y;
                                //std::cout<<x<<","<<y<<","<<prec_mass_minus_water<<","<<node_mass_list[y] - node_mass_list[x]<<","<<mass_diff<<","<<mm_list[z]<<std::endl;
                            } else {
                                if (z - 1 >= 0 && mass_diff - mm_list[z - 1] <= delta) {
                                    end_pos = y;
                                    //std::cout<<x<<","<<y<<","<<prec_mass_minus_water<<","<<node_mass_list[y] - node_mass_list[x]<<","<<mass_diff<<","<<mm_list[z - 1]<<std::endl;
                                }
                            }
                        }
                        y++;
                    }
                } else {
                    break;
                }
                if (end_pos != -1) {
                    match_num = match_num + 1;
//                                      if (raw_forms[j]->getSeqName() == "sp|P0AFB8|NTRC_ECOLI") {
//                                          std::cout << x << "," << end_pos << std::endl;
//                                      }
                    seg_list.push_back(std::make_pair(x, end_pos));
                }
            }
//            //std::cout<<"breakbreak"<<std::endl;
//            std::vector<std::pair<int, int>> merge_seg_list;
//            std::vector<std::vector<int>> bgn_node_list;
//            if (!seg_list.empty()) {
//                merge_seg_list.push_back(seg_list[0]);
//                bgn_node_list.push_back({seg_list[0].first});
//                for (int k = 1; k < seg_list.size(); k++) {
//                    if (seg_list[k].first <= merge_seg_list.back().second) {
//                        merge_seg_list.back().second = seg_list[k].second;
//                        bgn_node_list.back().push_back(seg_list[k].first);
//                    } else {
//                        merge_seg_list.push_back(seg_list[k]);
//                        bgn_node_list.push_back({seg_list[k].first});
//                    }
//                }
//            }
//            //std::cout<<"seeee"<<std::endl;
            filtered_seg.push_back(seg_list);
            //bgn_node_lists.push_back(bgn_node_list);
            //}
        }

        PrsmPtrVec prsms;
        ProteoformPtrVec proteoform_ptr_vec;
        //if (spec_id == 88){
        for (int j = 0; j < proteo_num; j++) {
            if (!filtered_seg[j].empty()) {
                for (int seg_i = 0; seg_i < filtered_seg[j].size(); seg_i++) {
                    int start_pos = filtered_seg[j][seg_i].first;
                    int end_pos = filtered_seg[j][seg_i].second - 1;
                    ProteoformPtr sub_proteo_ptr = proteoform_factory::geneSubProteoform(raw_forms[j],
                                                                                         raw_forms[j]->getFastaSeqPtr(),
                                                                                         start_pos,
                                                                                         end_pos);
                    proteoform_ptr_vec.push_back(sub_proteo_ptr);

                }
            }
        }
        std::cout<<proteoform_ptr_vec.size()<<std::endl;
        for (size_t i = 0; i < proteoform_ptr_vec.size(); i++) {

            VarPtmSlowMatch slow_match(proteoform_ptr_vec[i], spec_set_ptr, mng_ptr_);
            if (slow_match.isSuccessInit()) {
                PrsmPtr tmp = slow_match.compute();
                LOG_DEBUG("Slow match computation finish!");
                if (tmp != nullptr) {
                    prsms.push_back(tmp);
                }
            }
        }
        std::sort(prsms.begin(), prsms.end(), Prsm::cmpMatchFragmentDecMatchPeakDec);
        if (prsms.size() > 0) {
            prsms.erase(prsms.begin() + 1, prsms.end());
        }
        internal_writer.writeVector(prsms);

    }

//    //     ori var ptm search
//        // complete
//      SimplePrsmPtrVec comp_selected_prsm_ptrs;
//      while (comp_prsm_ptr != nullptr && comp_prsm_ptr->getSpectrumId() == spec_id) {
//        comp_selected_prsm_ptrs.push_back(comp_prsm_ptr);
//        comp_prsm_ptr = comp_prsm_reader.readOnePrsm();
//      }
//
//
//
//      //LOG_ERROR("complete list size " << comp_selected_prsm_ptrs.size());
//      if (comp_selected_prsm_ptrs.size() > 0) {
//        // LOG_DEBUG("start processing one spectrum.");
//        for (size_t k = 0; k < spec_set_vec.size(); k++) {
//          PrsmPtrVec prsms = varPtmSearchOneSpec(spec_set_vec[k], comp_selected_prsm_ptrs,
//                                                 reader_ptr, mng_ptr_, ProteoformType::COMPLETE);
//          comp_writer.writeVector(prsms);
//        }
//      }
//
//
//      // prefix
//      SimplePrsmPtrVec pref_selected_prsm_ptrs;
//      while (pref_prsm_ptr != nullptr && pref_prsm_ptr->getSpectrumId() == spec_id) {
//        pref_selected_prsm_ptrs.push_back(pref_prsm_ptr);
//        pref_prsm_ptr = pref_prsm_reader.readOnePrsm();
//      }
//      LOG_DEBUG("prefix list size " << pref_selected_prsm_ptrs.size());
//      if (pref_selected_prsm_ptrs.size() > 0) {
//        // LOG_DEBUG("start processing one spectrum.");
//        for (size_t k = 0; k < spec_set_vec.size(); k++) {
//          PrsmPtrVec prsms = varPtmSearchOneSpec(spec_set_vec[k], pref_selected_prsm_ptrs,
//                                                 reader_ptr, mng_ptr_, ProteoformType::PREFIX);
//          pref_writer.writeVector(prsms);
//        }
//      }
//
//      // suffix
//      SimplePrsmPtrVec suff_selected_prsm_ptrs;
//      while (suff_prsm_ptr != nullptr && suff_prsm_ptr->getSpectrumId() == spec_id) {
//        suff_selected_prsm_ptrs.push_back(suff_prsm_ptr);
//        suff_prsm_ptr = suff_prsm_reader.readOnePrsm();
//      }
//      LOG_DEBUG("suffix list size " << suff_selected_prsm_ptrs.size());
//      if (suff_selected_prsm_ptrs.size() > 0) {
//        // LOG_DEBUG("start processing one spectrum.");
//        for (size_t k = 0; k < spec_set_vec.size(); k++) {
//          PrsmPtrVec prsms = varPtmSearchOneSpec(spec_set_vec[k], suff_selected_prsm_ptrs,
//                                                 reader_ptr, mng_ptr_, ProteoformType::SUFFIX);
//          suff_writer.writeVector(prsms);
//        }
//      }
//
//      // internal
//      SimplePrsmPtrVec internal_selected_prsm_ptrs;
//      while (internal_prsm_ptr != nullptr && internal_prsm_ptr->getSpectrumId() == spec_id) {
//        internal_selected_prsm_ptrs.push_back(internal_prsm_ptr);
//        internal_prsm_ptr = internal_prsm_reader.readOnePrsm();
//      }
//      LOG_DEBUG("internal list size " << internal_selected_prsm_ptrs.size());
//      if (internal_selected_prsm_ptrs.size() > 0) {
//        // LOG_DEBUG("start processing one spectrum.");
//        for (size_t k = 0; k < spec_set_vec.size(); k++) {
//          PrsmPtrVec prsms = varPtmSearchOneSpec(spec_set_vec[k], internal_selected_prsm_ptrs,
//                                                 reader_ptr, mng_ptr_, ProteoformType::INTERNAL);
//          internal_writer.writeVector(prsms);
//        }
//      }
//     ori var ptm search

        clock_t one_sp_e = clock();
        std::cout<< "spid:"<< spec_id << " time: "<< double(one_sp_e - one_sp_s)/CLOCKS_PER_SEC<<"s"<<std::endl;
    }

    std::cout << std::flush <<  "Variable PTM search - processing " << cnt
        << " of " << spectrum_num << " spectra.\r";
    deconv_ms_ptr_vec = msalign_reader_ptr->getNextMsPtrVec(); 
  }
  int remainder = spectrum_num - cnt;
  if (prsm_para_ptr->getGroupSpecNum() > remainder && remainder > 0){
      //if there are spectrum remaining because they were not combined due to not having enough pairs
      //fix the message as the processing is completed.
      //this code avoids error when no combined spectra is used but a scan is remaining unprocessed
      //because then it will not satisfy the first condition
      std::cout << std::flush <<  "Variable PTM search - processing " << spectrum_num
        << " of " << spectrum_num << " spectra.\r";
  } 
  comp_prsm_reader.close();
  pref_prsm_reader.close();
  suff_prsm_reader.close();
  internal_prsm_reader.close();
  comp_writer.close();
  pref_writer.close();
  suff_writer.close();
  internal_writer.close();
  std::cout << std::endl;
}

}  // namespace toppic
