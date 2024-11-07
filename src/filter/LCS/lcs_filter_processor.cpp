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
#include "filter/LCS/lcs_filter_processor.hpp"
#include <fstream>
#include "ms/factory/prm_ms_util.hpp"

#include <time.h>

namespace toppic {

    static int min_res_mass = 15643;
    static int max_res_mass = 51048;

    static bool cmpbin(LCSFilter::BucketPtr x,LCSFilter::BucketPtr y){
        return x->self_score > y->self_score;
    }

    static void outputProgressBar(float progress) {
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

    static std::vector<LCSFilter::SpPairPtr> getSpPairs(PrmPeakPtrVec & peak_vec, std::vector<int> & bgn_ys, int k){
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

    static std::vector<LCSFilter::SpPairPtr> getSpPairs(LCSFilter::PrmPeakSpecPtr & prm_spec, int k){
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

    static LCSFilter::ProtDataPtr getProtData(ProteoformPtr & proteo_ptr, LCSFilterMngPtr mng_ptr){
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
        mass_table.reserve(proteo_ptr->getLen() * k * 0.5);
        mass_pos_table.reserve(proteo_ptr->getLen() * k);
        for(int i = 0; i < proteo_ptr->getLen(); i++){
            int tmp_mass = 0;
            int res_num = 1;
            while(i + res_num - 1 < proteo_ptr->getLen() && res_num <= k) {
                tmp_mass = tmp_mass + round(seq->getResiduePtr(i + res_num - 1)->getMass() * mng_ptr->convert_ratio);
                int int_mass = tmp_mass;
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


    LCSFilter::ProtDataPtr getProtData_1(ProteoformPtr & proteo_ptr, LCSFilterMngPtr & mng_ptr){
        int k = mng_ptr->K;
        int max_mass = 0;
        int min_mass = 100000;
        std::vector<int> prot_peak_mass_list;
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();
        std::vector<std::pair<int,int>> mods_pos_list;
        int res_int_total_mass = 0;
        double res_double_total_mass = 0;
        std::set<Dist> dist_set;
        prot_peak_mass_list.push_back(0);
        for(int i = 0; i < proteo_ptr->getLen(); i++){
            res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
            res_int_total_mass = round(res_double_total_mass * mng_ptr->convert_ratio);
            prot_peak_mass_list.push_back(res_int_total_mass);
            for(int m = 0; m < mng_ptr->MODS_VEC.size(); m++){
                if(seq->getResiduePtr(i)->getAminoAcidPtr()->getOneLetter() == mng_ptr->MODS_VEC[m]){
                    mods_pos_list.push_back(std::make_pair(i,m));
                }
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
        return protdataptr;
    }

    LCSFilter::ProtDataPtr getProtData_noK(ProteoformPtr & proteo_ptr, LCSFilterMngPtr & mng_ptr){
        int k = mng_ptr->K;
        int max_mass = 0;
        int min_mass = 100000;
        std::vector<int> prot_peak_mass_list;
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();
        std::vector<std::pair<int,int>> mods_pos_list;
        int res_int_total_mass = 0;
        double res_double_total_mass = 0;
        std::set<Dist> dist_set;
        prot_peak_mass_list.push_back(0);
        for(int i = 0; i < proteo_ptr->getLen(); i++){
            res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
            res_int_total_mass = round(res_double_total_mass * mng_ptr->convert_ratio);
            prot_peak_mass_list.push_back(res_int_total_mass);
            for(int m = 0; m < mng_ptr->MODS_VEC.size(); m++){
                if(seq->getResiduePtr(i)->getAminoAcidPtr()->getOneLetter() == mng_ptr->MODS_VEC[m]){
                    mods_pos_list.push_back(std::make_pair(i,m));
                }
            }
        }

        DistVec dist_vec;
        LCSFilter::ProtDataPtr protdataptr = std::make_shared<LCSFilter::ProtData>(dist_vec, mods_pos_list, prot_peak_mass_list, max_mass + 27);
        return protdataptr;
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


//        inline void filterBlock(ProteoformPtrVec & raw_forms,
//                        int block_idx,
//                        LCSFilterMngPtr mng_ptr,
//                        const std::vector<double> & mod_mass_list) {
//  std::string block_str = str_util::toString(block_idx);
//  LCSFilterPtr filter_ptr = std::make_shared<LCSFilter>(raw_forms, mng_ptr, block_str);
//
//    struct cand_cmp
//    {
//        bool operator() (const std::pair<int,int> & p1,const std::pair<int,int> & p2)
//        {
//            //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
//            return p1.second > p2.second;
//        }
//    };
//  PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
//  SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
//  std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
//  int group_spec_num = mng_ptr->prsm_para_ptr_->getGroupSpecNum();
//
//  SimpleMsAlignReaderPtr reader_ptr = std::make_shared<SimpleMsAlignReader>(sp_file_name,
//                                                                            group_spec_num,
//                                                                            sp_para_ptr->getActivationPtr());
//
//  // init writer
//  std::string output_file_name = file_util::basename(prsm_para_ptr->getSpectrumFileName())
//      + "." + mng_ptr->output_file_ext_+"_"+ block_str;
//
//  SimplePrsmXmlWriterPtr writer_ptr = std::make_shared<SimplePrsmXmlWriter>(output_file_name);
//
//  DeconvMsPtrVec deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();
//  std::vector<double> prec_error_vec = sp_para_ptr->getMultiShiftSearchPrecErrorVec();
//
//
//    std::vector<int> mods_mass_list;
//    std::vector<int> mods_num_list;
//  for(auto it = mng_ptr->mod_mass_table_.begin(); it != mng_ptr->mod_mass_table_.end(); it++){
//        mods_mass_list.push_back(it->first);
//        int mods_num = 0;
//        for(int mn = 0; mn < mng_ptr->MODS_VEC.size(); mn++){
//            mods_num = mods_num + it->second[mn];
//        }
//        mods_num_list.push_back(mods_num);
//    }
//  mng_ptr->mods_mass_list_ = mods_mass_list;
//    mng_ptr->mods_num_list_ = mods_num_list;
//
//  int proteo_num = raw_forms.size();
//  clock_t total_start,total_end;
//  clock_t total, prot_s, prot_e;
//  clock_t s,e;
//  total_start = clock();
//
//  prot_s = clock();
//  std::vector<LCSFilter::ProtDataPtr> prot_data_vec;
//  for(int i = 0; i < raw_forms.size(); i ++){
//      LCSFilter::ProtDataPtr protdataptr = getProtData(raw_forms[i], mng_ptr);
//      prot_data_vec.push_back(protdataptr);
//  }
//  prot_e = clock();
//  std::cout<<"prot proceeded in: "<<double(prot_e - prot_s)/CLOCKS_PER_SEC<<std::endl;
//
//  std::vector<LCSFilter::PrmPeakSpecPtr> prm_spec_vec;
//  s = clock();
//
//  while (deconv_ms_ptr_vec.size() != 0) {
//      // allow one dalton error
//      SpectrumSetPtrVec spec_set_vec
//              = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(deconv_ms_ptr_vec,
//                                                                         sp_para_ptr, prec_error_vec);
//
//      for (size_t k = 0; k < spec_set_vec.size(); k++) {
//          int peak_num;
//          SpectrumSetPtr spec_set_ptr = spec_set_vec[k];
//          //std::cout<<"PrecMonoMass:"<<spec_set_ptr->getPrecMonoMass()<<std::endl;
//          if (spec_set_ptr->isValid()) {
//              if (mng_ptr->var_num_ == 0) {
//                  PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
//                  SimplePrsmPtrVec match_ptrs = filter_ptr->getBestMatch(ms_ptr_vec);
//                  writer_ptr->write(match_ptrs);
//              } else {
//                  DeconvMsPtrVec deconv_ms_ptr_vec = spec_set_ptr->getDeconvMsPtrVec();
//                  double prec_mono_mass = spec_set_ptr->getPrecMonoMass();
//                  std::vector<double> mod_mass(0);
//                  //std::cout<<"ms peak num: "<<deconv_ms_ptr_vec[0]->size()<<std::endl;
////         PrmMsPtrVec ms_ptr_vec = prm_ms_factory::geneMsSixPtrVec(deconv_ms_ptr_vec,
////                                                                      sp_para_ptr,
////                                                                      prec_mono_mass);
//                  PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
//                  mng_ptr->sp_id = spec_set_ptr->getSpectrumId();
//                  //if (mng_ptr->sp_id == 0) {
//
//                      PrmPeakPtrVec peak_vec = prm_ms_util::getPrmPeakPtrs(ms_ptr_vec,
//                                                                           sp_para_ptr->getPeakTolerancePtr());
//
//                  std::vector<int> bgn_ys;
//                  for(int x = 0; x < peak_vec.size() - 1; x++){
//                      int y = x;
//                      while(y < peak_vec.size()){
//                          int mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
//                          if(mass < min_res_mass) {
//                              y++;
//                          }else{
//                              break;
//                          }
//                      }
//                      bgn_ys.push_back(y);
//                  }
//
//                  clock_t sp_s, sp_e;
//                  sp_s = clock();
//
//                  std::vector<LCSFilter::SpPairPtr> pair_list = getSpPairs(peak_vec, bgn_ys, mng_ptr->K);
//
//
//                      int max_score = 0;
//                      ResSeqPtr best_seq;
//                  std::vector<LCSFilter::BucketPtr> best_path;
//                  std::vector<std::vector<LCSFilter::ModsFound>> best_dp;
//                  std::vector<int> best_seq_peak_mass_list;
//                  std::string best_proteo_name;
//
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
//                      ProteoformPtr best_proteo;
//                      std::cout << "sp: " << mng_ptr->sp_id << " alignment processing" << std::endl;
//                      for (int j = 0; j < proteo_num; j++) {
//                          //if (raw_forms[j]->getSeqName() == seqs_name[spec_set_ptr->getSpectrumId()]) {
//                              mng_ptr->proteo_name = raw_forms[j]->getSeqName();
//                              //s = clock();
//                              int score = filter_ptr->Compute(peak_vec, raw_forms[j], pair_list, prot_data_vec[j], j);
//                              if (max_score <= score) {
//                                  max_score = score;
//                                  best_proteo = raw_forms[j];
//                                  best_path = filter_ptr->best_ss_path;
//                                  best_dp = filter_ptr->best_ss_dp;
//                                  best_seq = raw_forms[j]->getResSeqPtr();
//                                  best_proteo_name = raw_forms[j]->getSeqName();
//                                  best_seq_peak_mass_list = mng_ptr->seq_peak_mass_list;
//                              }
//                              outputProgressBar(float(j) / float(proteo_num));
//                              //e = clock();
////                          double perc = j * 100.0 / proteo_num;
////                          std::cout << "No. " << j << " protein has been proceeded with time: " << double(e - s) / CLOCKS_PER_SEC
////                                    << "s, processing " << perc << "%" << std::endl;
//                         //}
//                      }
//                      if(max_score != 0) {
//                          mng_ptr->proteo_name = best_proteo_name;
//                          mng_ptr->seq_peak_mass_list = best_seq_peak_mass_list;
//                          filter_ptr->backtrack_1(best_path, best_dp, best_seq);
//                      }
//                      sp_e = clock();
//                      std::cout << std::endl;
//                      std::cout << "sp: " << mng_ptr->sp_id << "proceeded using: "<< double(sp_e - sp_s)/CLOCKS_PER_SEC << "s"<<std::endl;
//                      std::ofstream outFile;
//                      std::string result_file = mng_ptr->resultpath + "LCSA_scoreboard.txt";
////                                                                      "(K="
////                                                + std::to_string(mng_ptr->K)
////                                                + ",maxmods=" + std::to_string(mng_ptr->max_total_mods_num) + ").txt";
//
//                      outFile.open(result_file, std::ios::app);
//                      outFile << std::to_string(mng_ptr->sp_id) + "\t" + best_proteo->getSeqName() + "\t" +
//                                 std::to_string(max_score) << std::endl;
//                      outFile.close();
//                  //}
//              }
//          }
//      }
//      deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();
//  }
//  e = clock();
//  std::cout<<"Spectra Processed, LCS-Alignment Finished "<< double(e - s)/CLOCKS_PER_SEC << "s" << std::endl;
//  std::ofstream outFile;
//  std::string result_file = mng_ptr->resultpath + "LCSA_scoreboard.txt";
////          +"(K="
////          + std::to_string(mng_ptr->K)
////          + ",maxmods=" + std::to_string(mng_ptr->max_total_mods_num) + ").txt";
//
//  outFile.open(result_file, std::ios::app);
//  outFile <<"Spectra Processed, LCS-Alignment Finished "<< double(e - s)/CLOCKS_PER_SEC << "s" << std::endl ;
//  outFile.close();
//}

    bool cand_v_cmp(std::pair<ProteoformPtr, int> & x,std::pair<ProteoformPtr, int> & y){

        return x.second > y.second;
    }


    static int Lowerbound(PrmPeakPtrVec a, float key){
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

static float getLossProb(float x, float max, float min){
        float a = 2 * (max - min);
        float b = max - a;
        float lossprob = a / (1 + exp(- (x - 1)))+ b;
        return lossprob;
    }

   static inline void filterBlock(ProteoformPtrVec & raw_forms,
                            int block_idx,
                            LCSFilterMngPtr mng_ptr,
                            const std::vector<double> & mod_mass_list) {
       std::string block_str = str_util::toString(block_idx);
       LCSFilterPtr filter_ptr = std::make_shared<LCSFilter>(raw_forms, mng_ptr, block_str);

       //srand(time(0));
       srand(17);

       struct cand_cmp {
           bool operator()(const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
               //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
               return p1.second > p2.second;
           }
       };
       PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
       SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
       std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
       int group_spec_num = mng_ptr->prsm_para_ptr_->getGroupSpecNum();

       SimpleMsAlignReaderPtr reader_ptr = std::make_shared<SimpleMsAlignReader>(sp_file_name,
                                                                                 group_spec_num,
                                                                                 sp_para_ptr->getActivationPtr());

       // init writer
       std::string output_file_name = file_util::basename(prsm_para_ptr->getSpectrumFileName())
                                      + "." + mng_ptr->output_file_ext_ + "_" + block_str;

       SimplePrsmXmlWriterPtr writer_ptr = std::make_shared<SimplePrsmXmlWriter>(output_file_name);

       DeconvMsPtrVec deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();


       std::vector<double> prec_error_vec;
       if(mng_ptr->use_adjusted_precmass){
           prec_error_vec = sp_para_ptr->getExtendOffsets();
       }else{
           prec_error_vec = sp_para_ptr->getMultiShiftSearchPrecErrorVec();
       }

       std::vector<int> mods_mass_list;
       std::vector<int> mods_num_list;
       for (auto it = mng_ptr->mod_mass_table_.begin(); it != mng_ptr->mod_mass_table_.end(); it++) {
           mods_mass_list.push_back(it->first);
           int mods_num = 0;
           for (int mn = 0; mn < mng_ptr->MODS_VEC.size(); mn++) {
               mods_num = mods_num + it->second[mn];
           }
           mods_num_list.push_back(mods_num);
       }
       mng_ptr->mods_mass_list_ = mods_mass_list;
       mng_ptr->mods_num_list_ = mods_num_list;

       int proteo_num = raw_forms.size();
       clock_t total_start, total_end;
       clock_t total;
       clock_t s, e;
       total_start = clock();

       std::vector<LCSFilter::PrmPeakSpecPtr> prm_spec_vec;


       while (deconv_ms_ptr_vec.size() != 0) {
           // allow one dalton error
           SpectrumSetPtrVec spec_set_vec
                   = spectrum_set_factory::geneSpectrumSetPtrVecWithPrecError(deconv_ms_ptr_vec,
                                                                              sp_para_ptr, prec_error_vec);

           for (size_t k = 0; k < spec_set_vec.size(); k++) {
               int peak_num;
               SpectrumSetPtr spec_set_ptr = spec_set_vec[k];
               //std::cout<<"PrecMonoMass:"<<spec_set_ptr->getPrecMonoMass()<<std::endl;
               if (spec_set_ptr->isValid()) {
                   if (mng_ptr->var_num_ == 0) {
                       PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
                       SimplePrsmPtrVec match_ptrs = filter_ptr->getBestMatch(ms_ptr_vec);
                       writer_ptr->write(match_ptrs);
                   } else {
                       DeconvMsPtrVec deconv_ms_ptr_vec = spec_set_ptr->getDeconvMsPtrVec();
                       double prec_mono_mass = spec_set_ptr->getPrecMonoMass();
                       std::vector<double> mod_mass(0);
                       //std::cout<<"ms peak num: "<<deconv_ms_ptr_vec[0]->size()<<std::endl;
//         PrmMsPtrVec ms_ptr_vec = prm_ms_factory::geneMsSixPtrVec(deconv_ms_ptr_vec,
//                                                                      sp_para_ptr,
//                                                                      prec_mono_mass);
                       PrmMsPtrVec ms_ptr_vec = spec_set_ptr->getMsTwoPtrVec();
//                  PrmMsPtrVec ms_ptr_vec = prm_ms_factory::geneMsTwoPtrVec(deconv_ms_ptr_vec,
//                                                                           sp_para_ptr,
//                                                                           prec_mono_mass, mod_mass);
                       PrmPeakPtrVec peak_vec = prm_ms_util::getPrmPeakPtrs(ms_ptr_vec,
                                                                            sp_para_ptr->getPeakTolerancePtr());

                       PrmPeakPtrVec ori_peak_vec = peak_vec;
                       int spec_id = ms_ptr_vec[0]->getMsHeaderPtr()->getId();

                       if (peak_vec.size() > 0) {
                           PrmPeakPtrVec peak_vec_1;
                           int neibor_num = 0;
                           peak_vec_1.push_back(peak_vec[0]);
                           for (int i = 1; i < peak_vec.size() - 1; i++) {
                               if (peak_vec[i]->getIntPosition() - peak_vec_1.back()->getIntPosition() <=
                                   mng_ptr->delta) { ;
//                                    float new_pos = 0.5 * (peak_vec[i]->getMonoMass() + peak_vec_1.back()->getMonoMass());
//                                    int new_int_pos = round(new_pos * mng_ptr->convert_ratio);
//                                    peak_vec_1.back()->setMonoMass(new_pos);
//                                    peak_vec_1.back()->setIntPosition(new_int_pos);
                                   neibor_num = neibor_num + 1;
                               } else {
                                   peak_vec_1.push_back(peak_vec[i]);
                               }
                           }
                           peak_vec_1.push_back(peak_vec.back());
                           std::cout << "Neighbor num: " << neibor_num << std::endl;

                           PrmPeakPtrVec peak_vec_2;
                           peak_vec_2.push_back(peak_vec_1[0]);
                           PrmPeakPtrVec window_peak_vec;
                           //window_peak_vec.push_back(peak_vec_1[0]);
                           std::vector<float> ionlist = {-18.015, -17.013, -28.01, -45.023, -46.025, 18.015, 17.013,
                                                         28.01, 45.023, 46.025};
                           PrmPeakPtrVec new_peak_vec;
                           int total_keep = 0;
                           double window_time = 0;
                           for (int i = 1; i < peak_vec_1.size() - 1; i++) {
                               clock_t window_s;
                               int j = i + 1;

                               bool keep = false;
                               while (j < peak_vec_1.size() && (peak_vec_1[j]->getMonoMass() <=
                                                                peak_vec_1[i]->getMonoMass() + ionlist.back() + 0.1)) {
                                   //std::cout<<"j:"<<peak_vec_1[j]->getMonoMass()<<", i:"<< peak_vec_1[i]->getMonoMass()<<", j-i:"<<peak_vec_1[j]->getMonoMass() - peak_vec_1[i]->getMonoMass()<<std::endl;
                                   for (int ioni = ionlist.size() - 1; ionlist[ioni] > 0; ioni--) {
                                       //std::cout<<ioni<<", ion:"<<ionlist[ioni]<<std::endl;
                                       if (abs(peak_vec_1[j]->getMonoMass() - peak_vec_1[i]->getMonoMass() -
                                               ionlist[ioni]) <= 0.1) {
                                           keep = true;
//                                            std::cout<<"j = "<<j<<": "<<peak_vec_1[j]->getMonoMass()<<", i = "<<i<<": " <<peak_vec_1[i]->getMonoMass()<<", j-i:"<<peak_vec_1[j]->getMonoMass() - peak_vec_1[i]->getMonoMass()<<std::endl;
//                                            std::cout<<ioni<<", ion:"<<ionlist[ioni]<<std::endl;
                                           break;
                                       }
                                   }
                                   if (keep) {
                                       break;
                                   } else {
                                       j = j + 1;
                                   }
                               }

                               if (!keep) {
                                   j = i - 1;
                                   while (j >= 0 && (peak_vec_1[j]->getMonoMass() >=
                                                     peak_vec_1[i]->getMonoMass() + ionlist[4] - 0.1)) {
                                       //std::cout<<"j:"<<peak_vec_1[j]->getMonoMass()<<", i:"<< peak_vec_1[i]->getMonoMass()<<", j-i:"<<peak_vec_1[j]->getMonoMass() - peak_vec_1[i]->getMonoMass()<<std::endl;
                                       for (int ioni = 0; ionlist[ioni] < 0; ioni++) {
                                           //std::cout<<ioni<<", ion:"<<ionlist[ioni]<<std::endl;
                                           if (abs((peak_vec_1[j]->getMonoMass() - peak_vec_1[i]->getMonoMass()) -
                                                   ionlist[ioni]) <= 0.1) {
                                               keep = true;
                                               break;
                                           }
                                       }
                                       if (keep) {
                                           break;
                                       } else {
                                           j = j - 1;
                                       }
                                   }
                               }
                               clock_t window_e;
                               window_time = window_time + double(window_e - window_s) / CLOCKS_PER_SEC;
                               if (keep) {
                                   peak_vec_2.push_back(peak_vec_1[i]);
                                   window_peak_vec.push_back(peak_vec_1[i]);
                                   total_keep = total_keep + 1;
                               } else {
                                   float prob = rand() % 100 / double(100);
                                   if (prob >= float(mng_ptr->filter_drop_prob)) {
                                       peak_vec_2.push_back(peak_vec_1[i]);
                                       //peak_vec_1.back()->setPeakId(peak_vec_1.size() - 1);
                                   }
                               }
                           }


                       }

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

                       std::vector<int> ori_bgn_ys;
                       for (int x = 0; x < ori_peak_vec.size() - 1; x++) {
                           int y = x;
                           while (y < ori_peak_vec.size()) {
                               int mass = ori_peak_vec[y]->getIntPosition() - ori_peak_vec[x]->getIntPosition();
                               if (mass < min_res_mass) {
                                   y++;
                               } else {
                                   break;
                               }
                           }
                           ori_bgn_ys.push_back(y);
                       }


                       LCSFilter::PrmPeakSpecPtr prm_spec = std::make_shared<LCSFilter::PrmPeakSpec>(
                               ms_ptr_vec[0]->getMsHeaderPtr(),
                               peak_vec,
                               mng_ptr->filter_result_num_,
                               ms_ptr_vec.size());
                       prm_spec->ori_peak_vec_ = ori_peak_vec;
                       prm_spec->ori_bgn_ys_ = ori_bgn_ys;
                       prm_spec->spec_id_ = spec_id;
                       prm_spec->offset_ = mng_ptr->prec_error_vec[k];
                       prm_spec->spec_name_ = std::to_string(spec_id) + '.' + std::to_string(prm_spec->offset_);
                       prm_spec->setBgn_ys(bgn_ys);
                       prm_spec_vec.push_back(prm_spec);


                   }
               }
           }
           deconv_ms_ptr_vec = reader_ptr->getNextMsPtrVec();
       }
       e = clock();
       std::cout << "Spectra Initialized " << double(e - s) / CLOCKS_PER_SEC << "s" << std::endl;


       clock_t lcs_s, lcs_e;
//        lcs_s = clock();
//        std::vector<LCSFilter::ProtDataPtr> prot_data_vec;
//        for(int i = 0; i < raw_forms.size(); i ++){
//            LCSFilter::ProtDataPtr protdataptr = getProtData_1(raw_forms[i], mng_ptr);
//            prot_data_vec.push_back(protdataptr);
//        }
//        lcs_e = clock();
//        std::cout<<"all prot got tables: "<< double(lcs_e - lcs_s)/CLOCKS_PER_SEC<<"s"<<std::endl;
//        std::cout<<proteo_num<<std::endl;
//        std::cout<<prot_data_vec.size()<<std::endl;


       clock_t prot_s = clock();
       std::vector<std::vector<int>> node_mass_list_vec;
       for (int j = 0; j < raw_forms.size(); j++) {
           ResSeqPtr seq = raw_forms[j]->getResSeqPtr();
           std::vector<int> node_mass_list;
           int res_int_total_mass = 0;
           double res_double_total_mass = 0;
           std::set<Dist> dist_set;
           node_mass_list.push_back(0);
           for (int i = 0; i < raw_forms[j]->getLen(); i++) {
               res_double_total_mass = res_double_total_mass + seq->getResiduePtr(i)->getMass();
//               if(seq->getResiduePtr(i)->toString() == "C"){
//                   std::cout<<"C: "<<seq->getResiduePtr(i)->getMass()<<std::endl;
//               }
               res_int_total_mass = round(res_double_total_mass * mng_ptr->convert_ratio);
               node_mass_list.push_back(res_int_total_mass);
           }
           node_mass_list_vec.push_back(node_mass_list);
       }
       clock_t prot_e = clock();
       std::cout << "prot proceeded in: " << double(prot_e - prot_s) / CLOCKS_PER_SEC << std::endl;


       lcs_s = clock();

       std::vector<int> mm_list;
       for (int i = 0; i < mng_ptr->T_mass_comb_.size(); i++) {
           mm_list.push_back(get<0>(mng_ptr->T_mass_comb_[i]));
       }
       mm_list.push_back(0);
       mm_list.push_back(mng_ptr->delta);
       std::sort(mm_list.begin(), mm_list.end());
       int mm_min = mm_list[0];
       int mm_max = mm_list.back();


       clock_t mass_filter_s, mass_filter_e;
       mass_filter_s = clock();
       for (int i = 0; i < prm_spec_vec.size(); i++) {
           if(prm_spec_vec[i]->spec_id_ != -1) {
           s = clock();
           std::vector<std::vector<std::pair<int, int>>> filtered_seg;
           std::vector<std::vector<std::vector<int>>> bgn_node_lists;
           std::vector<std::vector<std::vector<int>>> end_node_lists;
           int prec_mass_minus_water = round(
                   prm_spec_vec[i]->getHeader()->getPrecMonoMassMinusWater() * mng_ptr->convert_ratio);
           int prec_adjust_tole = round(prec_mass_minus_water * sp_para_ptr->getPeakTolerancePtr()->getPpo());
           int fixed_tole = mng_ptr->delta;
           int prec_tole;
           if(mng_ptr->mass_filter_use_fixed_tole){
               prec_tole = fixed_tole;
           }else{
               prec_tole = prec_adjust_tole;
           }

           for (int j = 0; j < proteo_num; j++) {
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
                           y++;
                       }
                   } else {
                       break;
                   }
                   if (end_pos != -1) {
                       seg_list.push_back(std::make_pair(x, end_pos));
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
                           if(merge_seg_list.back().second < seg_list[k].second) {
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
               end_node_lists.push_back(end_node_list);
           }


           PrmPeakPtrVec peak_vec = prm_spec_vec[i]->getPeakVec();
           std::vector<int> bgn_ys = prm_spec_vec[i]->getBgn_ys();
           DistVec sp_dist_vec = getSpPairs_1(peak_vec, bgn_ys, mng_ptr->K);
           for (int j = 0; j < proteo_num; j++) {

               if (!filtered_seg[j].empty()){
               //&& (raw_forms[j]->getSeqName() == "sp|P0A6W9|GSH1_ECOLI")) {
                   for (int seg_i = 0; seg_i < filtered_seg[j].size(); seg_i++) {
                       int start_pos = filtered_seg[j][seg_i].first;
                       int end_pos = filtered_seg[j][seg_i].second - 1;
                       std::vector<int> bgn_nodes = bgn_node_lists[j][seg_i];
                       std::vector<int> end_nodes = end_node_lists[j][seg_i];
                       for (int b = 0; b < bgn_nodes.size(); b++) {
                           bgn_nodes[b] = bgn_nodes[b] - start_pos;
                           end_nodes[b] = end_nodes[b] - start_pos - 1;
                       }
                       ProteoformPtr sub_proteo_ptr = proteoform_factory::geneSubProteoform(raw_forms[j],
                                                                                            raw_forms[j]->getFastaSeqPtr(),
                                                                                            start_pos,
                                                                                            end_pos);
                       LCSFilter::ProtDataPtr prot_data = getProtData_1(sub_proteo_ptr, mng_ptr);
                       //if (raw_forms[j]->getSeqName() == seqs_name[spec_set_ptr->getSpectrumId()]) {
                       filter_ptr->Filtering(prm_spec_vec[i], sub_proteo_ptr, sp_dist_vec, prot_data, bgn_nodes, end_nodes,j);

                       outputProgressBar(float(j) / float(raw_forms.size()));

                   }
               }
           }

//               SimplePrsmPtrVec match_ptrs;


           std::priority_queue<std::tuple<ProteoformPtr, int, int, int>, std::vector<std::tuple<
                   ProteoformPtr, int, int, int >>,
                   LCSFilter::cand_cmp_2> cand_q = prm_spec_vec[i]->getCandQ();
           prm_spec_vec[i]->cand_proteo_vec_.reserve(mng_ptr->filter_result_num_);
           while(!cand_q.empty()) {
               ProteoformPtr proteo_ptr = get<0>(cand_q.top());
               std::string seq_name = proteo_ptr->getSeqName();
               int score = get<3>(cand_q.top());
               ProteoformPtr sub_proteo_ptr = proteoform_factory::geneSubProteoform(proteo_ptr,
                                                                                    proteo_ptr->getFastaSeqPtr(),
                                                                                    get<1>(cand_q.top()),
                                                                                    get<2>(cand_q.top()));
//                   match_ptrs.push_back(std::make_shared<SimplePrsm>(prm_spec_vec[i]->getHeader(),
//                                                                     prm_spec_vec[i]->getSpNum(),
//                                                                     raw_forms[id], score));
                prm_spec_vec[i]->InsertCandtoVec(sub_proteo_ptr);
               std::ofstream outFile;
               std::string result_file = mng_ptr->resultpath + "LCSA_filter_results.txt";
//                                                                      "(K="
//                                                + std::to_string(mng_ptr->K)
//                                                + ",maxmods=" + std::to_string(mng_ptr->max_total_mods_num) + ").txt";

               outFile.open(result_file, std::ios::app);
               outFile << std::to_string(prm_spec_vec[i]->spec_id_) + "\t" + seq_name + "\t" +
                          std::to_string(sub_proteo_ptr->getStartPos()) + "\t" + std::to_string(sub_proteo_ptr->getEndPos())<< "\t" << score << std::endl;
               outFile.close();

               cand_q.pop();

           }
           e = clock();
           std::cout << "No. " << i << " spec has been filtered with time: " << double(e - s) / CLOCKS_PER_SEC
                     << "s" << std::endl;
          }
       }


       lcs_e = clock();
       std::cout << "all spectra filtered " << double(lcs_e - lcs_s) / CLOCKS_PER_SEC << "s" << std::endl;


       total_end = clock();
       std::cout << " total filter time: " << double(total_end - total_start) / CLOCKS_PER_SEC << "s" << std::endl;
       writer_ptr->close();

       //sTopMG
       clock_t sp_s, sp_e, stopmg_s, stopmg_e;
       stopmg_s = clock();

       for (int i = 0; i < prm_spec_vec.size(); i++) {
           if(prm_spec_vec[i]->spec_id_ != -1) {

               sp_s = clock();
               PrmPeakPtrVec peak_vec = prm_spec_vec[i]->ori_peak_vec_;
               std::cout<<"number of peak: "<< peak_vec.size()<<std::endl;



               int max_score = 0;
               int max_score_shifting = 0;
               ResSeqPtr best_seq;
               int best_start_pos;
               std::vector<LCSFilter::BucketPtr> best_path;
               std::vector<std::vector<LCSFilter::ModsFound>> best_dp;
               std::vector<int> best_seq_peak_mass_list;
               std::string best_proteo_name;
               ProteoformPtr best_proteo;
               std::cout << "sp: " << prm_spec_vec[i]->spec_id_ << " alignment processing" << std::endl;
               std::vector<ProteoformPtr> cand_prots = prm_spec_vec[i]->cand_proteo_vec_;
               if(cand_prots.empty()){
                   std::ofstream outFile;
                   std::string result_file = mng_ptr->resultpath + "LCSA_scoreboard.txt";
                   outFile.open(result_file, std::ios::app);
                   outFile << std::to_string(prm_spec_vec[i]->spec_id_) + "\t" + "-"+ "\t" +
                              "-"+ "\t" +
                              std::to_string(0) << std::endl;
                   outFile.close();
               }else {
                   int score = 0;
                   int shifting = 0;
                   if (mng_ptr->alignment_K > 0) {
                       std::vector<int> bgn_ys = prm_spec_vec[i]->ori_bgn_ys_;
                       clock_t step2_s, step2_e;
                       step2_s = clock();
                       DistVec sp_dist_vec = getSpPairs_1(peak_vec, bgn_ys, mng_ptr->K);
                       step2_e = clock();
                       //std::cout<<"step2 time: "<<std::fixed<<std::setprecision(4)<<double(step2_e - step2_s)/CLOCKS_PER_SEC<<std::endl;
                       for (int j = 0; j < cand_prots.size(); j++) {
                           clock_t cand_seg_s, cand_seg_e;
                           cand_seg_s = clock();

                           ProteoformPtr sub_proteo_ptr = cand_prots[j];
                           clock_t step1_s, step1_e;
                           step1_s = clock();
                           LCSFilter::ProtDataPtr prot_data = getProtData_1(sub_proteo_ptr, mng_ptr);
                           step1_e = clock();
                           //std::cout<<"step1 time: "<<std::fixed<<std::setprecision(4)<<double(step1_e - step1_s)/CLOCKS_PER_SEC<<std::endl;
                           std::vector<int> bgn_nodes = {0};
                           score = filter_ptr->Compute(peak_vec, sub_proteo_ptr, sp_dist_vec, prot_data, bgn_nodes);
                           if(score > 0) {
                           ResSeqPtr seq = sub_proteo_ptr->getResSeqPtr();
//                           shifting = filter_ptr->getTotalShifting(filter_ptr->best_ss_path, filter_ptr->best_ss_dp,
//                                                                   seq, prot_data, peak_vec);

                           cand_seg_e = clock();
                           //std::cout<<"J: "<<j<<", score: "<<score<<", seq: "<< sub_proteo_ptr->getSeqName()<<", start: "<< sub_proteo_ptr->getStartPos()<<std::endl;
//                       if(cand_prots[j]->getSeqName() == "sp|P31806|NNR_ECOLI"){
//                           std::cout<<"score: "<<score<<" res: "<<cand_prots[j]->getResSeqPtr()->toString()<<std::endl;
//                       }
                           //std::cout<<"total seg time: "<<std::fixed<<std::setprecision(4)<<double(cand_seg_e - cand_seg_s)/CLOCKS_PER_SEC<<std::endl;
                           //std::cout<<"===="<<std::endl;
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

                   if (mng_ptr->alignment_K == -1) {
                       for (int j = 0; j < cand_prots.size(); j++) {

                           clock_t cand_seg_s, cand_seg_e;
                           cand_seg_s = clock();

                           ProteoformPtr sub_proteo_ptr = cand_prots[j];
                           std::vector<int> bgn_nodes = {0};
                           LCSFilter::ProtDataPtr prot_data = getProtData_1(sub_proteo_ptr, mng_ptr);
                           score = filter_ptr->Compute_noK(peak_vec, sub_proteo_ptr, prot_data, bgn_nodes);
                           ResSeqPtr seq = sub_proteo_ptr->getResSeqPtr();
                           if(score > 0) {
//                               shifting = filter_ptr->getTotalShifting(filter_ptr->best_ss_path, filter_ptr->best_ss_dp,
//                                                                       seq, prot_data, peak_vec);
                               cand_seg_e = clock();
                               //std::cout<<"total seg time: "<<std::fixed<<std::setprecision(4)<<double(cand_seg_e - cand_seg_s)/CLOCKS_PER_SEC<<std::endl;
                               //std::cout<<"===="<<std::endl;
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


                   if (max_score > 0) {
                       mng_ptr->proteo_name = best_proteo_name;
                       mng_ptr->seq_peak_mass_list = best_seq_peak_mass_list;
                       mng_ptr->start_pos = best_start_pos;
                       mng_ptr->sp_id = prm_spec_vec[i]->spec_id_;
                       filter_ptr->backtrack_2(best_path, best_dp, best_seq);
                   }
                   sp_e = clock();
//                   std::cout << std::endl;
                   std::cout << "sp: " << prm_spec_vec[i]->spec_id_ << "proceeded using: "
                             << double(sp_e - sp_s) / CLOCKS_PER_SEC << "s"
                             << std::endl;
                   std::ofstream outFile;
                   std::string result_file = mng_ptr->resultpath + "LCSA_scoreboard.txt";


                   outFile.open(result_file, std::ios::app);
                   if(max_score <= 0){
                       outFile << std::to_string(prm_spec_vec[i]->spec_id_) + "\t" + "-"+ "\t" +
                                  "-"+ "\t" +
                                  std::to_string(0) << std::endl;
                   }else{
                       outFile << std::to_string(prm_spec_vec[i]->spec_id_) + "\t" + best_proteo->getSeqName() + "\t" +
//                                  std::to_string(best_start_pos) + "\t" +
                                  std::to_string(prm_spec_vec[i]->offset_) + "\t" +
                                  std::to_string(max_score) << std::endl;
                   }
                   outFile.close();
               }
           }
       }

       stopmg_e = clock();
       std::cout << " exact algorithm time: " << double(stopmg_e - stopmg_s) / CLOCKS_PER_SEC << "s" << std::endl;
       std::cout << "total sTopMG time: " << double(stopmg_e - stopmg_s + total_end - total_start) / CLOCKS_PER_SEC
                 << "s" << std::endl;

   }


static std::function<void()> geneTask(int block_idx,
                               const std::vector<double> &mod_mass_list, 
                               LCSFilterMngPtr mng_ptr) {
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
    filterBlock(raw_forms, block_idx, mng_ptr, mod_mass_list);
  };
}

 void LCSFilterProcessor::process() {
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


}  // namespace toppic
