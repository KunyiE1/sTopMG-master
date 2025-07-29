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

#include "filter/LCS/lcs_filter.hpp"
#include "search/LCS/lcs_alignment_processor.hpp"

#include "prsm/simple_prsm_reader.hpp"
#include "prsm/simple_prsm_util.hpp"

#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include "common/util/logger.hpp"

#include <fstream>
#include "ms/factory/prm_ms_util.hpp"
#include "seq/fasta_sub_util.hpp"
#include "search/graph/dist.hpp"
#include "prsm/prsm.hpp"
#include "search/varptmsearch/var_ptm_slow_match.hpp"
#include <time.h>
#include "prsm/evalue_prsm.hpp"
#include "stat/tdgf/tdgf_mng.hpp"
#include "stat/tdgf/evalue_processor.hpp"


namespace toppic {

    static int min_res_mass = 15643;
    static int max_res_mass = 51048;

    static bool cmpbin(LCSFilter::BucketPtr x,LCSFilter::BucketPtr y){
        return x->self_score > y->self_score;
    }

    static bool tuple_cmp(std::tuple<int,int,int> &a, std::tuple<int,int,int> &b) {
        return std::get<2>(a) < std::get<2>(b);
    }

    void outputProgressBar(float progress) {
        // 进度条的长度
        int barWidth = 50;

        // 计算已完成的进度比例
        int pos = progress * barWidth;

        // 输出进度条
        std::cout << "Progress [" << std::setw(barWidth) << std::setfill('=') << "" << "] " << std::fixed
                  << std::setprecision(1) << (progress * 100) << "%\r";
        std::cout.flush(); // 刷新输出缓冲区，确保立即显示

        // 如果进度已经达到100%，输出换行
        if (progress >= 1)
            std::cout << std::endl;
    }

    std::vector<LCSFilter::SpPairPtr> getSpPairs(PrmPeakPtrVec & peak_vec, std::vector<int> & bgn_ys, int k){
        int peak_num = peak_vec.size();
        int max_mass = k * max_res_mass + 27;

        std::vector<LCSFilter::SpPairPtr> pair_list;
        for(int x = 0; x < peak_num - 1; x++){
            int y = bgn_ys[x];
            while(y < peak_vec.size()){
                int mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
                if(mass <= max_mass) {
                    LCSFilter::SpPairPtr pair = std::make_shared<LCSFilter::SpPair>(x, y, mass);
                    pair_list.push_back(pair);
                    y++;
                }else{
                    break;
                }
            }
        }
        return pair_list;
    }

    std::vector<LCSFilter::SpPairPtr> getSpPairs(LCSFilter::PrmPeakSpecPtr & prm_spec, int k){
        PrmPeakPtrVec peak_vec = prm_spec->getPeakVec();
        std::vector<int> bgn_ys = prm_spec->getBgn_ys();
        int peak_num = peak_vec.size();
        int max_mass = k * max_res_mass + 27;

        std::vector<LCSFilter::SpPairPtr> pair_list;
        for(int x = 0; x < peak_num - 1; x++){
            int y = bgn_ys[x];
            while(y < peak_vec.size()){
                int mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
                if(mass <= max_mass) {
                    LCSFilter::SpPairPtr pair = std::make_shared<LCSFilter::SpPair>(x, y, mass);
                    pair_list.push_back(pair);
                    y++;
                }else{
                    break;
                }
            }
        }
        return pair_list;
    }


    static   DistVec getSpPairs_1(PrmPeakPtrVec & peak_vec, std::vector<int> & bgn_ys, int k){
        int peak_num = peak_vec.size();
        int max_mass = k * max_res_mass + 27;
        std::set<Dist> dist_set;


        for(int x = 0; x < peak_num - 1; x++){
            int y = bgn_ys[x];
            while(y < peak_vec.size()){
                int mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
                if(mass <= max_mass) {
                    Dist tmp = Dist(mass, x, y);
                    auto search = dist_set.find(tmp);

                    if (search != dist_set.end()) {
                        search->pair_ij_.push_back(std::pair<int, int>(x, y));
                    } else {
                        dist_set.insert(tmp);
                    }
                    y++;
                }else{
                    break;
                }

            }
        }

        DistVec dist_vec;
        std::copy(dist_set.begin(), dist_set.end(), std::back_inserter(dist_vec));
        std::sort(dist_vec.begin(), dist_vec.end(), distVecUp);
        return dist_vec;
    }


static LCSFilter::ProtDataPtr getProtData_1(ProteoformPtr & proteo_ptr, LCSFilterMngPtr & mng_ptr){
        int k = mng_ptr->K;
        int max_mass = 0;
        int min_mass = 100000;
        std::vector<int> prot_peak_mass_list;
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();
        std::vector<std::pair<int,int>> mods_pos_list;
        std::vector<std::pair<int,std::vector<int>>> modlist_pos;
        int res_int_total_mass = 0;
        double res_double_total_mass = 0;
        std::set<Dist> dist_set;
        prot_peak_mass_list.push_back(0);
        for(int i = 0; i < proteo_ptr->getLen(); i++) {
            res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
            res_int_total_mass = round(res_double_total_mass * mng_ptr->convert_ratio);
            prot_peak_mass_list.push_back(res_int_total_mass);
            std::string resletter = seq->getResiduePtr(i)->getAminoAcidPtr()->getOneLetter();
            std::vector<int> mods_idx_list;
            for (int j = 0; j < mng_ptr->RES_MOD_TABLE[resletter].size(); j++) {
                std::string mod_name = mng_ptr->RES_MOD_TABLE[resletter][j];
                for (int q = 0; q < mng_ptr->MODS_VEC.size(); q++){
                    if(mod_name == mng_ptr->MODS_VEC[q]){
                        mods_idx_list.push_back(q);
                    }
                }
            }
            if(!mods_idx_list.empty()){
                modlist_pos.push_back(std::make_pair(i, mods_idx_list));
            }
        }
        for(int i = 0; i < proteo_ptr->getLen(); i++){
            double tmp_mass = 0;
            int res_num = 1;
            while(i + res_num - 1 < proteo_ptr->getLen() && res_num <= k) {
                tmp_mass = tmp_mass + seq->getResiduePtr(i + res_num - 1)->getMass();
                int int_mass = round(tmp_mass * mng_ptr->convert_ratio);
                Dist tmp = Dist(int_mass, i, i + res_num);
                auto search = dist_set.find(tmp);
                if (search != dist_set.end()) {
                    search->pair_ij_.push_back(std::pair<int, int>(i, i + res_num));
                } else {
                    dist_set.insert(tmp);
                }
                if(int_mass > max_mass){
                    max_mass = int_mass;
                }
                if(int_mass < min_mass){
                    min_mass = int_mass;
                }
                res_num++;
            }
        }
        DistVec dist_vec;
        std::copy(dist_set.begin(), dist_set.end(), std::back_inserter(dist_vec));
        std::sort(dist_vec.begin(), dist_vec.end(), distVecUp);
        LCSFilter::ProtDataPtr protdataptr = std::make_shared<LCSFilter::ProtData>(dist_vec, mods_pos_list, prot_peak_mass_list, max_mass + 54);
        protdataptr->modlist_for_pos_ = modlist_pos;
        return protdataptr;
    }


//static  LCSFilter::ProtDataPtr  getProtData_1(ProteoformPtr & proteo_ptr, LCSFilterMngPtr & mng_ptr){
//        int k = mng_ptr->K;
//        int max_mass = 0;
//        int min_mass = 100000;
//        std::vector<int> prot_peak_mass_list;
//        ResSeqPtr seq = proteo_ptr->getResSeqPtr();
//        std::vector<std::pair<int,int>> mods_pos_list;
//        int res_int_total_mass = 0;
//        double res_double_total_mass = 0;
//        std::set<Dist> dist_set;
//        prot_peak_mass_list.push_back(0);
//        for(int i = 0; i < proteo_ptr->getLen(); i++){
//            res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
//            res_int_total_mass = round(res_double_total_mass * mng_ptr->convert_ratio);
//            prot_peak_mass_list.push_back(res_int_total_mass);
//            for(int m = 0; m < mng_ptr->MODS_VEC.size(); m++){
//                if(seq->getResiduePtr(i)->getAminoAcidPtr()->getOneLetter() == mng_ptr->MODS_VEC[m]){
//                    mods_pos_list.push_back(std::make_pair(i,m));
//                }
//            }
//        }
//        for(int i = 0; i < proteo_ptr->getLen(); i++){
//            double tmp_mass = 0;
//            int res_num = 1;
//            while(i + res_num - 1 < proteo_ptr->getLen() && res_num <= k) {
//                tmp_mass = tmp_mass + seq->getResiduePtr(i + res_num - 1)->getMass();
//                int int_mass = round(tmp_mass * mng_ptr->convert_ratio);
//                Dist tmp = Dist(int_mass, i, i + res_num);
//                auto search = dist_set.find(tmp);
//                if (search != dist_set.end()) {
//                    search->pair_ij_.push_back(std::pair<int, int>(i, i + res_num));
//                } else {
//                    dist_set.insert(tmp);
//                }
//                if(int_mass > max_mass){
//                    max_mass = int_mass;
//                }
//                if(int_mass < min_mass){
//                    min_mass = int_mass;
//                }
//                res_num++;
//            }
//        }
//        DistVec dist_vec;
//        std::copy(dist_set.begin(), dist_set.end(), std::back_inserter(dist_vec));
//        std::sort(dist_vec.begin(), dist_vec.end(), distVecUp);
//        LCSFilter::ProtDataPtr protdataptr = std::make_shared<LCSFilter::ProtData>(dist_vec, mods_pos_list, prot_peak_mass_list, max_mass + 27);
//        return protdataptr;
//    }



    LCSFilter::ProtDataPtr getProtData(ProteoformPtr & proteo_ptr, LCSFilterMngPtr mng_ptr){
        int k = mng_ptr->K;
        std::unordered_map<int, std::vector<int>> mass_table;
        std::unordered_map<int, std::vector<std::pair<int, int>>> mass_pos_table;
        std::vector<std::vector<int>> follow_mass_table(proteo_ptr->getLen() + 1);
        int max_mass = 0;
        int min_mass = 100000;
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();
        std::vector<std::pair<int,int>> mods_pos_list;
        std::vector<int> prot_peak_mass_list;
        int res_int_total_mass = 0;
        double res_double_total_mass = 0;
        prot_peak_mass_list.push_back(0);
        for(int i = 0; i < proteo_ptr->getLen(); i++){
            //res_total_mass = res_total_mass + round(seq->getResiduePtr(i)->getMass() * mng_ptr->convert_ratio);
            res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
            res_int_total_mass = res_double_total_mass * mng_ptr->convert_ratio;
            prot_peak_mass_list.push_back(res_int_total_mass);
            for(int m = 0; m < mng_ptr->MODS_VEC.size(); m++){
                if(seq->getResiduePtr(i)->getAminoAcidPtr()->getOneLetter() == mng_ptr->MODS_VEC[m]){
                    mods_pos_list.push_back(std::make_pair(i,m));
                }
            }
        }
        //mng_ptr->seq_peak_mass_list = prot_peak_mass_list;
        //std::cout<<mods_pos_list.size()<<std::endl;
        std::vector<std::tuple<int, int, int>> P_mass;
        mass_table.reserve(proteo_ptr->getLen() * k * 0.5);
        mass_pos_table.reserve(proteo_ptr->getLen() * k);
        for(int i = 0; i < proteo_ptr->getLen(); i++){
            //int tmp_mass = 0;
            double tmp_mass = 0;
            int res_num = 1;
            while(i + res_num - 1 < proteo_ptr->getLen() && res_num <= k) {
                tmp_mass = tmp_mass + seq->getResiduePtr(i + res_num - 1)->getMass();



                int int_mass = round(tmp_mass * mng_ptr->convert_ratio);
                P_mass.push_back(std::make_tuple(i, i + res_num, int_mass));
                int tol_mass = round(int_mass / mng_ptr->fix_tol_);
                mass_table[tol_mass].push_back(int_mass);
                mass_pos_table[int_mass].emplace_back(i, i + res_num);
                follow_mass_table[i].push_back(int_mass);
                if(int_mass > max_mass){
                    max_mass = int_mass;
                }
                if(int_mass < min_mass){
                    min_mass = int_mass;
                }
                res_num++;
            }
        }
        sort(P_mass.begin(), P_mass.end(), tuple_cmp);
        for(auto it = mass_table.begin(); it != mass_table.end(); ++it) {
            std::vector<int> int_mass_vec = it->second;
            if(int_mass_vec.size() >= 2){
                sort(int_mass_vec.begin(), int_mass_vec.end());
                int_mass_vec.erase(unique(int_mass_vec.begin(), int_mass_vec.end()), int_mass_vec.end());
                mass_table[it->first] = int_mass_vec;
            }
        }

        LCSFilter::ProtDataPtr protdataptr = std::make_shared<LCSFilter::ProtData>(mass_table, mass_pos_table, mods_pos_list, prot_peak_mass_list, max_mass + 27);
        return protdataptr;
    }


    int Lowerbound(PrmPeakPtrVec a, float key){
        int left = 0;
        int right = a.size() - 1;
        while(left <= right){
            int mid=(left + right) >> 1;
            if(a[mid]->getMonoMass() >= key){
                right = mid-1;
            }
            else{left = mid + 1;}
        }
        return left;
    }

    float getLossProb(float x, float max, float min){
        float a = 2 * (max - min);
        float b = max - a;
        float lossprob = a / (1 + exp(- (x - 1)))+ b;
        return lossprob;
    }

        inline void filterBlock(ProteoformPtrVec & raw_forms,
                        int block_idx,
                        LCSFilterMngPtr mng_ptr,
                        const std::vector<double> & mod_mass_list) {
  std::string block_str = str_util::toString(block_idx);
  LCSFilterPtr filter_ptr = std::make_shared<LCSFilter>(raw_forms, mng_ptr, block_str);

    struct cand_cmp
    {
        bool operator() (const std::pair<int,int> & p1,const std::pair<int,int> & p2)
        {
            //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
            return p1.second > p2.second;
        }
    };

    mng_ptr->max_total_iso_num = mng_ptr->search_max_total_iso_num;
  PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
  SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
  std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
  int group_spec_num = mng_ptr->prsm_para_ptr_->getGroupSpecNum();

  std::string db_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder();
  FastaIndexReaderPtr fasta_reader_ptr = std::make_shared<FastaIndexReader>(db_file_name);

  SimpleMsAlignReaderPtr reader_ptr = std::make_shared<SimpleMsAlignReader>(sp_file_name,
                                                                            group_spec_num,
                                                                            sp_para_ptr->getActivationPtr());

//  // init writer
//  std::string output_file_name = file_util::basename(prsm_para_ptr->getSpectrumFileName())
//      + "." + mng_ptr->output_file_ext_+"_"+ block_str;
//
//  SimplePrsmXmlWriterPtr writer_ptr = std::make_shared<SimplePrsmXmlWriter>(output_file_name);

    std::string input_file_name
                    = file_util::basename(sp_file_name) + "." + mng_ptr->input_file_ext_;

//    SimplePrsmReader simple_prsm_reader(input_file_name);
//    SimplePrsmStrPtr prsm_ptr = simple_prsm_reader.readOnePrsmStr();


  DeconvMsPtrVec deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();

  std::vector<double> prec_error_vec;
  if(mng_ptr->use_adjusted_precmass){
//      prec_error_vec = sp_para_ptr->getExtendOffsets();
        prec_error_vec = {mass_constant::getIsotopeMass()};
  }else{
      prec_error_vec = sp_para_ptr->getMultiShiftSearchPrecErrorVec();
  }




    std::vector<int> mods_mass_list;
    std::vector<int> mods_num_list;
  for(auto it = mng_ptr->mod_mass_table_.begin(); it != mng_ptr->mod_mass_table_.end(); it++){
        mods_mass_list.push_back(it->first);
        int mods_num = 0;
        for(int mn = 0; mn < mng_ptr->MODS_VEC.size(); mn++){
            mods_num = mods_num + it->second[mn];
        }
        mods_num_list.push_back(mods_num);
    }
  mng_ptr->mods_mass_list_ = mods_mass_list;
    mng_ptr->mods_num_list_ = mods_num_list;

  int proteo_num = raw_forms.size();
  clock_t total_start,total_end;
  clock_t total, prot_s, prot_e;
  clock_t s,e;
  total_start = clock();
  s = clock();
  prot_s = clock();
  std::vector<LCSFilter::ProtDataPtr> prot_data_vec;
//  for(int i = 0; i < raw_forms.size(); i ++){
//      LCSFilter::ProtDataPtr protdataptr = getProtData_1(raw_forms[i], mng_ptr);
//      prot_data_vec.push_back(protdataptr);
//  }
  prot_e = clock();
  std::cout<<"prot proceeded in: "<<double(prot_e - prot_s)/CLOCKS_PER_SEC<<std::endl;

            prot_s = clock();
            std::vector<std::vector<int>> node_mass_list_vec;
            for(int j = 0; j < raw_forms.size(); j ++){
                ResSeqPtr seq = raw_forms[j]->getResSeqPtr();
                std::vector<int> node_mass_list;
                int res_int_total_mass = 0;
                double res_double_total_mass = 0;
                std::set<Dist> dist_set;
                node_mass_list.push_back(0);
                for(int i = 0; i < raw_forms[j]->getLen(); i++){
                    res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
                    res_int_total_mass = round(res_double_total_mass * mng_ptr->convert_ratio);
                    node_mass_list.push_back(res_int_total_mass);
                }
                node_mass_list_vec.push_back(node_mass_list);
            }
            prot_e = clock();
            std::cout<<"prot proceeded in: "<<double(prot_e - prot_s)/CLOCKS_PER_SEC<<std::endl;

  std::vector<LCSFilter::PrmPeakSpecPtr> prm_spec_vec;


  std::vector<Evalue_PrsmPtr> Eprsm_vec;

  while (deconv_ms_ptr_vec.size() != 0) {
      // allow one dalton error
      SpectrumSetPtrVec spec_set_vec
              = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(deconv_ms_ptr_vec,
                                                                         sp_para_ptr, prec_error_vec);
      int max_of_three = 0;
      ResSeqPtr best_seq_of_three;
      std::vector<LCSFilter::BucketPtr> best_path_of_three;
      std::vector<std::vector<LCSFilter::ModsFound>> best_dp_of_three;
      int best_k;
      int best_start_pos_of_three;
      std::vector<int> best_seq_mass_list_of_three;
      std::string best_proteo_of_three;
      ProteoformPtr best_protptr;
      PrmPeakPtrVec best_peak_vec;

      for (size_t k = 0; k < spec_set_vec.size(); k++) {

          int peak_num;
          SpectrumSetPtr spec_set_ptr = spec_set_vec[k];
          int spec_id = spec_set_ptr->getSpectrumId();
          mng_ptr->sp_id = spec_set_ptr->getSpectrumId();
          //std::cout<<"PrecMonoMass:"<<spec_set_ptr->getPrecMonoMass()<<std::endl;
          if (spec_set_ptr->isValid() && spec_id != -1) {
              if (mng_ptr->var_num_ == 0) {
                  PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
                  SimplePrsmPtrVec match_ptrs = filter_ptr->getBestMatch(ms_ptr_vec);
              } else {
                  DeconvMsPtrVec deconv_ms_ptr_vec = spec_set_ptr->getDeconvMsPtrVec();
                  double prec_mono_mass = spec_set_ptr->getPrecMonoMass();

                  std::vector<double> mod_mass(0);
                  //std::cout<<"ms peak num: "<<deconv_ms_ptr_vec[0]->size()<<std::endl;
//         PrmMsPtrVec ms_ptr_vec = prm_ms_factory::geneMsSixPtrVec(deconv_ms_ptr_vec,
//                                                                      sp_para_ptr,
//                                                                      prec_mono_mass);

                  std::vector<SimplePrsmStrPtr> selected_prsm_ptrs;
//                  while (prsm_ptr != nullptr && prsm_ptr->getSpectrumId() == spec_id) {
//                      selected_prsm_ptrs.push_back(prsm_ptr);
//                      prsm_ptr = simple_prsm_reader.readOnePrsmStr();
//                  }
                  PrmMsPtrVec ms_ptr_vec;
                  if(mng_ptr->use_MS6) {
                      ms_ptr_vec = spec_set_ptr->getMsSixPtrVec();
                  }else {
                      ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
                  }
                  mng_ptr->sp_id = spec_set_ptr->getSpectrumId();
                  //if (mng_ptr->sp_id == 0) {

                  PrmPeakPtrVec peak_vec = prm_ms_util::getPrmPeakPtrs(ms_ptr_vec,
                                                                       sp_para_ptr->getPeakTolerancePtr());

                  int max_tole = 0;
                  for (int i = 0; i < peak_vec.size(); i++) {
                      if (peak_vec[i]->getIntTolerance() > max_tole);
                      max_tole = peak_vec[i]->getIntTolerance();
//                      max_tole = peak_vec[i]->getIntNRelaxCStrictTolerance();
                  }


                  mng_ptr->max_bin_tol_ = max_tole;
//                  std::cout<<"max tolerance: "<<max_tole<<std::endl;

                  std::vector<int> bgn_ys;
                  for (int x = 0; x < peak_vec.size() - 1; x++) {
                      int y = x;
                      while (y < peak_vec.size()) {
                          int mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
                          if (mass < min_res_mass) {
                              y++;
                          } else {
                              break;
                          }
                      }
                      bgn_ys.push_back(y);
                  }

                  clock_t sp_s, sp_e;
                  sp_s = clock();

                  //std::vector<LCSFilter::SpPairPtr> pair_list = getSpPairs_1(peak_vec, bgn_ys, mng_ptr->K);

                  sp_e = clock();
//                  std::cout << "step2: " << (double) (sp_e - sp_s) / CLOCKS_PER_SEC << "s" << std::endl;


                  std::vector<int> mm_list;
                  for (int i = 0; i < mng_ptr->T_mass_comb_.size(); i++) {
                      mm_list.push_back(get<0>(mng_ptr->T_mass_comb_[i]));
                  }


//                  if(mng_ptr->NEG_ISO_SHIFT_IDX == -1 && mng_ptr->POS_ISO_SHIFT_IDX == -1){
//                      for (int i = 0; i < mng_ptr->T_mass_comb_.size(); i++) {
//                          mm_list.push_back(get<0>(mng_ptr->T_mass_comb_[i]));
//                      }
//                  }
//                  else{
//                      for (int i = 0; i < mng_ptr->T_mass_comb_.size(); i++) {
//                          if (get<1>(mng_ptr->T_mass_comb_[i])[mng_ptr->NEG_ISO_SHIFT_IDX] == 0 &&
//                              get<1>(mng_ptr->T_mass_comb_[i])[mng_ptr->POS_ISO_SHIFT_IDX] == 0) {
//                              mm_list.push_back(get<0>(mng_ptr->T_mass_comb_[i]));
//                          }
//                      }
//                  }


                  mm_list.push_back(0);
                  mm_list.push_back(mng_ptr->delta);
                  std::cout<<"mmlist size: "<<mm_list.size();
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
                          (spec_set_ptr->getPrecMonoMass() - mass_constant::getWaterMass()) * mng_ptr->convert_ratio);
                  int prec_adjust_tole = round(prec_mass_minus_water * sp_para_ptr->getPeakTolerancePtr()->getPpo());
                  int fixed_tole = mng_ptr->delta;
                  int prec_tole;
                  if (mng_ptr->mass_filter_use_fixed_tole) {
                      prec_tole = fixed_tole;
                  } else {
                      prec_tole = prec_adjust_tole;
                  }

                  //prec_tole = fixed_tole;
                  for (int j = 0; j < proteo_num; j++) {
                      if (!mng_ptr->whole_protein_only) {
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
                                     -mm_max - prec_tole) {
                                  y++;
                              }
                              if (y < node_mass_list.size()) {
                                  range_left = y;
                                  while (y < node_mass_list.size() &&
                                         node_mass_list[y] - node_mass_list[x] - prec_mass_minus_water <=
                                         -mm_min + prec_tole) {
                                      int mass_diff = prec_mass_minus_water - (node_mass_list[y] - node_mass_list[x]);
                                      auto it = std::lower_bound(mm_list.begin(), mm_list.end(), mass_diff);
                                      if (it != mm_list.end()) {
                                          int z = std::distance(mm_list.begin(), it);
                                          if (abs(mm_list[z] - mass_diff) <= prec_tole) {
                                              end_pos = y;
                                          } else {
                                              if (z - 1 >= 0 && mass_diff - mm_list[z - 1] <= prec_tole) {
                                                  end_pos = y;
                                              }
                                          }
                                      }
                                      if (end_pos != -1) {
                                          seg_list.push_back(std::make_pair(x, end_pos));
                                          end_pos = -1;
                                      }
                                      y++;
                                  }
                              } else {
                                  break;
                              }
//                       if (end_pos != -1) {
//                           seg_list.push_back(std::make_pair(x, end_pos));
//                       }
                          }
                          std::vector<std::pair<int, int>> merge_seg_list;
                          std::vector<std::vector<int>> bgn_node_list;
                          std::vector<std::vector<int>> end_node_list;
                          if (!seg_list.empty()) {
                              merge_seg_list.push_back(seg_list[0]);
                              bgn_node_list.push_back({seg_list[0].first});
                              end_node_list.push_back({seg_list[0].second});
                              for (int k = 1; k < seg_list.size(); k++) {
                                  if (seg_list[k].first <= merge_seg_list.back().second) {
                                      if (merge_seg_list.back().second < seg_list[k].second) {
                                          merge_seg_list.back().second = seg_list[k].second;
                                      }
                                      bgn_node_list.back().push_back(seg_list[k].first);
                                      end_node_list.back().push_back(seg_list[k].second);
                                  } else {
                                      merge_seg_list.push_back(seg_list[k]);
                                      bgn_node_list.push_back({seg_list[k].first});
                                      end_node_list.push_back({seg_list[k].second});
                                  }
                              }
                          }
                          //std::cout<<"seeee"<<std::endl;
                          filtered_seg.push_back(merge_seg_list);
                          bgn_node_lists.push_back(bgn_node_list);
//                          end_node_lists.push_back(end_node_list);
                      }else{
                          std::vector<std::pair<int, int>> seg_list;
                          std::vector<int> node_mass_list;
                          node_mass_list = node_mass_list_vec[j];
                          int mass_diff = prec_mass_minus_water - node_mass_list.back();
//                          std::cout<<mass_diff<<std::endl;

                          auto it = std::lower_bound(mm_list.begin(), mm_list.end(), mass_diff);
                          if (it != mm_list.end()) {
                              int z = std::distance(mm_list.begin(), it);
                              //std::cout<<"mm"<<mm_list[z]<<std::endl;
                              if (abs(mm_list[z] - mass_diff) <= prec_tole ||
                                  (z - 1 >= 0 && abs(mm_list[z - 1] - mass_diff) <= prec_tole)) {
                                  int end_pos = node_mass_list.size() - 1;
                                  seg_list.push_back(std::make_pair(0, end_pos));
                              }
                          }
                          std::vector<std::pair<int, int>> merge_seg_list;
                          std::vector<std::vector<int>> bgn_node_list;
                          std::vector<std::vector<int>> end_node_list;
                          if (!seg_list.empty()) {
                              merge_seg_list.push_back(seg_list[0]);
                              bgn_node_list.push_back({seg_list[0].first});
                              end_node_list.push_back({seg_list[0].second});
                              for (int k = 1; k < seg_list.size(); k++) {
                                  if (seg_list[k].first <= merge_seg_list.back().second) {
                                      if (merge_seg_list.back().second < seg_list[k].second) {
                                          merge_seg_list.back().second = seg_list[k].second;
                                      }
                                      bgn_node_list.back().push_back(seg_list[k].first);
                                      end_node_list.back().push_back(seg_list[k].second);
                                  } else {
                                      merge_seg_list.push_back(seg_list[k]);
                                      bgn_node_list.push_back({seg_list[k].first});
                                      end_node_list.push_back({seg_list[k].second});
                                  }
                              }
                          }
                          //std::cout<<"seeee"<<std::endl;
                          filtered_seg.push_back(merge_seg_list);
                          bgn_node_lists.push_back(bgn_node_list);
//                          end_node_lists.push_back(end_node_list);
                      }
                  }
                  //}
                  mass_filter_e = clock();
//                  std::cout<<"mass-filter time: "<<(double)(mass_filter_e - mass_filter_s)/CLOCKS_PER_SEC<<"s"<<std::endl;
                  std::cout << "match seg num:" << match_num << std::endl;
//
                  int filtered_protnum = 0;
                  for (int j = 0; j < proteo_num; j++) {
                      if (!filtered_seg.empty()) {
                          filtered_protnum = filtered_protnum + 1;
                      }
                  }
//                  std::cout << "filtered prot num: " << filtered_protnum << std::endl;

//                  for(int j = 0; j < proteo_num; j++){
//                      if(raw_forms[j]->getSeqName() == "sp|P0AFB8|NTRC_ECOLI"){
//                          std::vector<std::vector<int>> temp = bgn_node_lists[j];
//                          std::cout<<11111<<std::endl;
//                      }
//                  }

                  int max_score = 0;
                  ResSeqPtr best_seq;
                  int best_start_pos;
                  std::vector<int> best_seq_peak_mass_list;
                  std::string best_proteo_name;
                  std::vector<LCSFilter::BucketPtr> best_path;
                  std::vector<std::vector<LCSFilter::ModsFound>> best_dp;

                  std::vector<ProteoformPtr> max_align_prot_list;
                  ProteoformPtr max_align_prot;

                  ProteoformPtr best_proteo;
                  std::cout << "sp: " << mng_ptr->sp_id << " alignment processing" << std::endl;
                  std::vector<int> selected_prot_idx_list;

//                  ProteoformPtrVec selected_prots;
//                  std::vector<LCSFilter::ProtDataPtr> selected_prot_data_vec;

//                  for (int cp = 0; cp < selected_prsm_ptrs.size(); cp++) {
//                      std::string seq_name = selected_prsm_ptrs[cp]->getSeqName();
//                      std::string seq_desc = selected_prsm_ptrs[cp]->getSeqDesc();
//                      FastaSeqPtr seq_ptr = fasta_reader_ptr->readFastaSeq(seq_name, seq_desc);
//                      ProteoformPtr selected_form = proteoform_factory::geneDbProteoformPtr(seq_ptr, prsm_para_ptr->getFixModPtrVec(), false);
//                      selected_prots.push_back(selected_form);
//                      LCSFilter::ProtDataPtr protdataptr = getProtData_1(selected_form, mng_ptr);
//                      selected_prot_data_vec.push_back(protdataptr);
//                  }

//                  std::string seq_info_file = mng_ptr->resultpath + "seq_info.txt";
//                  std::ifstream in(seq_info_file);
//                  std::string line;
//                  std::vector<std::string> seqs_name;
//                  while(std::getline(in,line)){
//                      std::stringstream ss(line.substr(0, line.size()-1));
//                      std::string tmp;
//                      std::vector<std::string> tmp_vec;
//                      std::vector<std::vector<int>> mods_vec_list;
//
//                      while(std::getline(ss, tmp, '\t')){
//                          tmp_vec.push_back(tmp);
//                      }
//                      std::string name = tmp_vec[0];
//                      seqs_name.push_back(name);
//
//                  }


//
//                  for (int cp = 0; cp < selected_prsm_ptrs.size(); cp++) {
//
//
////
//                          //if (raw_forms[j]->getSeqName() == seqs_name[spec_set_ptr->getSpectrumId()]) {
//                              mng_ptr->proteo_name = selected_prsm_ptrs[cp]->getSeqName(); ;
//                              int j;
//                              for (int j_i = 0; j_i < raw_forms.size(); j_i ++){
//                                  if(mng_ptr->proteo_name == raw_forms[j_i]->getSeqName()){
//                                      j = j_i;
//                                  }
//                              }
                  //s = clock();










//                if (spec_id == 41){
                  if (mng_ptr->K == -1) {
                      int score = 0;
                      int shifting = 0;
                      int max_score_shifting = 0;
//                      for (int j = 0; j < filtered_seg.size(); j++) {
                      for (int j = 0; j < proteo_num; j++) {
                          score = 0;
                          if (!filtered_seg[j].empty()) {
//                              std::cout<<"+++++"<<j<<std::endl;
                              for (int seg_i = 0; seg_i < filtered_seg[j].size(); seg_i++) {
                                  int start_pos = filtered_seg[j][seg_i].first;
                                  int end_pos = filtered_seg[j][seg_i].second - 1;
                                  std::vector<int> bgn_nodes = bgn_node_lists[j][seg_i];
                                  bgn_nodes.erase(unique(bgn_nodes.begin(),bgn_nodes.end()),bgn_nodes.end());
                                  for (int b = 0; b < bgn_nodes.size(); b++) {
                                      bgn_nodes[b] = bgn_nodes[b] - start_pos;
                                  }

//                                  bgn_nodes = {0};
//                                  std::cout<<"start"<<start_pos<<std::endl;
                                  ProteoformPtr sub_proteo_ptr = proteoform_factory::geneSubProteoform(raw_forms[j],
                                                                                                       raw_forms[j]->getFastaSeqPtr(),
                                                                                                       start_pos,
                                                                                                       end_pos);
                                  LCSFilter::ProtDataPtr prot_data = getProtData_1(sub_proteo_ptr, mng_ptr);

                                  score = filter_ptr->Compute_noK(peak_vec, sub_proteo_ptr, prot_data, bgn_nodes);
                                  if (score > 0) {
                                      ResSeqPtr seq = sub_proteo_ptr->getResSeqPtr();
//                                      shifting = filter_ptr->getTotalShifting(filter_ptr->best_ss_path,
//                                                                              filter_ptr->best_ss_dp, seq, prot_data,
//                                                                              peak_vec);
                                      if (max_score < score) {
                                          max_score = score;
                                          max_score_shifting = shifting;
                                          best_proteo = sub_proteo_ptr;
                                          best_path = filter_ptr->best_ss_path;
                                          best_dp = filter_ptr->best_ss_dp;
                                          best_seq = sub_proteo_ptr->getResSeqPtr();
                                          best_proteo_name = sub_proteo_ptr->getSeqName();
                                          best_seq_peak_mass_list = mng_ptr->seq_peak_mass_list;
                                          best_start_pos = sub_proteo_ptr->getStartPos();
                                          //filter_ptr->backtrack_2(best_path, best_dp, best_seq);
                                      }
                                  }
                              }

                          }
                          outputProgressBar(float(j) / float(raw_forms.size()));
                      }

                  } else {
                      int score = 0;
                      int shifting = 0;
                      int max_score_shifting = 0;

                      DistVec sp_dist_vec;
                      if (match_num > 0) {
                          sp_dist_vec = getSpPairs_1(peak_vec, bgn_ys, mng_ptr->K);
                      }

                      //if (spec_id == 88){
                      for (int j = 0; j < proteo_num; j++) {
                          if (!filtered_seg[j].empty()) {
                              for (int seg_i = 0; seg_i < filtered_seg[j].size(); seg_i++) {
                                  int start_pos = filtered_seg[j][seg_i].first;
                                  int end_pos = filtered_seg[j][seg_i].second - 1;
                                  std::vector<int> bgn_nodes = bgn_node_lists[j][seg_i];
                                  for (int b = 0; b < bgn_nodes.size(); b++) {
                                      bgn_nodes[b] = bgn_nodes[b] - start_pos;
                                  }
                                  ProteoformPtr sub_proteo_ptr = proteoform_factory::geneSubProteoform(raw_forms[j],
                                                                                                       raw_forms[j]->getFastaSeqPtr(),
                                                                                                       start_pos,
                                                                                                       end_pos);
                                  LCSFilter::ProtDataPtr prot_data = getProtData_1(sub_proteo_ptr, mng_ptr);
                                  //if (raw_forms[j]->getSeqName() == seqs_name[spec_set_ptr->getSpectrumId()]) {

                                  score = filter_ptr->Compute(peak_vec, sub_proteo_ptr, sp_dist_vec, prot_data,
                                                              bgn_nodes);
                                  //std::cout<<"+++____++++"<<std::endl;
                                  if (score > 0) {
                                      ResSeqPtr seq = sub_proteo_ptr->getResSeqPtr();

//                                      shifting = filter_ptr->getTotalShifting(filter_ptr->best_ss_path,
//                                                                              filter_ptr->best_ss_dp, seq, prot_data,
//                                                                              peak_vec);
                                      if (max_score < score) {
                                          max_score = score;
                                          max_score_shifting = shifting;
                                          best_proteo = sub_proteo_ptr;
                                          best_path = filter_ptr->best_ss_path;
                                          best_dp = filter_ptr->best_ss_dp;
                                          best_seq = sub_proteo_ptr->getResSeqPtr();
                                          best_proteo_name = sub_proteo_ptr->getSeqName();
                                          best_seq_peak_mass_list = mng_ptr->seq_peak_mass_list;
                                          best_start_pos = sub_proteo_ptr->getStartPos();
//                                          filter_ptr->backtrack_2(best_path, best_dp, best_seq);



                                      }
                                  }
                              }

                          }
//                          outputProgressBar(float(j) / float(raw_forms.size()));
                      }
                  }
//              }

                  if (max_score != 0 && max_score >= max_of_three) {
                      best_proteo_of_three = best_proteo_name;
                      best_seq_mass_list_of_three = best_seq_peak_mass_list;
                      best_start_pos_of_three = best_start_pos;
                      mng_ptr->sp_id = spec_set_ptr->getSpectrumId();
                      best_path_of_three = best_path;
                      best_dp_of_three = best_dp;
                      best_seq_of_three = best_seq;
                      best_k = k;
                      max_of_three = max_score;
                      best_peak_vec = peak_vec;
                      best_protptr = best_proteo;
                  }
                  sp_e = clock();
//                  std::cout << std::endl;
                  std::cout << "sp: " << spec_set_ptr->getSpectrumId() << "offset: "<< mng_ptr->prec_error_vec[k] << "proceeded using: "
                            << double(sp_e - sp_s) / CLOCKS_PER_SEC << "s"
                            << std::endl;

              }
          }

      }
      if(max_of_three > 0) {
          mng_ptr->peak_vec_ = best_peak_vec;
          mng_ptr->proteo_name = best_proteo_of_three;
          mng_ptr->seq_peak_mass_list = best_seq_mass_list_of_three;
          mng_ptr->start_pos = best_start_pos_of_three;
          mng_ptr->best_protptr_ = best_protptr;
          filter_ptr->backtrack_2(best_path_of_three, best_dp_of_three, best_seq_of_three);
      }

      std::string result_file = mng_ptr->resultpath + "LCSA_scoreboard.txt";

      std::ofstream outFile;
      outFile.open(result_file, std::ios::app);
      if(max_of_three <= 0){
          std::cout<<"==="<<std::to_string(mng_ptr->sp_id)<< std::endl;
          outFile << std::to_string(mng_ptr->sp_id) + "\t" + "-- -- -- --"+ "\t" +
                  "--"+ "\t" +
                     std::to_string(0) << std::endl;
      }else{
          outFile << std::to_string(mng_ptr->sp_id) + "\t" + mng_ptr->proteo_name  + "\t" +
                     std::to_string(mng_ptr->prec_error_vec[best_k]) + "\t" +
                     std::to_string(max_of_three) << std::endl;

          int ori_len = mng_ptr->best_protptr_->getFastaSeqPtr()->getRawSeq().size();
//          std::cout<<"ori len"<<ori_len<<std::endl;
          ProteoformPtr ori_proteo = proteoform_factory::geneDbProteoformPtr(mng_ptr->best_protptr_->getFastaSeqPtr(), prsm_para_ptr->getFixModPtrVec(), false);
          ProteoformPtr match_proteo = proteoform_factory::geneSubProteoform(ori_proteo, mng_ptr->best_protptr_->getFastaSeqPtr(), mng_ptr->start_pos, mng_ptr->end_pos);
          std::cout<<match_proteo->getResSeqPtr()->toAcidString()<<std::endl;
          double res_mass_sum = match_proteo->getResSeqPtr()->getResMassSum();

//          std::cout<<"start end:"<<mng_ptr->start_pos<<","<<mng_ptr->end_pos<<std::endl;
          Evalue_PrsmPtr e_prsm = std::make_shared<Evalue_Prsm>(deconv_ms_ptr_vec, max_of_three, mng_ptr->total_mod_num_in_best_, mng_ptr->best_protptr_->getSeqName(), res_mass_sum);
          if(mng_ptr->start_pos == 0){
              if(mng_ptr->end_pos != ori_len - 1) {
                  e_prsm->type_ptr = ProteoformType::PREFIX;
              }else{
                  e_prsm->type_ptr = ProteoformType::COMPLETE;
              }
          }else{
              if(mng_ptr->end_pos != ori_len - 1) {
                  e_prsm->type_ptr = ProteoformType::INTERNAL;
              }else{
                  e_prsm->type_ptr = ProteoformType::SUFFIX;
              }
          }
          Eprsm_vec.push_back(e_prsm);


      }
      outFile.close();
      deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();
  }
  e = clock();
  std::cout<<"Spectra Processed, LCS-Alignment Finished "<< double(e - s)/CLOCKS_PER_SEC << "s" << std::endl;
  std::ofstream outFile;
  std::string result_file = mng_ptr->resultpath + "LCSA_scoreboard.txt";
//          +"(K="
//          + std::to_string(mng_ptr->K)
//          + ",maxmods=" + std::to_string(mng_ptr->max_total_mods_num) + ").txt";

  outFile.open(result_file, std::ios::app);
  outFile <<"Spectra Processed, LCS-Alignment Finished "<< double(e - s)/CLOCKS_PER_SEC << "s" << std::endl ;
  outFile.close();


            std::cout << "E-value computation - started." << std::endl;
            bool use_gf = true;


//           MCMCMngPtr mcmc_mng_ptr
//        = std::make_shared<MCMCMng>(prsm_para_ptr, "topmg_graph_post", "topmg_evalue",
//                                    "var_mod_file_name", 5, 1);
//    DprProcessorPtr processor = std::make_shared<DprProcessor>(mcmc_mng_ptr);
//    processor->process();
//    processor = nullptr;




            TdgfMngPtr tdgf_mng_ptr
                    = std::make_shared<TdgfMng>(prsm_para_ptr, 0,
                                                std::max(std::abs(mng_ptr->max_shift_mass), std::abs(mng_ptr->min_shift_mass)),
                                                use_gf, mng_ptr->var_ptm_type_num, 1,
                                                "toppic_combined", "toppic_evalue");
            tdgf_mng_ptr->result_path = mng_ptr->resultpath;
            tdgf_mng_ptr->obj_fdr = mng_ptr->obj_fdr_;
            EValueProcessorPtr processor = std::make_shared<EValueProcessor>(tdgf_mng_ptr);
            std::cout<<processor<<std::endl;
            processor->init();
            processor->process_1(false, Eprsm_vec);
            processor = nullptr;


}






//    inline void filterBlock(ProteoformPtrVec & raw_forms,
//                            int block_idx,
//                            LCSFilterMngPtr mng_ptr,
//                            const std::vector<double> & mod_mass_list) {
//        std::string block_str = str_util::toString(block_idx);
//        LCSFilterPtr filter_ptr = std::make_shared<LCSFilter>(raw_forms, mng_ptr, block_str);
//
//        struct cand_cmp
//        {
//            bool operator() (const std::pair<int,int> & p1,const std::pair<int,int> & p2)
//            {
//                //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
//                return p1.second > p2.second;
//            }
//        };
//
//        std::unordered_map<int,std::vector<std::vector<double>>> probsForOffsetN;
//        probsForOffsetN[0] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/0_nodeR_CID.csv");
//        probsForOffsetN[-4667] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/-17_nodeR_CID.csv");
//        probsForOffsetN[-4912] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/-18_nodeR_CID.csv");
//        probsForOffsetN[-7684] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/-28_nodeR_CID.csv");
//        probsForOffsetN[-12351] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/-45_nodeR_CID.csv");
//        probsForOffsetN[-12626] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/-45_nodeR_CID.csv");
//
//
//        std::unordered_map<int,std::vector<std::vector<double>>> probsForOffsetC;
//        probsForOffsetC[0] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/0_nodeL_CID.csv");
//        probsForOffsetC[4667] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/-17_nodeL_CID.csv");
//        probsForOffsetC[4912] = read_csv("/home/kunyili/toppic-suite-main/prob_for_offset/-18_nodeL_CID.csv");
//
//
//
//        std::string seq_info_file = mng_ptr->resultpath + "seq_info.txt";
//        std::ifstream in(seq_info_file);
//        std::string line;
//        std::vector<std::string> seqs_name;
//        while(std::getline(in,line)){
//            std::stringstream ss(line.substr(0, line.size()-1));
//            std::string tmp;
//            std::vector<std::string> tmp_vec;
//            std::vector<std::vector<int>> mods_vec_list;
//
//            while(std::getline(ss, tmp, '\t')){
//                tmp_vec.push_back(tmp);
//            }
//            std::string name = tmp_vec[0];
//            seqs_name.push_back(name);
//
//        }
//
//        PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
//        SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
//        std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
//        int group_spec_num = mng_ptr->prsm_para_ptr_->getGroupSpecNum();
//
//        std::string db_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder();
//        FastaIndexReaderPtr fasta_reader_ptr = std::make_shared<FastaIndexReader>(db_file_name);
//
//        SimpleMsAlignReaderPtr reader_ptr = std::make_shared<SimpleMsAlignReader>(sp_file_name,
//                                                                                  group_spec_num,
//                                                                                  sp_para_ptr->getActivationPtr());
//
////  // init writer
////  std::string output_file_name = file_util::basename(prsm_para_ptr->getSpectrumFileName())
////      + "." + mng_ptr->output_file_ext_+"_"+ block_str;
////
////  SimplePrsmXmlWriterPtr writer_ptr = std::make_shared<SimplePrsmXmlWriter>(output_file_name);
//
//        std::string input_file_name
//                = file_util::basename(sp_file_name) + "." + mng_ptr->input_file_ext_;
//
//        SimplePrsmReader simple_prsm_reader(input_file_name);
//        SimplePrsmStrPtr prsm_ptr = simple_prsm_reader.readOnePrsmStr();
//
//
//        DeconvMsPtrVec deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();
//        std::vector<double> prec_error_vec = sp_para_ptr->getMultiShiftSearchPrecErrorVec();
//
//
//        std::vector<int> mods_mass_list;
//        std::vector<int> mods_num_list;
//        for(auto it = mng_ptr->mod_mass_table_.begin(); it != mng_ptr->mod_mass_table_.end(); it++){
//            mods_mass_list.push_back(it->first);
//            int mods_num = 0;
//            for(int mn = 0; mn < mng_ptr->MODS_VEC.size(); mn++){
//                mods_num = mods_num + it->second[mn];
//            }
//            mods_num_list.push_back(mods_num);
//        }
//        mng_ptr->mods_mass_list_ = mods_mass_list;
//        mng_ptr->mods_num_list_ = mods_num_list;
//
//        int proteo_num = raw_forms.size();
//        clock_t total_start,total_end;
//        clock_t total, prot_s, prot_e;
//        clock_t s,e;
//        total_start = clock();
//
//  prot_s = clock();
//  std::vector<LCSFilter::ProtDataPtr> prot_data_vec;
//
//
//
//
//  for(int i = 0; i < raw_forms.size(); i ++){
//      if(raw_forms[i]->getSeqName() == "sp|P76347|YEEJ_ECOLI") {
//          LCSFilter::ProtDataPtr protdataptr = getProtData_1(raw_forms[i], mng_ptr);
//          prot_data_vec.push_back(protdataptr);
//      }
//  }
//  prot_e = clock();
//  std::cout<<"prot proceeded in: "<<double(prot_e - prot_s)/CLOCKS_PER_SEC<<std::endl;
//
//        std::vector<LCSFilter::PrmPeakSpecPtr> prm_spec_vec;
//        s = clock();
//
//        while (deconv_ms_ptr_vec.size() != 0) {
//            // allow one dalton error
//            SpectrumSetPtrVec spec_set_vec
//                    = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(deconv_ms_ptr_vec,
//                                                                               sp_para_ptr, prec_error_vec);
//
//            for (size_t k = 0; k < spec_set_vec.size(); k++) {
//                int peak_num;
//                SpectrumSetPtr spec_set_ptr = spec_set_vec[k];
//                //std::cout<<"PrecMonoMass:"<<spec_set_ptr->getPrecMonoMass()<<std::endl;
//                if (spec_set_ptr->isValid()) {
//                    if (mng_ptr->var_num_ == 0) {
//                        PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
//                        SimplePrsmPtrVec match_ptrs = filter_ptr->getBestMatch(ms_ptr_vec);
//                    } else {
//                        DeconvMsPtrVec deconv_ms_ptr_vec = spec_set_ptr->getDeconvMsPtrVec();
//                        double prec_mono_mass = spec_set_ptr->getPrecMonoMass();
//                        std::vector<double> mod_mass(0);
//                        //std::cout<<"ms peak num: "<<deconv_ms_ptr_vec[0]->size()<<std::endl;
////         PrmMsPtrVec ms_ptr_vec = prm_ms_factory::geneMsSixPtrVec(deconv_ms_ptr_vec,
////                                                                      sp_para_ptr,
////                                                                      prec_mono_mass);
//                        int spec_id = spec_set_ptr->getSpectrumId();
////                        std::vector<SimplePrsmStrPtr> selected_prsm_ptrs;
////                        while (prsm_ptr != nullptr && prsm_ptr->getSpectrumId() == spec_id) {
////                            selected_prsm_ptrs.push_back(prsm_ptr);
////                            prsm_ptr = simple_prsm_reader.readOnePrsmStr();
////                        }
//                        PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
//                        mng_ptr->sp_id = spec_set_ptr->getSpectrumId();
//                        //if (mng_ptr->sp_id == 0) {
//
//                        PrmPeakPtrVec peak_vec = prm_ms_util::getPrmPeakPtrs(ms_ptr_vec,
//                                                                             sp_para_ptr->getPeakTolerancePtr());
//
//                        std::vector<int> bgn_ys;
//                        for(int x = 0; x < peak_vec.size() - 1; x++){
//                            int y = x;
//                            while(y < peak_vec.size()){
//                                int mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
//                                if(mass < min_res_mass) {
//                                    y++;
//                                }else{
//                                    break;
//                                }
//                            }
//                            bgn_ys.push_back(y);
//                        }
//
//                        clock_t sp_s, sp_e;
//                        sp_s = clock();
//
//                        clock_t getSpPair_s = clock();
//                        //std::vector<LCSFilter::SpPairPtr> pair_list = getSpPairs(peak_vec, bgn_ys, mng_ptr->K);
//                        DistVec sp_dist_vec = getSpPairs_1(peak_vec, bgn_ys, mng_ptr->K);
//                        clock_t getSpPair_e = clock();
//                        std::cout<<"getsppair time: "<<double(getSpPair_e - getSpPair_s)/CLOCKS_PER_SEC<<std::endl;
//
//
//                        int max_score = 0;
//                        ResSeqPtr best_seq;
//                        std::vector<LCSFilter::BucketPtr> best_path;
//                        std::vector<std::vector<LCSFilter::ModsFound>> best_dp;
//                        std::vector<int> best_seq_peak_mass_list;
//                        std::string best_proteo_name;
//
//
//                        ProteoformPtr best_proteo;
//                        std::cout << "sp: " << mng_ptr->sp_id << " alignment processing" << std::endl;
//                        std::vector<int> selected_prot_idx_list;
//
////                        ProteoformPtrVec selected_prots;
////                        std::vector<LCSFilter::ProtDataPtr> selected_prot_data_vec;
//
////                        for (int cp = 0; cp < selected_prsm_ptrs.size(); cp++) {
////                            std::string seq_name = selected_prsm_ptrs[cp]->getSeqName();
////                            std::string seq_desc = selected_prsm_ptrs[cp]->getSeqDesc();
////                            FastaSeqPtr seq_ptr = fasta_reader_ptr->readFastaSeq(seq_name, seq_desc);
////                            ProteoformPtr selected_form = proteoform_factory::geneDbProteoformPtr(seq_ptr, prsm_para_ptr->getFixModPtrVec(), false);
////                            selected_prots.push_back(selected_form);
////                            LCSFilter::ProtDataPtr protdataptr = getProtData(selected_form, mng_ptr);
////                            selected_prot_data_vec.push_back(protdataptr);
////                        }
//
//                        std::priority_queue<std::pair<int, LCSFilter::SearchProtCand>, std::vector<std::pair<int, LCSFilter::SearchProtCand>>, LCSFilter::search_cand_cmp> cand_q;
//                        //for (int cp = 0; cp < selected_prsm_ptrs.size(); cp++) {
//                        for  (int j = 0; j < raw_forms.size(); j ++){
//
////
////                          //if (raw_forms[j]->getSeqName() == seqs_name[spec_set_ptr->getSpectrumId()]) {
////                              mng_ptr->proteo_name = selected_prsm_ptrs[cp]->getSeqName(); ;
////                              int j;
////                              for (int j_i = 0; j_i < raw_forms.size(); j_i ++){
////                                  if(mng_ptr->proteo_name == raw_forms[j_i]->getSeqName()){
////                                      j = j_i;
////                                  }
////                              }
//                            //s = clock();
//                            //int score;
//
//                            mng_ptr->proteo_name = raw_forms[j]->getSeqName();
//                            if(raw_forms[j]->getSeqName() == "sp|P76347|YEEJ_ECOLI") {
//                                int score = filter_ptr->Compute(peak_vec, raw_forms[j], sp_dist_vec, prot_data_vec[0], j);
//                                //int score = filter_ptr->Compute(peak_vec, raw_forms[j], pair_list, prot_data_vec[0], j);
//
//                            if(score > 0) {
//                                if (cand_q.size() < mng_ptr->search_cand_num) {
//                                    LCSFilter::SearchProtCand prot_cand = LCSFilter::SearchProtCand(raw_forms[j],
//                                                                                                    filter_ptr->best_ss_path,
//                                                                                                    filter_ptr->best_ss_dp,
//                                                                                                    prot_data_vec[j]->prot_peak_mass_list_);
//                                    cand_q.push(std::make_pair(score, prot_cand));
//                                } else {
//                                    if (score > cand_q.top().first) {
//                                        cand_q.pop();
//                                        LCSFilter::SearchProtCand prot_cand = LCSFilter::SearchProtCand(raw_forms[j],
//                                                                                                        filter_ptr->best_ss_path,
//                                                                                                        filter_ptr->best_ss_dp,
//                                                                                                        prot_data_vec[j]->prot_peak_mass_list_);
//                                        cand_q.push(std::make_pair(score, prot_cand));
//                                    }
//                                }
//                            }
//
//                            outputProgressBar(float(j) / float(raw_forms.size()));
//                            //e = clock();
////                          double perc = j * 100.0 / proteo_num;
////                          std::cout << "No. " << j << " protein has been proceeded with time: " << double(e - s) / CLOCKS_PER_SEC
////                                    << "s, processing " << perc << "%" << std::endl;
//                            }
//
//                        }
//
//                        sp_e = clock();
//                        std::cout << std::endl;
//                        std::cout << "sp: " << mng_ptr->sp_id << "proceeded using: "<< double(sp_e - sp_s)/CLOCKS_PER_SEC << "s"<<std::endl;
//                        std::ofstream outFile;
//                        std::string result_file = mng_ptr->resultpath + "LCSA_scoreboard.txt";
//                        std::string alignmen_size_rst = mng_ptr->resultpath + "LCSA_alignmentsize.txt";
////                                                                      "(K="
////                                                + std::to_string(mng_ptr->K)
////                                                + ",maxmods=" + std::to_string(mng_ptr->max_total_mods_num) + ").txt";
//
//                        double best_refinescore = 0;
//                        int max_alignmentsize = 0;
//                        std::vector<ProteoformPtr> max_align_prot_list;
//                        ProteoformPtr max_align_prot;
//                        ProteoformPtr best_protein;
//                        while(!cand_q.empty()){
//                            LCSFilter::SearchProtCand topcand = cand_q.top().second;
//
//                            double refine_score = filter_ptr->getRefinedScore_max(topcand, peak_vec, probsForOffsetN, probsForOffsetC);
//                            refine_score = refine_score + cand_q.top().first;
//                            if(refine_score > best_refinescore){
//                                best_refinescore = refine_score;
//                                best_protein = topcand.prot_;
//                            }
//                            if(cand_q.top().first > max_alignmentsize){
//                                max_alignmentsize = cand_q.top().first;
//                                max_align_prot_list.clear();
//                                max_align_prot_list.push_back(topcand.prot_);
//                            }else{
//                                if(cand_q.top().first == max_alignmentsize){
//                                    max_align_prot_list.push_back(topcand.prot_);
//                                }
//                            }
//
//
//
//                            outFile.open(result_file, std::ios::app);
//                            outFile << std::to_string(spec_id) + "\t" + topcand.prot_->getSeqName() + "\t" +
//                                       std::to_string(cand_q.top().first) + "\t" + std::to_string(refine_score)<< std::endl;
//                            outFile.close();
//
//
//                            cand_q.pop();
//                        }
//                        outFile.open(alignmen_size_rst, std::ios::app);
//                        outFile << std::to_string(spec_id) + "\t" + std::to_string(max_alignmentsize) + "\t";
//                        for(int p = 0 ; p< max_align_prot_list.size(); p ++){
//                            outFile << max_align_prot_list[p]->getSeqName() << " ";
//                        }
//                        outFile<<std::endl;
//                        outFile.close();
//
//                        outFile.open(mng_ptr->resultpath + "LCSA_refinescoreboard.txt", std::ios::app);
//                        outFile << std::to_string(spec_id) + "\t" + best_protein->getSeqName() + "\t" +
//                                   std::to_string(best_refinescore) << std::endl;
//                        outFile.close();
//                        //}
//                    }
//                }
//            }
//            deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();
//        }
//        e = clock();
//        std::cout<<"Spectra Processed, LCS-Alignment Finished "<< double(e - s)/CLOCKS_PER_SEC << "s" << std::endl;
//        std::ofstream outFile;
//        std::string result_file = mng_ptr->resultpath + "LCSA_refinescoreboard.txt";
////          +"(K="
////          + std::to_string(mng_ptr->K)
////          + ",maxmods=" + std::to_string(mng_ptr->max_total_mods_num) + ").txt";
//
//        outFile.open(result_file, std::ios::app);
//        outFile <<"Spectra Processed, LCS-Alignment Finished "<< double(e - s)/CLOCKS_PER_SEC << "s" << std::endl ;
//        outFile.close();
//    }


std::function<void()> geneTask(int block_idx, 
                               const std::vector<double> &mod_mass_list, 
                               LCSFilterMngPtr mng_ptr) {
  return[block_idx, mod_mass_list, mng_ptr] () {
    PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
    std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
    std::string db_block_file_name = prsm_para_ptr->getSearchDbFileNameWithFolder()
      + "_" + str_util::toString(block_idx);
    ProteoformPtrVec raw_forms
        = proteoform_factory::readFastaToProteoformPtrVec(db_block_file_name,
                                                          prsm_para_ptr->getFixModPtrVec(),
                                                          false);
    filterBlock(raw_forms, block_idx, mng_ptr, mod_mass_list);
  };
}

void LCSAlignmentProcessor::process() {


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
  std::cout << "Alignment Finished" << std::endl;
}


}  // namespace toppic
