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
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <boost/thread/mutex.hpp>
#include <queue>
#include <unordered_map>
#include <math.h>
#include <numeric>

#include "common/util/file_util.hpp"
#include "seq/prot_candidate.hpp"
#include "ms/factory/prm_ms_util.hpp"
#include "prsm/simple_prsm_util.hpp"

#include "filter/massmatch/mass_match_factory.hpp"
#include "filter/massmatch/mass_match_util.hpp"
#include "filter/LCS/lcs_filter.hpp"

#include "common/base/amino_acid_base.hpp"
#include "common/base/ptm_base.hpp"
#include "common/base/residue_base.hpp"
#include "common/base/residue_util.hpp"

namespace toppic {

// serialization mutex.
boost::mutex lcs_filter_mutex;

LCSFilter::LCSFilter(const ProteoformPtrVec &proteo_ptrs,
                       LCSFilterMngPtr mng_ptr, std::string block_str) {
  mng_ptr_ = mng_ptr;
  proteo_ptrs_ = proteo_ptrs;
  PrsmParaPtr prsm_para_ptr = mng_ptr->prsm_para_ptr_;
}

SimplePrsmPtrVec LCSFilter::getBestMatch(const PrmMsPtrVec &ms_ptr_vec) {
  SimplePrsmPtrVec match_ptrs = compute(ms_ptr_vec);
  SimplePrsmPtrVec unique_match_ptrs = simple_prsm_util::getUniqueMatches(match_ptrs);
  std::sort(unique_match_ptrs.begin(), unique_match_ptrs.end(), SimplePrsm::cmpScoreDec);
  size_t num = mng_ptr_->filter_result_num_;
  if (num > unique_match_ptrs.size()) {
    num = unique_match_ptrs.size();
  }
  SimplePrsmPtrVec result_ptrs;
  for (size_t i = 0; i < num; i++) {
    SimplePrsmPtr match_ptr = unique_match_ptrs[i];
    if (match_ptr->getScore() > 0.0) {
      result_ptrs.push_back(match_ptr);
    } else {
      break;
    }
  }
  return result_ptrs;
}

SimplePrsmPtrVec LCSFilter::compute(const PrmMsPtrVec &ms_ptr_vec) {
  PeakTolerancePtr tole_ptr = mng_ptr_->prsm_para_ptr_->getSpParaPtr()->getPeakTolerancePtr();
  std::vector<std::pair<int, int> > mass_errors
      = prm_ms_util::getIntMassErrorList(ms_ptr_vec, tole_ptr, mng_ptr_->filter_scale_, true, false);
  SimplePrsmPtrVec match_ptrs;
  int row_num = index_ptr_->getRowNum();
  std::vector<int> proteo_row_begins = index_ptr_->getProteoRowBegins();
  std::vector<int> proteo_row_ends = index_ptr_->getProteoRowEnds();
  int threshold = 4;
  std::vector<short> scores(row_num, 0);
  std::vector<short> max_scores(row_num, 0);
  for (size_t i = 0; i < mass_errors.size(); i++) {
    std::fill(scores.begin(), scores.end(), 0);
    index_ptr_->compScores(mass_errors, i, -mass_errors[i].first, scores);
    for (int j = 0; j < row_num; j++) {
      if (scores[j] > max_scores[j]) {
        max_scores[j] = scores[j];
      }
    }
  }
 
  ProtCandidatePtrVec results
    = mass_match_util::findDiagTopProteins(max_scores, proteo_row_begins, proteo_row_ends, threshold,
                                           mng_ptr_->filter_result_num_);
  for (size_t j = 0; j < results.size(); j++) {
    int id = results[j]->getProteinId();
    int score = results[j]->getScore();
    match_ptrs.push_back(std::make_shared<SimplePrsm>(ms_ptr_vec[0]->getMsHeaderPtr(),
                                                      ms_ptr_vec.size(),
                                                      proteo_ptrs_[id], score));
  }
  return match_ptrs;
}



PrmPeakPtrVec LCSFilter::getSortedMergeList_pos(std::vector<PrmPeakPtrVec>& lists){
    PrmPeakPtrVec merge_list;
    std::priority_queue<LCSFilter::NodePtr, std::vector<NodePtr>, CompareNode_PosAfterMove> q;
    for(int i = 0; i < lists.size(); i++){
        if(lists[i].empty()){
            continue;
        }
        q.push(std::make_shared<Node>(lists[i][0], i, 0));
    }
    while(!q.empty()){
        NodePtr node_ptr = q.top();
        q.pop();
        //PrmPeakPtr prm_peak_ptr = std::make_shared<PrmPeak>(node_ptr->p);
        int arrayIdx = node_ptr->arrayIdx;
        int idx = node_ptr->idx;
        merge_list.push_back(node_ptr->p);
        if(idx == lists[arrayIdx].size() - 1){
            continue;
        }else{
            NodePtr newpoint_ptr = std::make_shared<Node>(lists[arrayIdx][idx + 1], arrayIdx, idx + 1);
            q.push(newpoint_ptr);
        }
    }
    return merge_list;
}



void LCSFilter::Shifting(std::vector<PrmPeakPtrVec> & lists, ProteoformPtr & proteo_ptr) {
     ResSeqPtr res_seq_ptr = proteo_ptr->getResSeqPtr();
     //mng_ptr_->seq_peak_mass_list.clear();
     int list_dist = 0;
     for(int i = 0; i < res_seq_ptr->getLen() + 1; i++){
         PrmPeakPtrVec reslist;
//         if(i!=0){
             list_dist = mng_ptr_->seq_peak_mass_list[i];

//         }
         //mng_ptr_->seq_peak_mass_list.push_back(list_dist);
         //if(i < res_seq_ptr->getLen()) {
             for (int j = 0; j < lists[i].size(); j++) {
                 lists[i][j]->setIntPosAfterMove(lists[i][j]->getIntPosition() - list_dist);
             }
         //}
     }
}

std::unordered_map<int, std::vector<int>> LCSFilter::getSigma_prot(ProteoformPtr & proteo_ptr, int k){
    std::unordered_map<int, std::vector<int>> mass_table;
    std::unordered_map<int, std::vector<std::pair<int, int>>> mass_pos_table;
    std::vector<std::vector<int>> follow_mass_table(proteo_ptr->getLen() + 1);
    int max_mass = 0;
    int min_mass = 100000;
    ResSeqPtr seq = proteo_ptr->getResSeqPtr();
    mass_table.reserve(proteo_ptr->getLen() * k * 0.5);
    mass_pos_table.reserve(proteo_ptr->getLen() * k);
    for(int i = 0; i < proteo_ptr->getLen(); i++){
        int tmp_mass = 0;
        int res_num = 1;
        while(i + res_num - 1 < proteo_ptr->getLen() && res_num <= k) {
            tmp_mass = tmp_mass + round(seq->getResiduePtr(i + res_num - 1)->getMass() * mng_ptr_->convert_ratio);
            int int_mass = tmp_mass;
            int tol_mass = round(int_mass / mng_ptr_->fix_tol_);
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

//    int pos_total = 0;
//    for(auto it = mass_pos_table.begin(); it != mass_pos_table.end(); ++it) {
//
//       pos_total = pos_total + it->second.size();
//    }
//    std::cout<<mass_pos_table.size()<<std::endl;
//    std::cout<<pos_total<<std::endl;

    mng_ptr_->max_mass_ = max_mass;
    mng_ptr_->min_mass_ = min_mass;
    return mass_table;
}

std::vector<int> LCSFilter::FindMassInTable(int mass_dist, std::unordered_map<int, std::vector<int>> & mass_table){
    std::vector<int> match_mass_vec;
    for(int t = -1; t < 2; t++) {
        int dist_tol = round(double(mass_dist) / mng_ptr_->fix_tol_) + t;
        if(mass_table.count(dist_tol)){
            std::vector<int> tmp = mass_table[dist_tol];
            for (int i = 0; i < tmp.size(); i++) {
                if (abs(mass_dist - tmp[i]) <= mng_ptr_->fix_tol_) {
                    match_mass_vec.push_back(tmp[i]);
                }
            }
        }
    }
    return match_mass_vec;
}

std::vector<std::vector<std::pair<int,int>>> LCSFilter::getSigma_spec(PrmPeakPtrVec & peak_vec,
                                                                                  std::unordered_map<int, std::vector<int>> & mass_table){
    int peak_num = peak_vec.size();
    std::vector<std::vector<std::pair<int,int>>> sigma_spec(peak_num);
    for(int x = 0; x < peak_num; x++){
        int y = x;
        int mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
        //mass = 609646;
        while(y < peak_vec.size() && mass <= mng_ptr_->max_mass_){
            if(mass >= mng_ptr_->min_mass_){
                std::vector<int> match_mass_vec = FindMassInTable(mass, mass_table);
                for(int i = 0; i < match_mass_vec.size(); i++){
                    sigma_spec[x].push_back(std::make_pair(y,match_mass_vec[i]));
                }
            }
            y++;
            if(y < peak_vec.size()){
                mass = peak_vec[y]->getIntPosition() - peak_vec[x]->getIntPosition();
            }
        }
    }
    return sigma_spec;
}

std::vector<LCSFilter::massNodePtr> LCSFilter::getMergeFollowMasses(int cur_mass, std::unordered_map<int, std::vector<std::pair<int, int>>> & mass_pos_table,
                                                   std::vector<std::vector<int>> & follow_mass_table){
    std::vector<LCSFilter::massNodePtr> merge_list;
    std::priority_queue<LCSFilter::massNodePtr, std::vector<massNodePtr>, CompareMassNode> q;
    for(int i = 0; i < mass_pos_table[cur_mass].size(); i++){
        int mass_i = mass_pos_table[cur_mass][i].first;
        int mass_j = mass_pos_table[cur_mass][i].second;
        if(follow_mass_table[mass_j].size() == 0){
            continue;
        }
        q.push(std::make_shared<massNode>(follow_mass_table[mass_j][0], mass_i, mass_j, 0));
    }
    while(!q.empty()){
        massNodePtr node_ptr = q.top();
        q.pop();
        int mass_j = node_ptr->mass_j;
        int idx = node_ptr->idx;
        int mass_i = node_ptr->mass_i;

        merge_list.push_back(node_ptr);

        if(idx == follow_mass_table[mass_j].size() - 1){
            continue;
        }else{
            massNodePtr newpoint_ptr = std::make_shared<massNode>(follow_mass_table[mass_j][idx + 1], mass_i, mass_j, idx + 1);
            q.push(newpoint_ptr);
        }
    }
    return merge_list;
}

void countMods(int bgn, int end, std::vector<int> & mods_in_gap, std::string & seq_string,   std::unordered_map<std::string, std::vector<std::string>> & res_mods_table, std::vector<std::string> MODS){
    for(int i = bgn; i < end; i++){
        std::string oneletter = std::to_string(seq_string[i]);
        std::vector<std::string> mods = res_mods_table[oneletter];
        for(int m = 0; m < mods.size(); m ++){
            for(int n =0 ; n < MODS.size(); n++){
                if(mods[m] == MODS[n]){
                    mods_in_gap[n] ++;
                }
            }
        }
    }
}


    void countMods(int bgn, int end, std::vector<int> & mods_in_gap, std::string & seq_string,   std::unordered_map<std::string, std::vector<std::string>> & res_mods_table, std::vector<std::string> MODS, std::vector<int> & c_pos_list, std::string dbond_name){
        for(int i = bgn; i < end; i++){
            std::string oneletter = std::to_string(seq_string[i]);
            std::vector<std::string> mods = res_mods_table[oneletter];
            for(int m = 0; m < mods.size(); m ++){
                if(mods[m] == dbond_name){
                    c_pos_list.push_back(i);
                }
                for(int n =0 ; n < MODS.size(); n++){
                    if(mods[m] == MODS[n]){
                        mods_in_gap[n] ++;
                    }
                }
            }
        }
    }


void countMods(int bgn, int end, std::vector<int> & mods_in_gap, std::string & seq_string, std::vector<std::string> MODS){
    for(int i = bgn; i < end; i++){
        for(int m = 0; m < MODS.size(); m++){
            if(seq_string[i] == MODS[m][0]){
                mods_in_gap[m] ++;
                break;
            }
        }
    }
}

void checkMods(bool & found, std::vector<int> & mods_in_gap, std::vector<int> & mods_vec){
    for(int i = 0; i < mods_in_gap.size(); i ++){
        if(mods_in_gap[i] < mods_vec[i]){
            found = false;
            break;
        }else{
            found = true;
        }
    }
}

    void checkMods(bool & found, std::vector<int> & mods_in_gap, std::vector<int> & mods_vec, int pos_iso_shift_idx, int neg_iso_shift_idx){
        for(int i = 0; i < mods_in_gap.size(); i ++){
            if(i!=pos_iso_shift_idx && i!= neg_iso_shift_idx) {
                if (mods_in_gap[i] < mods_vec[i]) {
                    found = false;
                    break;
                } else {
                    found = true;
                }
            }
        }
    }

    void checkMods(bool & found, std::vector<int> & mods_in_gap, std::vector<int> & mods_vec, int pos_iso_shift_idx, int neg_iso_shift_idx, int dbond_idx, bool dbond, std::vector<int> c_pos_list, int min_Dbond_dist_) {
        bool valid_num;

        for (int i = 0; i < mods_in_gap.size(); i++) {
            if (i != pos_iso_shift_idx && i != neg_iso_shift_idx) {
                if (mods_in_gap[i] < mods_vec[i]) {
                    valid_num = false;
                    break;
                } else {
                    valid_num = true;
                }
            }
        }
        if (valid_num) {
            if (dbond) {
                int valid_dist_num = 0;
                for (int i = 0; i + 1< c_pos_list.size(); i++){
                    int dist = c_pos_list[i + 1] - c_pos_list[i] - 1;
                    if (dist >= min_Dbond_dist_){
                        valid_dist_num = valid_dist_num + 1;
//                        found = true;
//                        break;
                    }
//                    else{
//                        found = false;
//                    }
                }

                if(valid_dist_num >= mods_vec[dbond_idx] / 2){
                    found = true;
                }else{
                    found = false;
                }
            }else{
                found = true;
            }
        }else{
            found = false;
        }
    }



int Lowerbound(std::vector<std::pair<int, int>> & mods_pos_list, int key){
    int left = 0;
    int right = mods_pos_list.size() - 1;
    while(left <= right){
        int mid=(left + right) >> 1;
        if(mods_pos_list[mid].first >= key){
            right = mid-1;
        }
        else{left = mid + 1;}
        }
    return left;
}



    int Lowerbound(std::vector<std::pair<int, std::vector<int>>> & modlist_for_pos, int key){
            int left = 0;
            int right = modlist_for_pos.size() - 1;
            while (left <= right) {
                int mid = (left + right) >> 1;
                if (modlist_for_pos[mid].first >= key) {
                    right = mid - 1;
                } else { left = mid + 1; }
            }
            return left;

    }



int Lowerbound(std::vector<int> & a, int key){
    int left = 0;
    int right = a.size() - 1;
    while(left <= right){
        int mid=(left + right) >> 1;
        if(a[mid] >= key){
            right = mid-1;
        }
        else{left = mid + 1;}
    }
    return left;
}


    int computeLoss(std::vector<int> & pre_seqpeak, std::vector<int> & cur_seqpeak, int mod_left, int mod_right){
        int loss = 0;
        for(int i = 0; i < cur_seqpeak.size(); i++){
            if(cur_seqpeak[i] < mod_right){
                loss = loss + 1;
            }
            else{
                break;
            }
        }
        for(int i = pre_seqpeak.size() - 1; i >= 0; i--){
            if(pre_seqpeak[i] > mod_left){
                loss = loss + 1;
            }
            else{
                break;
            }
        }
        return loss;
    }







int Lowerbound(PrmPeakPtrVec & merge_list, int key){
    int left = 0;
    int right = merge_list.size() - 1;
    while(left <= right){
        int mid=(left + right) >> 1;
        if(merge_list[mid]->getIntPosAfterMove() >= key){
            right = mid-1;
        }
        else{left = mid + 1;}
    }
    return left;
}

    int Lowerbound_noshift(PrmPeakPtrVec & merge_list, int key){
        int left = 0;
        int right = merge_list.size() - 1;
        while(left <= right){
            int mid=(left + right) >> 1;
            if(merge_list[mid]->getIntPosition() >= key){
                right = mid-1;
            }
            else{left = mid + 1;}
        }
        return left;
    }


    LCSFilter::BucketPtr LCSFilter::getBinAtPos(PrmPeakPtrVec & merge_list, int pos, bool fixed_tol){


        int lb_idx = Lowerbound(merge_list, pos);

        if(lb_idx == merge_list.size()){
            lb_idx = lb_idx - 1;
        }
        PrmPeakPtrVec temp_list;
        if(fixed_tol) {
            int tol = mng_ptr_->bin_tol_;
            int y = lb_idx;
            int z = lb_idx - 1;

            while (y < merge_list.size()) {
                if (abs(merge_list[y]->getIntPosAfterMove() - pos) <= tol) {
                    temp_list.push_back(merge_list[y]);

                    y = y + 1;
                } else { break; }
            }
            while (z >= 0) {
                if (abs(pos - merge_list[z]->getIntPosAfterMove()) <= tol) {
                    temp_list.push_back(merge_list[z]);
//                if(merge_list[z]->getPeakId()>267){
//                    std::cout<<z<<" "<<merge_list[z]->getPeakId()<<std::endl;
//                }
                    z = z - 1;
                } else { break; }
            }
        }else{
            int max_tol = mng_ptr_->max_bin_tol_;
            int y = lb_idx;
            int z = lb_idx - 1;

            while (y < merge_list.size()) {
                int dist = abs(merge_list[y]->getIntPosAfterMove() - pos);
                if (dist <= max_tol) {
                    if(dist <= merge_list[y]->getIntTolerance()) {
//                        if(dist <= merge_list[y]->getIntNRelaxCStrictTolerance()) {
                        temp_list.push_back(merge_list[y]);
                    }
                    y = y + 1;
                } else { break; }
            }
            while (z >= 0) {
                int dist = abs(pos - merge_list[z]->getIntPosAfterMove());

                if (dist <= max_tol) {
                    if(dist <= merge_list[z]->getIntTolerance()) {
//                    if(dist <= merge_list[z]->getIntNRelaxCStrictTolerance()) {
                        temp_list.push_back(merge_list[z]);
                    }
                    z = z - 1;
                } else { break; }
            }
        }
        //std::cout<<"tempsize: "<<temp_list.size()<<std::endl;
        BucketPtr bucket_ptr;
        //std::cout<<"empty bucket: "<<bucket_ptr->peak_ptr_vec_.size()<<std::endl;
        if(!temp_list.empty()) {
            bucket_ptr = std::make_shared<Bucket>(temp_list);
            bucket_ptr->arrange_bucket();
//            bucket_ptr->getSeqPeakVec();
            //bucket_ptr->checkEnd(spec_peak_num);
            if (bucket_ptr->spec_peak_vec_.back() == mng_ptr_->peak_vec_.back()->getPeakId()) {
                bucket_ptr->end_bin = true;
            }
            bucket_ptr->bucket_position = pos;
        }
        return bucket_ptr;
    }


    void LCSFilter::HandleNonOverlap_2(int mod_dist, BucketPtr & pre_bin, BucketPtr & cur_bin, int head,
                                       int j,
                                       std::vector<std::vector<ModsFound>> & dp_array,
                                       std::vector<int> & mods_vec,
                                       std::string & seq_string,
                                       std::vector<std::pair<int,std::vector<int>>> & modlist_for_pos,
                                       std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map){
        if(!modlist_for_pos.empty()) {
            std::vector<std::string> MODS = mng_ptr_->MODS_VEC;
            std::vector<int> mods_in_gap(MODS.size(), 0);
            std::vector<int> best_mods_pos(2);

            bool Nmod = false;
            int Nmod_idx;
            for (int nx = 0; nx < mng_ptr_->N_mod_num_; nx++) {
                if (mods_vec[nx] > 0) {
                    Nmod = true;
                    Nmod_idx = nx;
                    break;
                }
            }

            //d_bond
            bool d_bond = false;
            if (mng_ptr_->DISULFIDE_BOND_IDX != -1 && mods_vec[mng_ptr_->DISULFIDE_BOND_IDX] > 0) {
                d_bond = true;
            }

            if (Nmod) {
                bool valid_N = false;
                int first_node = pre_bin->left_end;

                int bgn = Lowerbound(modlist_for_pos, first_node);
                int x;
                if (bgn < modlist_for_pos.size()) {
                    if (modlist_for_pos[bgn].first == first_node) {
                        x = bgn;
                        valid_N = true;
                    } else {
                        valid_N = false;
                    }
                } else { valid_N = false; }
//            for(int bi = bgn; bi < modlist_for_pos.size(); bi++){
//                if(modlist_for_pos[bi].first == first_node){
//                    x = bi;
//                    break;
//                }
//                if(modlist_for_pos[bi].first > first_node){
//                    valid_N = false;
//                    break;
//                }
//            }
                if (valid_N) {
                    for (int q = 0; q < modlist_for_pos[x].second.size(); q++) {
                        if (modlist_for_pos[x].second[q] == Nmod_idx) {
                            std::vector<int> mods_counted(MODS.size(), 0);
                            mods_counted[Nmod_idx]++;
                            int mod_left = modlist_for_pos[x].first;
                            int end = Lowerbound(modlist_for_pos, cur_bin->right_end) - 1;
                            int y = x + 1;
                            std::vector<int> c_pos_list;
                            bool found = true;
                            while (x <= end && y <= end) {
                                for (int mi = 0; mi < modlist_for_pos[y].second.size(); mi++) {
                                    mods_counted[modlist_for_pos[y].second[mi]]++;

                                    if (modlist_for_pos[y].second[mi] == mng_ptr_->DISULFIDE_BOND_IDX) {
                                        c_pos_list.push_back(modlist_for_pos[y].first);
                                    }

                                }
                                checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
                                          mng_ptr_->NEG_ISO_SHIFT_IDX, mng_ptr_->DISULFIDE_BOND_IDX, d_bond, c_pos_list, mng_ptr_->min_Dbond_dist_);
                                if (!found) {
                                    y++;
                                } else {
                                    int mod_right = modlist_for_pos[y].first + 1;
                                    int local_score = -1;
                                    ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                                    mods.mods_vec_ = mods_vec;
                                    mods.mods_mass = mod_dist;
                                    dp_array[j].push_back(mods);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
            } else {
                int bestloss;
                std::vector<int> dbond_pos_list;
//            countMods(pre_bin->right_end, cur_bin->left_end, mods_in_gap, seq_string, mng_ptr_->RES_MOD_TABLE, MODS);
                countMods(pre_bin->right_end, cur_bin->left_end, mods_in_gap, seq_string, mng_ptr_->RES_MOD_TABLE, MODS,
                          dbond_pos_list, mng_ptr_->dbond_name);
                bool found = true;
//            checkMods(found, mods_in_gap, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX, mng_ptr_->NEG_ISO_SHIFT_IDX);
                checkMods(found, mods_in_gap, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
                          mng_ptr_->NEG_ISO_SHIFT_IDX, mng_ptr_->DISULFIDE_BOND_IDX, d_bond, dbond_pos_list, mng_ptr_->min_Dbond_dist_);

                if (found) {
                    int mod_left = pre_bin->right_end;
                    int mod_right = cur_bin->left_end;
                    int local_score = -1;
                    //if(mod_right - mod_left <= mng_ptr_->K) {
                    //int local_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score;
                    ModsFound cur_mods = ModsFound(mod_left, mod_right, local_score, -1);
                    cur_mods.mods_vec_ = mods_vec;
                    cur_mods.mods_mass = mod_dist;
                    dp_array[j].push_back(cur_mods);
                    //}
                } else {
                    int bgn = Lowerbound(modlist_for_pos, head);
                    if (bgn < modlist_for_pos.size()) {
                        std::vector<int> mods_counted(MODS.size(), 0);
                        int end = Lowerbound(modlist_for_pos, cur_bin->right_end) - 1;
                        int x = bgn;
                        int y = bgn;
                        std::vector<int> c_pos_list;
                        while (x <= end && y <= end) {
                            for (int mi = 0; mi < modlist_for_pos[y].second.size(); mi++) {
                                mods_counted[modlist_for_pos[y].second[mi]]++;

                                if (modlist_for_pos[y].second[mi] == mng_ptr_->DISULFIDE_BOND_IDX) {
                                    c_pos_list.push_back(modlist_for_pos[y].first);
                                }

                            }
//                        checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
//                                  mng_ptr_->NEG_ISO_SHIFT_IDX);
                            checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
                                      mng_ptr_->NEG_ISO_SHIFT_IDX, mng_ptr_->DISULFIDE_BOND_IDX, d_bond, c_pos_list, mng_ptr_->min_Dbond_dist_);
                            if (!found) {
                                y++;
                            } else {
                                while (found) {
                                    x++;
                                    for (int mj = 0; mj < modlist_for_pos[x - 1].second.size(); mj++) {
                                        mods_counted[modlist_for_pos[x - 1].second[mj]]--;

                                        if (modlist_for_pos[x - 1].second[mj] == mng_ptr_->DISULFIDE_BOND_IDX) {
                                            c_pos_list.erase(c_pos_list.begin());
                                        }

                                    }

//                                checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
//                                          mng_ptr_->NEG_ISO_SHIFT_IDX);
                                    checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
                                              mng_ptr_->NEG_ISO_SHIFT_IDX, mng_ptr_->DISULFIDE_BOND_IDX, d_bond, c_pos_list,
                                              mng_ptr_->min_Dbond_dist_);
                                }
                                int mod_left = modlist_for_pos[x - 1].first;
                                int mod_right = modlist_for_pos[y].first + 1;
                                int local_score = -1;
                                //if(mod_right - mod_left <= mng_ptr_->K) {
                                //int local_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score
                                //                  - computeLoss(pre_bin->seq_peak_vec_, cur_bin->seq_peak_vec_, mod_left,
                                //                                mod_right);
                                //if(local_score != 0) {
                                ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                                mods.mods_vec_ = mods_vec;
                                mods.mods_mass = mod_dist;
                                dp_array[j].push_back(mods);

                                //}
                                //}
                                y++;
                            }
                        }
                    }
                }
            }

            puv_map.insert({std::make_pair(pre_bin->bucket_position, cur_bin->bucket_position), dp_array[j]});
        }
    }

    void LCSFilter::HandleOverlap_2(int mod_dist, BucketPtr & pre_bin, BucketPtr & cur_bin, int head,
                                    int j,
                                    std::vector<std::vector<ModsFound>> & dp_array,
                                    std::vector<int> & mods_vec,
                                    std::string & seq_string,
                                    std::vector<std::pair<int,std::vector<int>>> & modlist_for_pos,
                                    std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map){
        if(!modlist_for_pos.empty()) {
            bool found = true;
            std::vector<std::string> MODS = mng_ptr_->MODS_VEC;

            bool only_iso_shift = true;
            for (int i = 0; i < mods_vec.size(); i++) {
                if (i != mng_ptr_->POS_ISO_SHIFT_IDX && i != mng_ptr_->NEG_ISO_SHIFT_IDX) {
                    if (mods_vec[i] > 0) {
                        only_iso_shift = false;
                        break;
                    }
                }
            }

            bool Nmod = false;
            int Nmod_idx;
            for (int nx = 0; nx < mng_ptr_->N_mod_num_; nx++) {
                if (mods_vec[nx] > 0) {
                    Nmod = true;
                    Nmod_idx = nx;
                    break;
                }
            }

            bool d_bond = false;
            if (mng_ptr_->DISULFIDE_BOND_IDX != -1 && mods_vec[mng_ptr_->DISULFIDE_BOND_IDX] > 0) {
                d_bond = true;
            }

            if (!only_iso_shift) {
                if (Nmod) {
                    bool valid_N = true;
                    int first_node = pre_bin->left_end;
                    int bgn = Lowerbound(modlist_for_pos, first_node);
                    int x;
                    if (bgn < modlist_for_pos.size()) {
                        if (modlist_for_pos[bgn].first == first_node) {
                            x = bgn;
                            valid_N = true;
                        } else {
                            valid_N = false;
                        }
                    } else { valid_N = false; }
//                for(int bi = bgn; bi < modlist_for_pos.size(); bi++){
//                    if(modlist_for_pos[bi].first == first_node){
//                        x = bi;
//                        break;
//                    }
//                    if(modlist_for_pos[bi].first > first_node){
//                        valid_N = false;
//                        break;
//                    }
//                }
                    if (valid_N) {
                        for (int q = 0; q < modlist_for_pos[x].second.size(); q++) {
                            if (modlist_for_pos[x].second[q] == Nmod_idx) {
                                std::vector<int> mods_counted(MODS.size(), 0);
                                mods_counted[Nmod_idx]++;
                                int mod_left = modlist_for_pos[x].first;
                                int end = Lowerbound(modlist_for_pos, cur_bin->right_end) - 1;
                                int y = x + 1;
                                std::vector<int> c_pos_list;
                                while (x <= end && y <= end) {
                                    for (int mi = 0; mi < modlist_for_pos[y].second.size(); mi++) {
                                        mods_counted[modlist_for_pos[y].second[mi]]++;

                                        if (modlist_for_pos[y].second[mi] == mng_ptr_->DISULFIDE_BOND_IDX) {
                                            c_pos_list.push_back(modlist_for_pos[y].first);
                                        }

                                    }
//                                checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
//                                          mng_ptr_->NEG_ISO_SHIFT_IDX);
                                    checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
                                              mng_ptr_->NEG_ISO_SHIFT_IDX,mng_ptr_->DISULFIDE_BOND_IDX, d_bond, c_pos_list,
                                              mng_ptr_->min_Dbond_dist_);
                                    if (!found) {
                                        y++;
                                    } else {
                                        int mod_right = modlist_for_pos[y].first + 1;
                                        int local_score = -1;
                                        ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                                        mods.mods_vec_ = mods_vec;
                                        mods.mods_mass = mod_dist;
                                        dp_array[j].push_back(mods);
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                } else {
                    int bgn = Lowerbound(modlist_for_pos, head);
                    if (bgn < modlist_for_pos.size()) {
                        std::vector<int> mods_counted(MODS.size(), 0);
                        int end = Lowerbound(modlist_for_pos, cur_bin->right_end) - 1;
                        int x = bgn;
                        int y = bgn;
                        std::vector<int> c_pos_list;
                        while (x <= end && y <= end) {
                            for (int mi = 0; mi < modlist_for_pos[y].second.size(); mi++) {
                                mods_counted[modlist_for_pos[y].second[mi]]++;

                                if (modlist_for_pos[y].second[mi] == mng_ptr_->DISULFIDE_BOND_IDX) {
                                    c_pos_list.push_back(modlist_for_pos[y].first);
                                }

                            }
//                        checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
//                                  mng_ptr_->NEG_ISO_SHIFT_IDX);
                            checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
                                      mng_ptr_->NEG_ISO_SHIFT_IDX, mng_ptr_->DISULFIDE_BOND_IDX, d_bond, c_pos_list, mng_ptr_->min_Dbond_dist_);
                            if (!found) {
                                y++;
                            } else {
                                while (found) {
                                    x++;
                                    for (int mj = 0; mj < modlist_for_pos[x - 1].second.size(); mj++) {
                                        mods_counted[modlist_for_pos[x - 1].second[mj]]--;

                                        if (modlist_for_pos[x - 1].second[mj] == mng_ptr_->DISULFIDE_BOND_IDX) {
                                            c_pos_list.erase(c_pos_list.begin());
                                        }

                                    }
//                                checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
//                                          mng_ptr_->NEG_ISO_SHIFT_IDX);
                                    checkMods(found, mods_counted, mods_vec, mng_ptr_->POS_ISO_SHIFT_IDX,
                                              mng_ptr_->NEG_ISO_SHIFT_IDX, mng_ptr_->DISULFIDE_BOND_IDX, d_bond, c_pos_list,
                                              mng_ptr_->min_Dbond_dist_);
                                }

                                int mod_left = modlist_for_pos[x - 1].first;
                                int mod_right = modlist_for_pos[y].first + 1;
                                //if(mod_right - mod_left <= mng_ptr_->K) {
                                //int local_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score
                                //                  - computeLoss(pre_bin->seq_peak_vec_, cur_bin->seq_peak_vec_, mod_left,
                                //                                mod_right);
                                int local_score = -1;
                                //if(local_score != 0) {
                                ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                                mods.mods_vec_ = mods_vec;
                                mods.mods_mass = mod_dist;
                                dp_array[j].push_back(mods);
                                //}
                                //}
                                y++;
                            }
                        }
                    }
                }
            } else {
                std::vector<int> overlap_node;
                int tail = cur_bin->right_end;
                for (int p = 0; p < pre_bin->seq_peak_vec_.size(); p++) {
                    if (pre_bin->seq_peak_vec_[p] >= head && pre_bin->seq_peak_vec_[p] <= tail) {
                        overlap_node.push_back(pre_bin->seq_peak_vec_[p]);
                    }
                }
                for (int p = 0; p < cur_bin->seq_peak_vec_.size(); p++) {
                    if (cur_bin->seq_peak_vec_[p] >= head && cur_bin->seq_peak_vec_[p] <= tail) {
                        overlap_node.push_back(cur_bin->seq_peak_vec_[p]);
                    }
                }
                sort(overlap_node.begin(), overlap_node.end());
//            std::cout<<overlap_node.size()<<std::endl;
                overlap_node.erase(unique(overlap_node.begin(), overlap_node.end()), overlap_node.end());

                for (int p = 1; p < overlap_node.size(); p++) {
                    ModsFound mods = ModsFound(overlap_node[p - 1], overlap_node[p], -1, -1);
                    mods.mods_vec_ = mods_vec;
                    mods.mods_mass = mod_dist;
                    dp_array[j].push_back(mods);
                }
            }
            puv_map.insert({std::make_pair(pre_bin->bucket_position, cur_bin->bucket_position), dp_array[j]});
        }
    }


    void LCSFilter::HandleNonOverlap_1(int mod_dist, BucketPtr & pre_bin, BucketPtr & cur_bin, int head,
                                     int j,
                                     std::vector<std::vector<ModsFound>> & dp_array,
                                     std::vector<int> & mods_vec,
                                     std::string & seq_string,
                                     std::vector<std::pair<int,int>> & mods_pos_list,
                                       std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map){
        std::vector<std::string> MODS = mng_ptr_->MODS_VEC;
        std::vector<int> mods_in_gap(MODS.size(), 0);
        std::vector<int> best_mods_pos(2);
        int bestloss;
        countMods(pre_bin->right_end, cur_bin->left_end, mods_in_gap, seq_string, MODS);
        bool found = true;
        checkMods(found, mods_in_gap, mods_vec);
        if(found){
            int mod_left = pre_bin->right_end;
            int mod_right = cur_bin->left_end;
            int local_score = -1;
            //if(mod_right - mod_left <= mng_ptr_->K) {
            //int local_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score;
            ModsFound cur_mods = ModsFound(mod_left, mod_right, local_score, -1);
            cur_mods.mods_vec_ = mods_vec;
            cur_mods.mods_mass = mod_dist;
            dp_array[j].push_back(cur_mods);
            //}
        }
        else{
            int bgn = Lowerbound(mods_pos_list, head);
            if(bgn < mods_pos_list.size()) {
                std::vector<int> mods_counted(MODS.size(), 0);
                int end = Lowerbound(mods_pos_list, cur_bin->right_end) - 1;
                int x = bgn;
                int y = bgn;
                while(x <= end && y <= end){
                    mods_counted[mods_pos_list[y].second]++;
                    checkMods(found, mods_counted, mods_vec);
                    if(!found){
                        y ++;
                    }
                    else{
                        while(found){
                            x ++;
                            mods_counted[mods_pos_list[x - 1].second] --;
                            checkMods(found, mods_counted, mods_vec);
                        }
                        int mod_left = mods_pos_list[x - 1].first;
                        int mod_right = mods_pos_list[y].first + 1;
                        int local_score = -1;
                        //if(mod_right - mod_left <= mng_ptr_->K) {
                        //int local_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score
                        //                  - computeLoss(pre_bin->seq_peak_vec_, cur_bin->seq_peak_vec_, mod_left,
                        //                                mod_right);
                        //if(local_score != 0) {
                            ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                            mods.mods_vec_ = mods_vec;
                            mods.mods_mass = mod_dist;
                            dp_array[j].push_back(mods);
                        //}
                        //}
                        y ++;
                    }
                }
            }
        }
        puv_map.insert({std::make_pair(pre_bin->bucket_position, cur_bin->bucket_position), dp_array[j]});
    }




    void LCSFilter::HandleOverlap_1(int mod_dist, BucketPtr & pre_bin, BucketPtr & cur_bin, int head,
                                  int j,
                                  std::vector<std::vector<ModsFound>> & dp_array,
                                  std::vector<int> & mods_vec,
                                  std::string & seq_string,
                                  std::vector<std::pair<int,int>> & mods_pos_list,
                                    std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map){
        bool found = true;
        std::vector<std::string> MODS = mng_ptr_->MODS_VEC;
        int bgn = Lowerbound(mods_pos_list, head);
        if(bgn < mods_pos_list.size()) {
            std::vector<int> mods_counted(MODS.size(), 0);
            int end = Lowerbound(mods_pos_list, cur_bin->right_end) - 1;
            int x = bgn;
            int y = bgn;
            while(x <= end && y <= end){
                mods_counted[mods_pos_list[y].second]++;
                checkMods(found, mods_counted, mods_vec);
                if(!found){
                    y ++;
                }
                else{
                    while(found){
                        x ++;
                        mods_counted[mods_pos_list[x - 1].second] --;
                        checkMods(found, mods_counted, mods_vec);
                    }
                    int mod_left = mods_pos_list[x - 1].first;
                    int mod_right = mods_pos_list[y].first + 1;
                    //if(mod_right - mod_left <= mng_ptr_->K) {
                    //int local_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score
                    //                  - computeLoss(pre_bin->seq_peak_vec_, cur_bin->seq_peak_vec_, mod_left,
                    //                                mod_right);
                    int local_score = -1;
                    //if(local_score != 0) {
                        ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                        mods.mods_vec_ = mods_vec;
                        mods.mods_mass = mod_dist;
                        dp_array[j].push_back(mods);
                    //}
                    //}
                    y ++;
                }
            }
        }
        puv_map.insert({std::make_pair(pre_bin->bucket_position, cur_bin->bucket_position), dp_array[j]});
    }


    std::vector<std::pair<int,int>> getMergeModRange(std::vector<std::pair<int,int>> A, std::vector<std::pair<int,int>> B){
        std::vector<std::pair<int,int>> Aend_vec;
        int x = 0;
        int y = 0;
        int s = -1;
        while(x < A.size() && y < B.size()){
            while(B[y].second <= A[x].second){
                y ++;
                if(y >= B.size()){
                    break;
                }
            }
            if(y - 1 > s){
                s = y - 1;
                Aend_vec.push_back(std::make_pair(std::min(B[s].first, A[x].first), A[x].second));
            }
            x++;
        }
        return Aend_vec;
}


    void LCSFilter::HandleAntibody(int mod_dist, BucketPtr & pre_bin, BucketPtr & cur_bin, int head,
                                   int j,
                                   std::vector<std::vector<ModsFound>> & dp_array,
                                   std::vector<int> & mods_vec,
                                   std::string & seq_string,
                                   std::vector<std::pair<int,int>> & mods_pos_list,
                                   std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map) {
        std::vector<std::string> MODS = mng_ptr_->MODS_VEC;

        bool Nterm_found = true;
        bool Dbond_found = true;
        bool vmods_found = true;
        bool Dbond = false;
        std::vector<std::pair<int, int>> Dbond_vec;
        std::vector<std::pair<int, int>> Vmod_vec;
        bool Nmod = false;
        int N_mod_idx;
        for (int i = 0; i < mng_ptr_->N_mod_num_; i++) {
            if (mods_vec[i] > 0) {
                Nmod = true;
                N_mod_idx = i;
            }
        }

        int Nmods_left, Nmods_right;
        if (Nmod) {
            if (seq_string[0] == MODS[N_mod_idx][0]) {
                Nterm_found = true;
                Nmods_left = 0;
                Nmods_right = 1;
            } else {
                Nterm_found = false;
            }
        }

        if (mods_vec.back() > 0) {
            Dbond = true;
            std::vector<int> Dbond_mods_vec(mods_vec.size(), 0);
            Dbond_mods_vec.back() = mods_vec.back();
            int bgn = Lowerbound(mods_pos_list, head);
            if (bgn < mods_pos_list.size()) {
                std::vector<int> mods_counted(MODS.size(), 0);
                int end = Lowerbound(mods_pos_list, cur_bin->right_end) - 1;
                int x = bgn;
                int y = bgn;
                while (x <= end && y <= end) {
                    mods_counted[mods_pos_list[y].second]++;
                    checkMods(Dbond_found, mods_counted, Dbond_mods_vec);
                    if (!Dbond_found) {
                        y++;
                    } else {
                        while (Dbond_found) {
                            x++;
                            mods_counted[mods_pos_list[x - 1].second]--;
                            checkMods(Dbond_found, mods_counted, Dbond_mods_vec);
                        }
                        int mod_left = mods_pos_list[x - 1].first;
                        int mod_right = mods_pos_list[y].first + 1;
                        if (mods_vec.back() == 4) {
                            int first_dist, last_dist;
                            for (int z = x; z < y; z++) {
                                if (mods_pos_list[z].second == mods_vec.size() - 1) {
                                    first_dist = mods_pos_list[z].first - (mod_left + 1);
                                    break;
                                }
                            }
                            for (int z = y; z > x; z--) {
                                if (mods_pos_list[z].second == mods_vec.size() - 1) {
                                    last_dist = mod_right - 1 - (mods_pos_list[z].first + 1);
                                    break;
                                }
                            }
                            if (first_dist > mng_ptr_->min_Dbond_dist_ && last_dist > mng_ptr_->min_Dbond_dist_) {
                                Dbond_vec.push_back(std::make_pair(mod_left, mod_right));

                            }
                        }
                        if (mods_vec.back() == 2) {
                            if (mod_right - 1 - (mod_left + 1) > mng_ptr_->min_Dbond_dist_) {
                                Dbond_vec.push_back(std::make_pair(mod_left, mod_right));
                                //std::cout<<"("<<mod_left<<","<<mod_right<<")"<<std::endl;
                            }
                        }
                        y++;
                    }
                }
            }
        }

        bool vmods = false;
        for (int i = mng_ptr_->N_mod_num_; i < mods_vec.size() - 1; i++) {
            if (mods_vec[i] > 0) {
                vmods = true;
                break;
            }
        }
        if (vmods) {
            std::vector<int> vmods_vec(mods_vec.size(), 0);
            for (int i = mng_ptr_->N_mod_num_; i < mods_vec.size() - 1; i++) {
                vmods_vec[i] = mods_vec[i];
            }
            int bgn = Lowerbound(mods_pos_list, head);
            if (bgn < mods_pos_list.size()) {
                std::vector<int> mods_counted(MODS.size(), 0);
                int end = Lowerbound(mods_pos_list, cur_bin->right_end) - 1;
                int x = bgn;
                int y = bgn;
                while (x <= end && y <= end) {
                    mods_counted[mods_pos_list[y].second]++;
                    checkMods(vmods_found, mods_counted, vmods_vec);
                    if (!vmods_found) {
                        y++;
                    } else {
                        while (vmods_found) {
                            x++;
                            mods_counted[mods_pos_list[x - 1].second]--;
                            checkMods(vmods_found, mods_counted, vmods_vec);
                        }
                        int mod_left = mods_pos_list[x - 1].first;
                        int mod_right = mods_pos_list[y].first + 1;
                        Vmod_vec.push_back(std::make_pair(mod_left, mod_right));
                        y++;
                    }
                }
            }
        }

        if ((Dbond && Dbond_vec.empty()) || (vmods && Vmod_vec.empty())) {
            ;
        } else {
            std::vector<std::pair<int, int>> notNmods_vec;
            if (Vmod_vec.empty()) {
                notNmods_vec = Dbond_vec;
            } else {
                if (Dbond_vec.empty()) {
                    notNmods_vec = Vmod_vec;
                }
            }
            if (!Vmod_vec.empty() && !Dbond_vec.empty()) {
                std::vector<std::pair<int, int>> Vend_vec = getMergeModRange(Vmod_vec, Dbond_vec);
                std::vector<std::pair<int, int>> Dend_vec = getMergeModRange(Dbond_vec, Vmod_vec);
                notNmods_vec = Vend_vec;
                for (int i = 0; i < Dend_vec.size(); i++) {
                    notNmods_vec.push_back(Dend_vec[i]);
                }
            }


            int mod_left, mod_right;
            if (Nmod && Nterm_found) {
                mod_left = Nmods_left;
                if (!vmods && mods_vec.back() == 0) {
                    mod_right = Nmods_right;
                    int local_score = -1;
                    ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                    mods.mods_vec_ = mods_vec;
                    mods.mods_mass = mod_dist;
                    dp_array[j].push_back(mods);
                } else {
                    if (!notNmods_vec.empty()) {
                        for (int i = 0; i < notNmods_vec.size(); i++) {
                            mod_right = notNmods_vec[i].second;
                            int local_score = -1;
                            ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                            mods.mods_vec_ = mods_vec;
                            mods.mods_mass = mod_dist;
                            dp_array[j].push_back(mods);
                        }
                    }
                }
            }
            if (!Nmod) {
                if (!notNmods_vec.empty()) {
                    for (int i = 0; i < notNmods_vec.size(); i++) {
                        mod_left = notNmods_vec[i].first;
                        mod_right = notNmods_vec[i].second;
                        int local_score = -1;
                        ModsFound mods = ModsFound(mod_left, mod_right, local_score, -1);
                        mods.mods_vec_ = mods_vec;
                        mods.mods_mass = mod_dist;
                        dp_array[j].push_back(mods);
                    }
                }
            }
            puv_map.insert({std::make_pair(pre_bin->bucket_position, cur_bin->bucket_position), dp_array[j]});
        }
    }


    void LCSFilter::SearchPaths_1(BucketPtr u, int mod_dist, int mod_num, int isotope_value, int isotope_shift_num, std::vector<int> mods_vec, int & total_mods_num, int & iso_sum, int & total_iso_num, std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, SearchSet & ss, std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map){

     if(path.size() >= 1) {
            int j = path.size();
            LCSFilter::BucketPtr pre_bin = path.back();
            LCSFilter::BucketPtr cur_bin = u;
            path.push_back(u);
//            for(int i = 0; i < path.size(); i++){
//                std::cout<<path[i]->bucket_position<<",";
//            }
//            std::cout<<std::endl;
//            std::cout<<"pathsize: "<<path.size()<<std::endl;
            std::vector<ModsFound> temp;
            dp_array.push_back(temp);
            total_mods_num = total_mods_num + mod_num;
            total_iso_num = total_iso_num + isotope_shift_num;
            iso_sum = iso_sum + isotope_value;

//            if(iso_sum >= 2 || iso_sum <=-2){
//                return;
//            }
//
//            if(total_mods_num > mng_ptr_->max_total_mods_num || total_iso_num > mng_ptr_->max_total_iso_num){
//                return;
//            }else if(!u->end_bin && total_mods_num == mng_ptr_->max_total_mods_num){
//                return;
//            }

         if(!u->end_bin && total_mods_num == mng_ptr_->max_total_mods_num){
             return;
         }

//            if(path.size() >= mng_ptr_->max_path_size_ && !u->end_bin){
//                return;
//            }


            int bgn_peak1 = Lowerbound(pre_bin->seq_peak_vec_, dp_array[j - 1][0].right_);
            int head1 = pre_bin->seq_peak_vec_[bgn_peak1];


            int tail1 = cur_bin->right_end;



            if ((tail1 - head1) >= 1 && (tail1 - head1) >= mod_num) {
                int pre_bin_pos = pre_bin->bucket_position;
                int cur_bin_pos = cur_bin->bucket_position;
                if(puv_map.count(std::make_pair(pre_bin_pos, cur_bin_pos)) == 0) {
                    if (pre_bin->right_end <= cur_bin->left_end) {
//                        HandleNonOverlap_1(mod_dist, pre_bin, cur_bin, head1, j, dp_array, mods_vec,
//                                           ss.seq_string_,
//                                           ss.mods_pos_list_, puv_map);

                        HandleNonOverlap_2(mod_dist, pre_bin, cur_bin, head1, j, dp_array, mods_vec,
                                           ss.seq_string_,
                                           ss.modlist_for_pos_, puv_map);

                    } else {
//                        HandleOverlap_1(mod_dist, pre_bin, cur_bin, head1, j, dp_array, mods_vec,
//                                        ss.seq_string_,
//                                        ss.mods_pos_list_, puv_map);

                        HandleOverlap_2(mod_dist, pre_bin, cur_bin, head1, j, dp_array, mods_vec,
                                        ss.seq_string_,
                                        ss.modlist_for_pos_, puv_map);
                    }
                }else{
                    dp_array[j] = puv_map[std::make_pair(pre_bin_pos, cur_bin_pos)];
                }
            }
            if(dp_array[j].empty()){
                return;
            }else {
                ss.tree_size_ = ss.tree_size_ + 1;
                for (int k = 0; k < dp_array[j - 1].size(); k++) {
                    int bgn_peak = Lowerbound(pre_bin->seq_peak_vec_, dp_array[j - 1][k].right_);
                    int head = pre_bin->seq_peak_vec_[bgn_peak];
                    int tail = cur_bin->right_end;

//                std::vector<int> mods_between(mng_ptr_->MODS_VEC.size(), 0);
//                countMods(head, tail, mods_between, ss.seq_string_, mng_ptr_->MODS_VEC);
//                bool found = true;
//                checkMods(found, mods_between, ss.mod_mass_table_[mod_dist]);

                    if (tail - head >= mod_num) {
                        for(int mm = 0; mm < dp_array[j].size(); mm++){
                            if(dp_array[j][mm].left_ >= head && dp_array[j][mm].right_ <= tail){
                                int now_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score
                                                 - computeLoss(pre_bin->seq_peak_vec_, cur_bin->seq_peak_vec_, dp_array[j][mm].left_,dp_array[j][mm].right_);

                                if(now_score > dp_array[j][mm].local_score_){
                                    dp_array[j][mm].local_score_ = now_score;
                                    dp_array[j][mm].pre_mods_ = k;
                                }
                            }
                        }
                    }
                }
            }
        }else{
            path.push_back(u);
         ss.tree_size_ = ss.tree_size_ + 1;
        }
        //std::cout<<graph_set_[u.bin_idx].size()<<std::endl;
        if(u->end_bin){
            //std::cout<<"pathsize: "<<path.size()<<std::endl;
            for(int s = 0; s < dp_array.back().size(); s++){
                if(dp_array.back()[s].local_score_ > ss.max_score){
                    ss.max_score = dp_array.back()[s].local_score_;
                    ss.best_path = path;
                    ss.best_dp = dp_array;
                }
                if(dp_array.back()[s].local_score_ > ss.max_score_for_cur_bgn){
                    ss.max_score_for_cur_bgn = dp_array.back()[s].local_score_;
                }
            }
            ss.path_list.push_back(path);

            return;
        }
//        else if(gs.graph_set_.count(u.bin_idx) == 0){
//            //path_list.push_back(path);
//            return;
//        }
        for(int i = 0; i < ss.T_mass_comb_.size(); i++){

            //std::cout<<"i:"<<i<<std::endl;
            int nxt_moddist = get<0>(ss.T_mass_comb_[i]);
            int nxt_mod_num = get<2>(ss.T_mass_comb_[i]);
            int nxt_bin_pos = u->bucket_position + nxt_moddist;
            std::vector<int> nxt_mod_vec = get<1>(ss.T_mass_comb_[i]);
            int nxt_iso_num = 0;
            int nxt_iso_value = 0;



            if(nxt_mod_vec[mng_ptr_->POS_ISO_SHIFT_IDX] > 0 || nxt_mod_vec[mng_ptr_->NEG_ISO_SHIFT_IDX] > 0 ){
                nxt_mod_num = nxt_mod_num - 1;
                nxt_iso_num = nxt_iso_num + 1;
                if(nxt_mod_vec[mng_ptr_->POS_ISO_SHIFT_IDX] > 0){
                    nxt_iso_value = 1;
                }else{
                    nxt_iso_value = -1;
                }
            }

            bool Nmod = false;
            for(int nx = 0; nx < mng_ptr_->N_mod_num_; nx ++){
                if(nxt_mod_vec[nx] > 0){
                    Nmod = true;
                    break;
                }
            }
            if((!u->bgn_bin) && Nmod){
                continue;
            }

            if(total_iso_num + nxt_iso_num > mng_ptr_->max_total_iso_num || total_mods_num + nxt_mod_num > mng_ptr_->max_total_mods_num){
                continue;
            }

            if(iso_sum + nxt_iso_value >= 2 || iso_sum + nxt_iso_value <= -2){
                continue;
            }

//            if(mng_ptr_->DISULFIDE_BOND_IDX != - 1 && nxt_mod_vec[mng_ptr_->DISULFIDE_BOND_IDX] > 2){
//                continue;
//            }


            BucketPtr v = getBinAtPos(ss.merge_list_, nxt_bin_pos, mng_ptr_->use_fixed_tol);

            if(v != NULL &&
               (v->right_end - u->left_end) > nxt_mod_num)

                //(v->left_end - u->right_end) > 0)
            {

                //std::cout<<"found i: "<<i<<std::endl;
                SearchPaths_1(v, nxt_moddist, nxt_mod_num, nxt_iso_value, nxt_iso_num, nxt_mod_vec, total_mods_num, iso_sum, total_iso_num,  path, dp_array, ss, puv_map);
                path.pop_back();
                dp_array.pop_back();
                total_mods_num = total_mods_num - nxt_mod_num;
                total_iso_num = total_iso_num - nxt_iso_num;
                iso_sum = iso_sum - nxt_iso_value;
            }
        }

    }

    void LCSFilter::SearchPaths_Antibody(BucketPtr u, int mod_dist, int mod_num, std::vector<int> mods_vec, int & total_mods_num, std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, SearchSet & ss, std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map){

        if(path.size() >= 1) {
            int j = path.size();
            LCSFilter::BucketPtr pre_bin = path.back();
            LCSFilter::BucketPtr cur_bin = u;
            path.push_back(u);

            std::vector<ModsFound> temp;
            dp_array.push_back(temp);
            total_mods_num = total_mods_num + mod_num;
            if(total_mods_num > mng_ptr_->max_total_mods_num){
                return;
            }else if(!u->end_bin && total_mods_num == mng_ptr_->max_total_mods_num){
                return;
            }


            int bgn_peak1 = Lowerbound(pre_bin->seq_peak_vec_, dp_array[j - 1][0].right_);
            int head1 = pre_bin->seq_peak_vec_[bgn_peak1];
            int tail1 = cur_bin->right_end;
            if (tail1 - head1 >= mod_num) {
                int pre_bin_pos = pre_bin->bucket_position;
                int cur_bin_pos = cur_bin->bucket_position;
                if(puv_map.count(std::make_pair(pre_bin_pos, cur_bin_pos)) == 0) {

                    HandleAntibody(mod_dist, pre_bin, cur_bin, head1, j, dp_array, mods_vec,
                                           ss.seq_string_,
                                           ss.mods_pos_list_, puv_map);
                }else{
                    dp_array[j] = puv_map[std::make_pair(pre_bin_pos, cur_bin_pos)];
                }
            }
            if(dp_array[j].empty()){
                return;
            }else {
                ss.tree_size_ = ss.tree_size_ + 1;
                for (int k = 0; k < dp_array[j - 1].size(); k++) {
                    int bgn_peak = Lowerbound(pre_bin->seq_peak_vec_, dp_array[j - 1][k].right_);
                    int head = pre_bin->seq_peak_vec_[bgn_peak];
                    int tail = cur_bin->right_end;


                    if (tail - head >= mod_num) {
                        for(int mm = 0; mm < dp_array[j].size(); mm++){
                            if(dp_array[j][mm].left_ >= head && dp_array[j][mm].right_ <= tail){
                                int now_score = dp_array[j - 1][k].local_score_ + cur_bin->self_score
                                                - computeLoss(pre_bin->seq_peak_vec_, cur_bin->seq_peak_vec_, dp_array[j][mm].left_,dp_array[j][mm].right_);
                                if(now_score > dp_array[j][mm].local_score_){

                                    dp_array[j][mm].local_score_ = now_score;
                                    dp_array[j][mm].pre_mods_ = k;
                                }
                            }
                        }
                    }
                }
            }
        }else{
            path.push_back(u);
            ss.tree_size_ = ss.tree_size_ + 1;
        }

        if(u->end_bin){

            for(int s = 0; s < dp_array.back().size(); s++){
                if(dp_array.back()[s].local_score_ > ss.max_score){
                    ss.max_score = dp_array.back()[s].local_score_;
                    ss.best_path = path;
                    ss.best_dp = dp_array;
                }
                if(dp_array.back()[s].local_score_ > ss.max_score_for_cur_bgn){
                    ss.max_score_for_cur_bgn = dp_array.back()[s].local_score_;
                }
            }
            ss.path_list.push_back(path);

            return;
        }
        for(int i = 0; i < ss.T_mass_comb_.size(); i++){

            int nxt_moddist = get<0>(ss.T_mass_comb_[i]);
            int nxt_mod_num = get<2>(ss.T_mass_comb_[i]);
            int nxt_bin_pos = u->bucket_position + nxt_moddist;
            std::vector<int> nxt_mod_vec = get<1>(ss.T_mass_comb_[i]);

            bool Nmod = false;
            for(int nx = 0; nx < mng_ptr_->N_mod_num_; nx ++){
                if(nxt_mod_vec[nx] > 0){
                    Nmod = true;
                    break;
                }
            }
            if((!u->bgn_bin) && Nmod){
                continue;
            }
            BucketPtr v = getBinAtPos(ss.merge_list_, nxt_bin_pos, mng_ptr_->use_fixed_tol);
            if(v != NULL &&
               (v->right_end - u->left_end) > nxt_mod_num)
            {
                SearchPaths_Antibody(v, nxt_moddist, nxt_mod_num, nxt_mod_vec, total_mods_num, path, dp_array, ss, puv_map);
                path.pop_back();
                dp_array.pop_back();
                total_mods_num = total_mods_num - nxt_mod_num;
            }
        }

    }


    void LCSFilter::backtrack_1(std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, ResSeqPtr & seq){
        std::vector<ModsFound> mods_list;
        int max_score = 0;
        ModsFound last_mods;
        for(int s = 0; s < dp_array.back().size(); s++){
            if(dp_array.back()[s].local_score_ > max_score){
                max_score = dp_array.back()[s].local_score_;
                last_mods = dp_array.back()[s];
            }
        }
        ModsFound mods = last_mods;
        for(int i = path.size() - 1; i >= 0; i--) {
            mods_list.insert(mods_list.begin(), mods);
            int k = mods.pre_mods_;
            if (i > 0) {
                mods = dp_array[i - 1][k];
            }
        }
        std::ofstream outFile;
        std::string result_file = mng_ptr_->resultpath+"LCSA_results.txt";
//                + "(K="
//                +std::to_string(mng_ptr_->K)
//                +",maxmods=" +std::to_string(mng_ptr_->max_total_mods_num) + ").txt"

        outFile.open(result_file,std::ios::app);
        int mod_mass = 0;
        int bgn_seq_peak_mass = mng_ptr_->seq_peak_mass_list[path[0]->seq_peak_vec_[0]];
        BucketPtr first_bin = path[0];
        int bin_shift = first_bin->bucket_position - first_bin->peak_ptr_vec_[0]->getIntPosAfterMove();
        outFile<<std::endl;
        outFile<<"========alignment sp: " + std::to_string(mng_ptr_->sp_id) + " seq: " + mng_ptr_->proteo_name + "======="<<std::endl;
        outFile<<std::setw(15)<<"prot_idx"<<std::setw(15)<<"spec_idx"
        <<std::setw(15)<<"res_mass"
        <<std::setw(15)<<"Ori_peak_pos"
        <<std::setw(15)<<"Mod_peak_pos"<<std::setw(15)<<"Shift"<<std::endl;
        for(int j = 0; j < path.size(); j++){
            BucketPtr b = path[j];
            if(j != path.size() - 1) {
                for (int p = 0; p < b->seq_peak_vec_.size(); p++) {
                    if (b->seq_peak_vec_[p] >= mods_list[j].right_ && b->seq_peak_vec_[p] <= mods_list[j + 1].left_){
                        if(j == 0 && bgn_seq_peak_mass == -1){
                            bgn_seq_peak_mass =mng_ptr_->seq_peak_mass_list[b->seq_peak_vec_[p]];
                            bin_shift = b->bucket_position - (0 - bgn_seq_peak_mass);
                        }
                        int seq_peak_mass = mng_ptr_->seq_peak_mass_list[b->seq_peak_vec_[p]] + mod_mass;
                        int spec_peak_mass = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getIntPosition();
                        outFile<<std::setw(15)<< b->seq_peak_vec_[p] + mng_ptr_->start_pos << std::setw(15) << b->spec_peak_vec_[p] << std::setw(15) << std::fixed << std::setprecision(0)
                               << seq_peak_mass
                               << std::setw(15) << spec_peak_mass
                               <<std::setw(15)<<seq_peak_mass - bgn_seq_peak_mass
                               //<<"\t"<< spec_peak_mass -bin_shift
                               <<std::setw(15)<< spec_peak_mass - (seq_peak_mass - bgn_seq_peak_mass)
                                       //                    <<"\t"<<seq_peak_mass - bgn_seq_peak_mass - (seq_peak_mass - mod_mass)
                               //                    <<"\t"<<b->bucket_position
                               <<std::endl;
                    }else if(b->seq_peak_vec_[p] >= mods_list[j + 1].left_){
                        break;
                    }
                }
                outFile<<"mods: ";
                for(int m = 0 ; m < mng_ptr_->MODS_VEC.size(); m++){
                    outFile<<mods_list[j + 1].mods_vec_[m]<<mng_ptr_->MODS_VEC[m];
                    mod_mass = mod_mass + mods_list[j+1].mods_vec_[m] * mng_ptr_->MODS_MASS_VEC[m];
                }

                //mod_mass = mod_mass + bucket_list[idx_path[j + 1]]->bucket_position - b->bucket_position;
                outFile<<"\t("<<mods_list[j+1].left_<<","<<mods_list[j+1].right_<<")"<<std::endl;


            }else{
                for (int p = 0; p < b->seq_peak_vec_.size(); p++) {
                    if (b->seq_peak_vec_[p] >= mods_list[j].right_) {
                        int seq_peak_mass = mng_ptr_->seq_peak_mass_list[b->seq_peak_vec_[p]] + mod_mass;
                        int spec_peak_mass = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getIntPosition();
                        outFile << std::setw(15)<<b->seq_peak_vec_[p] + mng_ptr_->start_pos<< std::setw(15) << b->spec_peak_vec_[p] << std::setw(15)
                                << seq_peak_mass
                                << std::setw(15)<< mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getIntPosition()
                                << std::setw(15) << seq_peak_mass - bgn_seq_peak_mass
                                //<<"\t"<< spec_peak_mass - bin_shift
                                << std::setw(15)<< spec_peak_mass - (seq_peak_mass - bgn_seq_peak_mass)
                                //                            <<"\t"<<seq_peak_mass - bgn_seq_peak_mass - (seq_peak_mass - mod_mass)
                                //                            <<"\t"<<b->bucket_position
                                <<std::endl;
                    }
                }
            }
        }
        outFile.close();
    }

    void LCSFilter::backtrack_empty(){
        std::ofstream outFile;
        std::string result_file = mng_ptr_->resultpath+"LCSA_results.txt";
        outFile.open(result_file,std::ios::app);
        outFile<<std::endl;
        outFile<<"========alignment sp: " + std::to_string(mng_ptr_->sp_id) + " seq: " + "None" + "======="<<std::endl;
        outFile<<std::endl;
}
    void LCSFilter::backtrack_2(std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, ResSeqPtr & seq){
        std::vector<ModsFound> mods_list;
        int max_score = 0;
        ModsFound last_mods;
        for(int s = 0; s < dp_array.back().size(); s++){
            if(dp_array.back()[s].local_score_ > max_score){
                max_score = dp_array.back()[s].local_score_;
                last_mods = dp_array.back()[s];
            }
        }

        int total_mods_num = 0;

        ModsFound mods = last_mods;
        for(int i = path.size() - 1; i >= 0; i--) {
            mods_list.insert(mods_list.begin(), mods);
            int k = mods.pre_mods_;

            int mods_num = 0;
            for(int j = 0; j < mods.mods_vec_.size(); j ++){
                if(j != mng_ptr_->POS_ISO_SHIFT_IDX && j != mng_ptr_->NEG_ISO_SHIFT_IDX){
                    mods_num = mods_num + mods.mods_vec_[j];
                }
            }
            total_mods_num = total_mods_num + mods_num;
            if (i > 0) {
                mods = dp_array[i - 1][k];
            }
        }
        mng_ptr_->total_mod_num_in_best_ = total_mods_num;


        std::ofstream outFile;
        std::string result_file = mng_ptr_->resultpath+"LCSA_results.txt";
//                + "(K="
//                +std::to_string(mng_ptr_->K)
//                +",maxmods=" +std::to_string(mng_ptr_->max_total_mods_num) + ").txt"

        outFile.open(result_file,std::ios::app);
        int mod_mass = 0;
        int bgn_seq_peak_mass = mng_ptr_->seq_peak_mass_list[path[0]->seq_peak_vec_[0]];
        BucketPtr first_bin = path[0];
        int bin_shift = first_bin->bucket_position - first_bin->peak_ptr_vec_[0]->getIntPosAfterMove();
        outFile<<std::endl;
        outFile<<"========alignment sp: " + std::to_string(mng_ptr_->sp_id) + " seq: " + mng_ptr_->proteo_name + "======="<<std::endl;
        outFile<<std::setw(15)<<"prot_idx"<<std::setw(15)<<"spec_idx"<<std::setw(15)<<"Mono Mass"<<std::setw(15)<<"Type"<<std::setw(15)<<"Shift"<<std::endl;
        for(int j = 0; j < path.size(); j++){
            BucketPtr b = path[j];
            if(j != path.size() - 1) {
                for (int p = 0; p < b->seq_peak_vec_.size(); p++) {
                    if (b->seq_peak_vec_[p] >= mods_list[j].right_ && b->seq_peak_vec_[p] <= mods_list[j + 1].left_){
                        if(j == 0 && bgn_seq_peak_mass == -1){
                            bgn_seq_peak_mass =mng_ptr_->seq_peak_mass_list[b->seq_peak_vec_[p]];
                            bin_shift = b->bucket_position - (0 - bgn_seq_peak_mass);
                        }
                        int seq_peak_mass = mng_ptr_->seq_peak_mass_list[b->seq_peak_vec_[p]] + mod_mass;
                        std::string type = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getBaseTypePtr()->getName();
                        int spec_peak_mass = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getIntPosition();
                        double spec_peak_double_mass = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getMonoMass();

                        double spec_peak_inten = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getIntensity();
                        outFile<<std::setw(15)<< b->seq_peak_vec_[p] + mng_ptr_->start_pos << std::setw(15) << b->spec_peak_vec_[p] << std::setw(15) << std::fixed << std::setprecision(3)
                                <<std::setw(15)<< spec_peak_double_mass
                                <<std::setw(15)<< type
//                                <<std::setw(15)<< spec_peak_inten
                               //<<"\t"<< spec_peak_mass -bin_shift
                        <<std::setprecision(3)<<std::setw(15)<< (spec_peak_mass - (seq_peak_mass - bgn_seq_peak_mass)) / mng_ptr_->convert_ratio
                               //                    <<"\t"<<seq_peak_mass - bgn_seq_peak_mass - (seq_peak_mass - mod_mass)
                               //                    <<"\t"<<b->bucket_position
                               <<std::endl;
                    }else if(b->seq_peak_vec_[p] >= mods_list[j + 1].left_){
                        break;
                    }
                }
                outFile<<"mods: ";
                for(int m = 0 ; m < mng_ptr_->MODS_VEC.size(); m++){
                    if(mods_list[j + 1].mods_vec_[m]!=0) {
                        outFile << "<" << mods_list[j + 1].mods_vec_[m] << ", " << mng_ptr_->MODS_VEC[m] << ">";
                    }
//                    mod_mass = mod_mass + mods_list[j+1].mods_vec_[m] * mng_ptr_->MODS_MASS_VEC[m];
                }
                mod_mass = mod_mass + mods_list[j+1].mods_mass;
                //mod_mass = mod_mass + bucket_list[idx_path[j + 1]]->bucket_position - b->bucket_position;
                outFile<<"\t("<<mods_list[j+1].left_ + mng_ptr_->start_pos<<","<<mods_list[j+1].right_ + mng_ptr_->start_pos <<")"<<std::endl;


            }else{

                for (int p = 0; p < b->seq_peak_vec_.size(); p++) {

                    mng_ptr_->end_pos = b->seq_peak_vec_.back() + mng_ptr_->start_pos - 1;
//                    double prot_mass_for_e_value = mng_ptr_->seq_peak_mass_list[b->seq_peak_vec_.back()] - bgn_seq_peak_mass;
//                    mng_ptr_->prot_mass_for_e_value_ = prot_mass_for_e_value;

                    if (b->seq_peak_vec_[p] >= mods_list[j].right_) {
                        int seq_peak_mass = mng_ptr_->seq_peak_mass_list[b->seq_peak_vec_[p]] + mod_mass;
                        int spec_peak_mass = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getIntPosition();
                        double spec_peak_inten = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getIntensity();
                        double spec_peak_double_mass = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getMonoMass();
                        std::string type = mng_ptr_->peak_vec_[b->spec_peak_vec_[p]]->getBaseTypePtr()->getName();
                        outFile << std::setw(15)<<b->seq_peak_vec_[p] + mng_ptr_->start_pos<< std::setw(15) << b->spec_peak_vec_[p] << std::setw(15)
//                                <<std::setw(15)<< spec_peak_inten
                                <<std::setw(15)<< spec_peak_double_mass
                                <<std::setw(15)<< type
                                //<<"\t"<< spec_peak_mass - bin_shift
                                <<std::setprecision(3)<<std::setw(15)<< (spec_peak_mass - (seq_peak_mass - bgn_seq_peak_mass)) / mng_ptr_->convert_ratio
                                //                            <<"\t"<<seq_peak_mass - bgn_seq_peak_mass - (seq_peak_mass - mod_mass)
                                //                            <<"\t"<<b->bucket_position
                                <<std::endl;
                    }
                }
            }
        }

        outFile.close();
    }



    int LCSFilter::getTotalShifting(std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, ResSeqPtr & seq, ProtDataPtr & prot_data, PrmPeakPtrVec & peak_vec){
        std::vector<ModsFound> mods_list;
        int max_score = 0;
        ModsFound last_mods;
        for(int s = 0; s < dp_array.back().size(); s++){
            if(dp_array.back()[s].local_score_ > max_score){
                max_score = dp_array.back()[s].local_score_;
                last_mods = dp_array.back()[s];
            }
        }
        ModsFound mods = last_mods;
        for(int i = path.size() - 1; i >= 0; i--) {
            mods_list.insert(mods_list.begin(), mods);
            int k = mods.pre_mods_;
            if (i > 0) {
                mods = dp_array[i - 1][k];
            }
        }

        int abs_shifting =0;
        std::vector<int> seq_peak_mass_list = prot_data->prot_peak_mass_list_;

        int mod_mass = 0;
        int bgn_seq_peak_mass = 0;
        BucketPtr first_bin = path[0];
        int bin_shift = first_bin->bucket_position - first_bin->peak_ptr_vec_[0]->getIntPosAfterMove();
        for(int j = 0; j < path.size(); j++){
            BucketPtr b = path[j];
            if(j != path.size() - 1) {
                for (int p = 0; p < b->seq_peak_vec_.size(); p++) {
                    if (b->seq_peak_vec_[p] >= mods_list[j].right_ && b->seq_peak_vec_[p] <= mods_list[j + 1].left_){
                        if(j == 0 && bgn_seq_peak_mass == -1){
                            bgn_seq_peak_mass =seq_peak_mass_list[b->seq_peak_vec_[p]];
                            bin_shift = b->bucket_position - (0 - bgn_seq_peak_mass);
                        }
                        int seq_peak_mass = seq_peak_mass_list[b->seq_peak_vec_[p]] + mod_mass;
                        int spec_peak_mass = peak_vec[b->spec_peak_vec_[p]]->getIntPosition();
                        abs_shifting = abs_shifting + abs(spec_peak_mass - seq_peak_mass);

                    }else if(b->seq_peak_vec_[p] >= mods_list[j + 1].left_){
                        break;
                    }
                }

                mod_mass = mod_mass + mods_list[j+1].mods_mass;
//                for(int m = 0 ; m < mng_ptr_->MODS_VEC.size(); m++){
//                    mod_mass = mod_mass + mods_list[j+1].mods_vec_[m] * mng_ptr_->MODS_MASS_VEC[m];
//                }

            }else{
                for (int p = 0; p < b->seq_peak_vec_.size(); p++) {
                    if (b->seq_peak_vec_[p] >= mods_list[j].right_) {
                        int seq_peak_mass = seq_peak_mass_list[b->seq_peak_vec_[p]] + mod_mass;
                        int spec_peak_mass = peak_vec[b->spec_peak_vec_[p]]->getIntPosition();
                        abs_shifting = abs_shifting + abs(spec_peak_mass - seq_peak_mass);

                    }
                }
            }
        }
    return abs_shifting;
    }


    int LCSFilter::getScore_2(std::vector<PrmPeakPtrVec>& lists_for_res, std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
                              ResSeqPtr & seq, std::vector<std::pair<int,int>> & mods_pos_list, std::vector<std::pair<int,std::vector<int>>> & modlist_for_pos){
        clock_t start,end1,end2;
        start = clock();
        std::string seq_string = seq->toAcidString();
        //std::vector<int> mods_mass_list = mng_ptr_->mods_mass_list_;

        clock_t merge_s = clock();
        PrmPeakPtrVec merge_list;
        for(int i = 0; i < lists_for_res.size(); i++){
            for(int j = 0; j < lists_for_res[i].size(); j++){
                merge_list.push_back(lists_for_res[i][j]);
            }
        }

        sort(merge_list.begin(), merge_list.end(), peak_posAfterMove_cmp);

        //PrmPeakPtrVec merge_list = getSortedMergeList_pos(lists_for_res);
        clock_t merge_e = clock();
        //std::cout<< "merge list size:"<<merge_list.size()<<std::endl;
        //std::cout<<"Step4_merge and sort "<< std::fixed<<std::setprecision(4)<<(double)(merge_e - merge_s)/CLOCKS_PER_SEC<<"s"<<std::endl;
        //std::cout<<merge_list.size()<<std::endl;
        std::vector<BucketPtr> bgn_bin_list;
        //std::vector<BucketPtr> bgn_bin_list = getBgnBins(merge_list, seq->getLen() + 1, spec_peak_num);
        for(int i = 0; i < mng_ptr_->bgn_bin_pos_list_.size(); i ++){
            BucketPtr bgn_bin = getBinAtPos(merge_list, mng_ptr_->bgn_bin_pos_list_[i], mng_ptr_->use_fixed_tol);
            if(bgn_bin != NULL){
                bgn_bin_list.push_back(bgn_bin);
            }
        }
        end1 = clock();
        clock_t compute_s = clock();
        SearchSet ss = SearchSet(seq_string, merge_list, mods_pos_list, T_mass_comb);
        ss.modlist_for_pos_ = modlist_for_pos;
        ss.max_score = 0;
        std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> puv_map;
        int max_tree_size = 0;
        ss.max_score_for_each_bgn_nodes.clear();
        for(int i = 0; i < bgn_bin_list.size(); i ++){
            ss.tree_size_ = 0;
            ss.max_score_for_cur_bgn = 0;
            std::vector<std::vector<ModsFound>> dp_array;
            ModsFound mods;
            std::vector<int> bgn_mods_vec(mng_ptr_->MODS_VEC.size());
            BucketPtr bgn_bin = bgn_bin_list[i];
            mods.local_score_ = bgn_bin->self_score;
            dp_array.push_back({mods});
            std::vector<BucketPtr> path;
            int total_mods_num = 0;
            int total_iso_num = 0;
            int iso_sum = 0;
//            if(mng_ptr_->forAntibody_){
//                SearchPaths_Antibody(bgn_bin, 0, 0, bgn_mods_vec, total_mods_num, path, dp_array, ss, puv_map);
//
//            }else {
                SearchPaths_1(bgn_bin, 0, 0, 0, 0, bgn_mods_vec, total_mods_num, iso_sum, total_iso_num, path, dp_array, ss, puv_map);
//            }
            ss.max_score_for_each_bgn_nodes.push_back(ss.max_score_for_cur_bgn);
//        std::cout<<ss.max_score_for_cur_bgn<<std::endl;
//        if(ss.tree_size_ > max_tree_size){
//            max_tree_size = ss.tree_size_;
//        }
            // std::cout<<i<<"/"<<bgn_bin_list.size() - 1<<"finished, max: "<< ss.max_score <<std::endl;
        }
        //std::cout<<"max tree size: "<<max_tree_size<<std::endl;
        if(!ss.path_list.empty()) {
            best_ss_path = ss.best_path;
            best_ss_dp = ss.best_dp;
            //backtrack_1(ss.best_path, ss.best_dp, seq);
            //backtrack_2(ss.best_path, ss.best_dp, seq);
        }
        max_score_diff_bgn = ss.max_score_for_each_bgn_nodes;
        end2 = clock();
        clock_t compute_e = clock();
        //std::cout<<"compute time: "<<double(compute_e - compute_s)/CLOCKS_PER_SEC<<std::endl;
        //std::cout<<"number of puv: "<<puv_map.size()<<std::endl;

        return ss.max_score;
    }


int LCSFilter::getScore_1(std::vector<PrmPeakPtrVec>& lists_for_res, std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
                          ResSeqPtr & seq, std::vector<std::pair<int,int>> & mods_pos_list){
    clock_t start,end1,end2;
    start = clock();
    std::string seq_string = seq->toAcidString();
    //std::vector<int> mods_mass_list = mng_ptr_->mods_mass_list_;

    clock_t merge_s = clock();
    PrmPeakPtrVec merge_list;
    for(int i = 0; i < lists_for_res.size(); i++){
        for(int j = 0; j < lists_for_res[i].size(); j++){
            merge_list.push_back(lists_for_res[i][j]);
        }
    }

    sort(merge_list.begin(), merge_list.end(), peak_posAfterMove_cmp);

    //PrmPeakPtrVec merge_list = getSortedMergeList_pos(lists_for_res);
    clock_t merge_e = clock();
    //std::cout<< "merge list size:"<<merge_list.size()<<std::endl;
    //std::cout<<"Step4_merge and sort "<< std::fixed<<std::setprecision(4)<<(double)(merge_e - merge_s)/CLOCKS_PER_SEC<<"s"<<std::endl;
    //std::cout<<merge_list.size()<<std::endl;
    std::vector<BucketPtr> bgn_bin_list;
    //std::vector<BucketPtr> bgn_bin_list = getBgnBins(merge_list, seq->getLen() + 1, spec_peak_num);
    for(int i = 0; i < mng_ptr_->bgn_bin_pos_list_.size(); i ++){
        BucketPtr bgn_bin = getBinAtPos(merge_list, mng_ptr_->bgn_bin_pos_list_[i], mng_ptr_->use_fixed_tol);
        if(bgn_bin != NULL){
            bgn_bin_list.push_back(bgn_bin);
        }
    }
    end1 = clock();
    clock_t compute_s = clock();
    SearchSet ss = SearchSet(seq_string, merge_list, mods_pos_list, T_mass_comb);
    ss.max_score = 0;
    std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> puv_map;
    int max_tree_size = 0;
    ss.max_score_for_each_bgn_nodes.clear();
    for(int i = 0; i < bgn_bin_list.size(); i ++){
        ss.tree_size_ = 0;
        ss.max_score_for_cur_bgn = 0;
        std::vector<std::vector<ModsFound>> dp_array;
        ModsFound mods;
        std::vector<int> bgn_mods_vec(mng_ptr_->MODS_VEC.size());
        BucketPtr bgn_bin = bgn_bin_list[i];
        mods.local_score_ = bgn_bin->self_score;
        dp_array.push_back({mods});
        std::vector<BucketPtr> path;
        int total_mods_num = 0;
        int total_iso_num = 0;
        int iso_sum = 0;
//        if(mng_ptr_->forAntibody_){
//            SearchPaths_Antibody(bgn_bin, 0, 0, bgn_mods_vec, total_mods_num, path, dp_array, ss, puv_map);

//        }else {
            SearchPaths_1(bgn_bin, 0, 0, 0, 0, bgn_mods_vec, total_mods_num, iso_sum, total_iso_num, path, dp_array, ss, puv_map);
//        }
        ss.max_score_for_each_bgn_nodes.push_back(ss.max_score_for_cur_bgn);
//        std::cout<<ss.max_score_for_cur_bgn<<std::endl;
//        if(ss.tree_size_ > max_tree_size){
//            max_tree_size = ss.tree_size_;
//        }
       // std::cout<<i<<"/"<<bgn_bin_list.size() - 1<<"finished, max: "<< ss.max_score <<std::endl;
    }
    //std::cout<<"max tree size: "<<max_tree_size<<std::endl;
    if(!ss.path_list.empty()) {
        best_ss_path = ss.best_path;
        best_ss_dp = ss.best_dp;
        //backtrack_1(ss.best_path, ss.best_dp, seq);
        //backtrack_2(ss.best_path, ss.best_dp, seq);
    }
    max_score_diff_bgn = ss.max_score_for_each_bgn_nodes;
    end2 = clock();
    clock_t compute_e = clock();
    //std::cout<<"compute time: "<<double(compute_e - compute_s)/CLOCKS_PER_SEC<<std::endl;
    //std::cout<<"number of puv: "<<puv_map.size()<<std::endl;

    return ss.max_score;
}

    static bool peak_id_cmp(PrmPeakPtr &p1, PrmPeakPtr &p2) {
        return p1->getPeakId() < p2->getPeakId();
    }

    static bool peak_id_equal(PrmPeakPtr &p1, PrmPeakPtr &p2){
        return p1->getPeakId() == p2->getPeakId();
    }



    int LCSFilter::Compute(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr,  DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, int prot_idx){
        int k = mng_ptr_->K;
        clock_t start,end;
        start = clock();
        //int spec_num = prm_spec_vec.size();

        //std::vector<std::vector<int>> follow_mass_table(proteo_ptr->getLen() + 1);
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();

        mng_ptr_->seq_peak_mass_list = prot_data_ptr->prot_peak_mass_list_;

        mng_ptr_->max_mass_ = prot_data_ptr->max_mass_;
        mng_ptr_->min_mass_ = 15643 - 27;
        std::unordered_map<int, std::vector<LCSFilter::massNodePtr>> merge_follow_mass;
        merge_follow_mass.reserve(proteo_ptr->getLen() * k);


        mng_ptr_->peak_vec_ = peak_vec;

        std::vector<PrmPeakPtrVec> lists_diff_pos(proteo_ptr->getLen() + 1);
        std::vector<int> bgn_bin_pos_list;
        for (int i = 0; i < lists_diff_pos.size(); i++){
            if(mng_ptr_->seq_peak_mass_list.back() - mng_ptr_->seq_peak_mass_list[i] >=
               peak_vec.back()->getIntPosition() - 2 * mng_ptr_->fix_tol_) {
                PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec[0]);
                new_peak->setNode(i);
//                new_peak->setSpecPeakVec({0, 0, 0});
//                new_peak->setProtPeakVec({i, i, i});
                lists_diff_pos[i].push_back(new_peak);
                bgn_bin_pos_list.push_back(0 - mng_ptr_->seq_peak_mass_list[i]);
            }
            else{
                //std::cout<<"i"<<i<<std::endl;
                break;
            }
        }
        mng_ptr_->bgn_bin_pos_list_ = bgn_bin_pos_list;
        //std::cout<<"spectrummass: "<<peak_vec.back()->getIntPosition()<<std::endl;
        clock_t step3_s = clock();

        DistVec dist_vec_ = prot_data_ptr->dist_vec_;
        size_t prot_idx_min = 0, prot_idx_max = 0;
        int tole =  mng_ptr_->delta;
        for (size_t spec_idx = 0; spec_idx < sp_dist_vec.size(); spec_idx++) {
            Dist distance = sp_dist_vec[spec_idx];
            int sp_dist = distance.dist_;

            bool flag = true;
            while (prot_idx_min < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_min].dist_ >= sp_dist - tole) {
                    flag = false;
                } else {
                    prot_idx_min++;
                }
            }

            if (prot_idx_min >= dist_vec_.size()) continue;

            prot_idx_max = std::max(prot_idx_min, prot_idx_max);

            flag = true;

            while (prot_idx_max < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_max].dist_ > sp_dist + tole) {
                    flag = false;
                } else {
                    prot_idx_max++;
                }
            }

            for (size_t t = prot_idx_min; t < prot_idx_max; t++) {
                Dist new_distance = dist_vec_[t];
                std::vector<std::pair<int, int>> & sp_pair_ij = distance.pair_ij_;
                std::vector<std::pair<int, int>> & pg_pair_ij = new_distance.pair_ij_;
                if (std::abs(sp_dist - new_distance.dist_) <= tole){
                    for (size_t k = 0; k < pg_pair_ij.size(); k++) {
                        for (size_t sp = 0; sp < sp_pair_ij.size(); sp++) {
                            int pr_v1 = pg_pair_ij[k].first;
                            int pr_v2 = pg_pair_ij[k].second;
                            int sp_v1 = sp_pair_ij[sp].first;
                            int sp_v2 = sp_pair_ij[sp].second;
                            PrmPeakPtr new_peak_1 = std::make_shared<PrmPeak>(peak_vec[sp_v1]);
                            new_peak_1->setNode(pr_v1);
                            lists_diff_pos[pr_v1].push_back(new_peak_1);

                            PrmPeakPtr new_peak_2 = std::make_shared<PrmPeak>(peak_vec[sp_v2]);
                            new_peak_2->setNode(pr_v2);
                            lists_diff_pos[pr_v2].push_back(new_peak_2);
                        }
                    }
                }

            }
        }



        clock_t step3_e = clock();
        std::cout<<"======step3 time: "<<double(step3_e - step3_s)/CLOCKS_PER_SEC<<std::endl;


        for(int i = 0; i < lists_diff_pos.size(); i++){
            sort(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_cmp);
            lists_diff_pos[i].erase(std::unique(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_equal), lists_diff_pos[i].end());
        }


        for (int i = lists_diff_pos.size() - 1; i >= 0; i--){
            if(mng_ptr_->seq_peak_mass_list[i] >=
               peak_vec.back()->getIntPosition() - 2 * mng_ptr_->fix_tol_) {
                PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec.back());
                int backpeak = peak_vec.size() - 1;
                new_peak->setNode(i);
//                new_peak->setSpecPeakVec({backpeak, backpeak, backpeak});
//                new_peak->setProtPeakVec({i, i, i});
                lists_diff_pos[i].push_back(new_peak);
            }
            else{
                //std::cout<<"i"<<i<<std::endl;
                break;
            }
        }
        clock_t step4_1_s = clock();
        Shifting(lists_diff_pos, proteo_ptr);
        clock_t step4_1_e = clock();

//        clock_t step4_2_s = clock();
//
//        clock_t step4_2_e = clock();
        //std::cout<<"step4_shifting time: "<<double(step4_1_e - step4_1_s)/CLOCKS_PER_SEC<<std::endl;

        //std::vector<LCSFilter::BucketPtr>bucket_list_more_res = getBuckets_1(lists_diff_pos, proteo_ptr->getLen() + 1, peak_vec.size());
        end = clock();
        //std::cout<<"before handle bucket: "<<double(end - start)/CLOCKS_PER_SEC<<"s"<<std::endl;
        //std::cout<<"bucket num: "<<bucket_list_more_res.size()<<std::endl;
        //clock_t compute_s = clock();
        int max_comb_score = getScore_1(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_);

        //int max_comb_score = getScore(bucket_list_more_res, mng_ptr_->mod_mass_table_, seq, mods_pos_list);
        //prm_spec_vec[j]->InsertCandProt(prot_idx, max_comb_score);
        //}
        //clock_t compute_e = clock();
        //std::cout<<"compute time: "<<double(compute_e - compute_s)/CLOCKS_PER_SEC<<std::endl;
        return max_comb_score;
    }

    int LCSFilter::Compute(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr,  DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes, int prot_idx){
        int k = mng_ptr_->K;
        clock_t start,end;
        start = clock();
        //int spec_num = prm_spec_vec.size();

        //std::vector<std::vector<int>> follow_mass_table(proteo_ptr->getLen() + 1);
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();

        mng_ptr_->seq_peak_mass_list = prot_data_ptr->prot_peak_mass_list_;

        mng_ptr_->max_mass_ = prot_data_ptr->max_mass_;
        mng_ptr_->min_mass_ = 15643 - 27;
        std::unordered_map<int, std::vector<LCSFilter::massNodePtr>> merge_follow_mass;
        merge_follow_mass.reserve(proteo_ptr->getLen() * k);


        mng_ptr_->peak_vec_ = peak_vec;

        std::vector<PrmPeakPtrVec> lists_diff_pos(proteo_ptr->getLen() + 1);
        std::vector<int> bgn_bin_pos_list;
        for (int i = 0; i < bgn_nodes.size(); i++){
            int bgn_node_idx = bgn_nodes[i];
            PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec[0]);
            new_peak->setNode(bgn_node_idx);
            lists_diff_pos[bgn_node_idx].push_back(new_peak);
            bgn_bin_pos_list.push_back(0 - mng_ptr_->seq_peak_mass_list[bgn_node_idx]);
        }


//        for (int i = 0; i < lists_diff_pos.size(); i++){
//            if(mng_ptr_->seq_peak_mass_list.back() - mng_ptr_->seq_peak_mass_list[i] >=
//               peak_vec.back()->getIntPosition() - 2 * mng_ptr_->fix_tol_) {
//                PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec[0]);
//                new_peak->setNode(i);
////                new_peak->setSpecPeakVec({0, 0, 0});
////                new_peak->setProtPeakVec({i, i, i});
//                lists_diff_pos[i].push_back(new_peak);
//                bgn_bin_pos_list.push_back(0 - mng_ptr_->seq_peak_mass_list[i]);
//            }
//            else{
//                //std::cout<<"i"<<i<<std::endl;
//                break;
//            }
//        }

        mng_ptr_->bgn_bin_pos_list_ = bgn_bin_pos_list;
        //std::cout<<"spectrummass: "<<peak_vec.back()->getIntPosition()<<std::endl;
        clock_t step3_s = clock();

        DistVec dist_vec_ = prot_data_ptr->dist_vec_;
        size_t prot_idx_min = 0, prot_idx_max = 0;
        int tole = mng_ptr_->delta;
        for (size_t spec_idx = 0; spec_idx < sp_dist_vec.size(); spec_idx++) {
            Dist distance = sp_dist_vec[spec_idx];
            int sp_dist = distance.dist_;

            bool flag = true;
            while (prot_idx_min < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_min].dist_ >= sp_dist - tole) {
                    flag = false;
                } else {
                    prot_idx_min++;
                }
            }

            if (prot_idx_min >= dist_vec_.size()) continue;

            prot_idx_max = std::max(prot_idx_min, prot_idx_max);

            flag = true;

            while (prot_idx_max < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_max].dist_ > sp_dist + tole) {
                    flag = false;
                } else {
                    prot_idx_max++;
                }
            }

            for (size_t t = prot_idx_min; t < prot_idx_max; t++) {
                Dist new_distance = dist_vec_[t];
                std::vector<std::pair<int, int>> & sp_pair_ij = distance.pair_ij_;
                std::vector<std::pair<int, int>> & pg_pair_ij = new_distance.pair_ij_;
                if (std::abs(sp_dist - new_distance.dist_) <= tole){
                    for (size_t k = 0; k < pg_pair_ij.size(); k++) {
                        for (size_t sp = 0; sp < sp_pair_ij.size(); sp++) {
                            int pr_v1 = pg_pair_ij[k].first;
                            int pr_v2 = pg_pair_ij[k].second;
                            int sp_v1 = sp_pair_ij[sp].first;
                            int sp_v2 = sp_pair_ij[sp].second;
                            PrmPeakPtr new_peak_1 = std::make_shared<PrmPeak>(peak_vec[sp_v1]);
                            new_peak_1->setNode(pr_v1);
                            lists_diff_pos[pr_v1].push_back(new_peak_1);

                            PrmPeakPtr new_peak_2 = std::make_shared<PrmPeak>(peak_vec[sp_v2]);
                            new_peak_2->setNode(pr_v2);
                            lists_diff_pos[pr_v2].push_back(new_peak_2);
                        }
                    }
                }

            }
        }



        clock_t step3_e = clock();
//       std::cout<<"step3 time: "<<double(step3_e - step3_s)/CLOCKS_PER_SEC<<std::endl;
//        std::cout<<"peak num: "<<peak_vec.size()<<std::endl;

        for(int i = 0; i < lists_diff_pos.size(); i++){
            sort(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_cmp);
            lists_diff_pos[i].erase(std::unique(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_equal), lists_diff_pos[i].end());
        }


        for (int i = lists_diff_pos.size() - 1; i >= 0; i--){
            if(mng_ptr_->seq_peak_mass_list[i] >=
               peak_vec.back()->getIntPosition() - 2 * mng_ptr_->fix_tol_) {
                PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec.back());
                int backpeak = peak_vec.size() - 1;
                new_peak->setNode(i);
//                new_peak->setSpecPeakVec({backpeak, backpeak, backpeak});
//                new_peak->setProtPeakVec({i, i, i});
                lists_diff_pos[i].push_back(new_peak);
            }
            else{
                //std::cout<<"i"<<i<<std::endl;
                break;
            }
        }
//        clock_t step4_1_s = clock();
        Shifting(lists_diff_pos, proteo_ptr);
//        clock_t step4_1_e = clock();

//        clock_t step4_2_s = clock();
//
//        clock_t step4_2_e = clock();
//        std::cout<<"step4_shifting time: "<<double(step4_1_e - step4_1_s)/CLOCKS_PER_SEC<<std::endl;

        //std::vector<LCSFilter::BucketPtr>bucket_list_more_res = getBuckets_1(lists_diff_pos, proteo_ptr->getLen() + 1, peak_vec.size());
//        end = clock();
        //std::cout<<"before handle bucket: "<<double(end - start)/CLOCKS_PER_SEC<<"s"<<std::endl;
        //std::cout<<"bucket num: "<<bucket_list_more_res.size()<<std::endl;
//        clock_t compute_s = clock();
        int max_comb_score = getScore_1(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_);

        //int max_comb_score = getScore(bucket_list_more_res, mng_ptr_->mod_mass_table_, seq, mods_pos_list);
        //prm_spec_vec[j]->InsertCandProt(prot_idx, max_comb_score);
        //}
        clock_t compute_e = clock();
//        std::cout<<"compute time: "<<double(compute_e - compute_s)/CLOCKS_PER_SEC<<std::endl;
        return max_comb_score;
    }


    int LCSFilter::Compute(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr,  DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes){
        int k = mng_ptr_->K;
        best_ss_path.clear();
        best_ss_dp.clear();
        //std::cout<<"K"<< mng_ptr_->K<< std::endl;
        clock_t start,end;
        start = clock();
        //int spec_num = prm_spec_vec.size();

        //std::vector<std::vector<int>> follow_mass_table(proteo_ptr->getLen() + 1);
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();

        mng_ptr_->seq_peak_mass_list = prot_data_ptr->prot_peak_mass_list_;

        //mng_ptr_->max_mass_ = prot_data_ptr->max_mass_;
        mng_ptr_->min_mass_ = 15643 - 27;
//        std::unordered_map<int, std::vector<LCSFilter::massNodePtr>> merge_follow_mass;
//        merge_follow_mass.reserve(proteo_ptr->getLen() * k);


        mng_ptr_->peak_vec_ = peak_vec;

        std::vector<PrmPeakPtrVec> lists_diff_pos(proteo_ptr->getLen() + 1);
        std::vector<int> bgn_bin_pos_list;
        for (int i = 0; i < bgn_nodes.size(); i++){
            int bgn_node_idx = bgn_nodes[i];
            PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec[0]);
            new_peak->setNode(bgn_node_idx);
            lists_diff_pos[bgn_node_idx].push_back(new_peak);
            bgn_bin_pos_list.push_back(0 - mng_ptr_->seq_peak_mass_list[bgn_node_idx]);
        }


        mng_ptr_->bgn_bin_pos_list_ = bgn_bin_pos_list;
        //std::cout<<"spectrummass: "<<peak_vec.back()->getIntPosition()<<std::endl;
        clock_t step3_s = clock();

        DistVec dist_vec_ = prot_data_ptr->dist_vec_;
        size_t prot_idx_min = 0;
        size_t prot_idx_max = 0;
        int tole;
        if(mng_ptr_->mass_filter_use_fixed_tole) {
            tole = mng_ptr_->delta;
        }else{
            tole = mng_ptr_->max_bin_tol_;
        }
        for (size_t spec_idx = 0; spec_idx < sp_dist_vec.size(); spec_idx++) {
            Dist distance = sp_dist_vec[spec_idx];
            int sp_dist = distance.dist_;

            bool flag = true;
            while (prot_idx_min < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_min].dist_ >= sp_dist - tole) {
                    flag = false;
                } else {
                    prot_idx_min++;
                }
            }

            if (prot_idx_min >= dist_vec_.size()) continue;

            prot_idx_max = std::max(prot_idx_min, prot_idx_max);

            flag = true;

            while (prot_idx_max < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_max].dist_ > sp_dist + tole) {
                    flag = false;
                } else {
                    prot_idx_max++;
                }
            }

            for (size_t t = prot_idx_min; t < prot_idx_max; t++) {
                Dist new_distance = dist_vec_[t];
                std::vector<std::pair<int, int>> & sp_pair_ij = distance.pair_ij_;
                std::vector<std::pair<int, int>> & pg_pair_ij = new_distance.pair_ij_;
                if (std::abs(sp_dist - new_distance.dist_) <= tole){
                    for (size_t k = 0; k < pg_pair_ij.size(); k++) {
                        for (size_t sp = 0; sp < sp_pair_ij.size(); sp++) {
                            int pr_v1 = pg_pair_ij[k].first;
                            int pr_v2 = pg_pair_ij[k].second;
                            int sp_v1 = sp_pair_ij[sp].first;
                            int sp_v2 = sp_pair_ij[sp].second;
                            PrmPeakPtr new_peak_1 = std::make_shared<PrmPeak>(peak_vec[sp_v1]);
                            new_peak_1->setNode(pr_v1);
                            lists_diff_pos[pr_v1].push_back(new_peak_1);

                            PrmPeakPtr new_peak_2 = std::make_shared<PrmPeak>(peak_vec[sp_v2]);
                            new_peak_2->setNode(pr_v2);
                            lists_diff_pos[pr_v2].push_back(new_peak_2);
                        }
                    }
                }

            }
        }






        for(int i = 0; i < lists_diff_pos.size(); i++){
            sort(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_cmp);
            lists_diff_pos[i].erase(std::unique(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_equal), lists_diff_pos[i].end());
        }


        for (int i = lists_diff_pos.size() - 1; i >= 0; i--){
            if(mng_ptr_->seq_peak_mass_list[i] >=
               peak_vec.back()->getIntPosition() - 2 * mng_ptr_->fix_tol_) {
                PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec.back());
                int backpeak = peak_vec.size() - 1;
                new_peak->setNode(i);
//                new_peak->setSpecPeakVec({backpeak, backpeak, backpeak});
//                new_peak->setProtPeakVec({i, i, i});
                lists_diff_pos[i].push_back(new_peak);
            }
            else{
                //std::cout<<"i"<<i<<std::endl;
                break;
            }
        }

        clock_t step3_e = clock();
        //std::cout<<"step3 time: "<<std::fixed<<std::setprecision(4)<<double(step3_e - step3_s)/CLOCKS_PER_SEC<<std::endl;


        clock_t step4_1_s = clock();
        Shifting(lists_diff_pos, proteo_ptr);
        clock_t step4_1_e = clock();


        //std::cout<<"step4_shifting time: "<<std::fixed<<std::setprecision(4)<<double(step4_1_e - step4_1_s)/CLOCKS_PER_SEC<<"s"<<std::endl;
//        clock_t step4_2_s = clock();
//
//        clock_t step4_2_e = clock();
        //std::cout<<"step4_shifting time: "<<double(step4_1_e - step4_1_s)/CLOCKS_PER_SEC<<std::endl;

        //std::vector<LCSFilter::BucketPtr>bucket_list_more_res = getBuckets_1(lists_diff_pos, proteo_ptr->getLen() + 1, peak_vec.size());
        end = clock();
        //std::cout<<"before handle bucket: "<<double(end - start)/CLOCKS_PER_SEC<<"s"<<std::endl;
        //std::cout<<"bucket num: "<<bucket_list_more_res.size()<<std::endl;
        //clock_t compute_s = clock();
//        int max_comb_score = getScore_1(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_);
        int max_comb_score = getScore_2(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_, prot_data_ptr->modlist_for_pos_);

        //int max_comb_score = getScore(bucket_list_more_res, mng_ptr_->mod_mass_table_, seq, mods_pos_list);
        //prm_spec_vec[j]->InsertCandProt(prot_idx, max_comb_score);
        //}
        //clock_t compute_e = clock();
        //std::cout<<"compute time: "<<double(compute_e - compute_s)/CLOCKS_PER_SEC<<std::endl;
        return max_comb_score;
    }



    int LCSFilter::Compute_noK(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes){
        int k = mng_ptr_->K;
        best_ss_path.clear();
        best_ss_dp.clear();
        clock_t start,end;
        start = clock();
        //int spec_num = prm_spec_vec.size();

        //std::vector<std::vector<int>> follow_mass_table(proteo_ptr->getLen() + 1);
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();

        mng_ptr_->seq_peak_mass_list = prot_data_ptr->prot_peak_mass_list_;

        //mng_ptr_->max_mass_ = prot_data_ptr->max_mass_;
        mng_ptr_->min_mass_ = 15643 - 27;
        std::unordered_map<int, std::vector<LCSFilter::massNodePtr>> merge_follow_mass;
        //merge_follow_mass.reserve(proteo_ptr->getLen() * k);


        mng_ptr_->peak_vec_ = peak_vec;

        std::vector<PrmPeakPtrVec> lists_diff_pos(proteo_ptr->getLen() + 1);


        std::vector<int> bgn_bin_pos_list;
        for (int i = 0; i < bgn_nodes.size(); i++){
            int bgn_node_idx = bgn_nodes[i];
//            PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec[0]);
//            new_peak->setNode(bgn_node_idx);
//            lists_diff_pos[bgn_node_idx].push_back(new_peak);
            bgn_bin_pos_list.push_back(0 - mng_ptr_->seq_peak_mass_list[bgn_node_idx]);
        }

        mng_ptr_->bgn_bin_pos_list_ = bgn_bin_pos_list;
        //std::cout<<"spectrummass: "<<peak_vec.back()->getIntPosition()<<std::endl;


        clock_t step3_s = clock();
        //std::cout<<"prot ptr: "<<proteo_ptr->getLen()<<std::endl;
        for(int i = 0; i < lists_diff_pos.size(); i++){
            for(int j = 0 ; j < peak_vec.size(); j++){
                PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec[j]);
                new_peak->setNode(i);
                lists_diff_pos[i].push_back(new_peak);
            }
        }
        clock_t step3_e = clock();
        //std::cout<<"step3 time: "<<std::fixed<<std::setprecision(4)<<double(step3_e - step3_s)/CLOCKS_PER_SEC<<std::endl;




        clock_t step4_1_s = clock();
        Shifting(lists_diff_pos, proteo_ptr);
        clock_t step4_1_e = clock();

//        clock_t step4_2_s = clock();
//
//        clock_t step4_2_e = clock();
        //std::cout<<"step4_shifting time: "<<std::fixed<<std::setprecision(4)<<double(step4_1_e - step4_1_s)/CLOCKS_PER_SEC<<"s"<<std::endl;

        //std::vector<LCSFilter::BucketPtr>bucket_list_more_res = getBuckets_1(lists_diff_pos, proteo_ptr->getLen() + 1, peak_vec.size());
        end = clock();

        //std::cout<<"before handle bucket: "<<double(end - start)/CLOCKS_PER_SEC<<"s"<<std::endl;
//        std::cout<<"bucket num: "<<bucket_list_more_res.size()<<std::endl;
        clock_t compute_s = clock();
//        int max_comb_score = getScore_1(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_);
        int max_comb_score = getScore_2(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_, prot_data_ptr->modlist_for_pos_);

        //int max_comb_score = getScore(bucket_list_more_res, mng_ptr_->mod_mass_table_, seq, mods_pos_list);
        //prm_spec_vec[j]->InsertCandProt(prot_idx, max_comb_score);
        //}
        clock_t compute_e = clock();
        //std::cout<<"compute time: "<<double(compute_e - compute_s)/CLOCKS_PER_SEC<<std::endl;
        return max_comb_score;
    }

    void LCSFilter::Filtering(PrmPeakSpecPtr & prm_spec, ProteoformPtr & proteo_ptr, DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes, std::vector<int> & end_nodes, int prot_idx ){
        int k = mng_ptr_->K;
        best_ss_path.clear();
        best_ss_dp.clear();
        clock_t start,end;
        start = clock();
//        std::unordered_map<int, std::vector<int>> mass_table = prot_data_ptr->mass_table_;
//        std::unordered_map<int, std::vector<std::pair<int, int>>> mass_pos_table = prot_data_ptr->mass_pos_table_;
        std::vector<std::vector<int>> follow_mass_table(proteo_ptr->getLen() + 1);
        //int max_mass = 0;
        //int min_mass = 100000;
        ResSeqPtr seq = proteo_ptr->getResSeqPtr();
        //std::vector<std::pair<int,int>> mods_pos_list = prot_data_ptr->mods_pos_list_;
        //std::vector<int> prot_peak_mass_list = prot_data_ptr->prot_peak_mass_list_;

        mng_ptr_->seq_peak_mass_list = prot_data_ptr->prot_peak_mass_list_;

        mng_ptr_->max_mass_ = prot_data_ptr->max_mass_;
        mng_ptr_->min_mass_ = 15643 - 27;
        std::unordered_map<int, std::vector<LCSFilter::massNodePtr>> merge_follow_mass;
        merge_follow_mass.reserve(proteo_ptr->getLen() * k);

        PrmPeakPtrVec peak_vec = prm_spec->getPeakVec();

        mng_ptr_->peak_vec_ = peak_vec;
        std::vector<PrmPeakPtrVec> lists_diff_pos(proteo_ptr->getLen() + 1);
        std::vector<int> bgn_bin_pos_list;
        for (int i = 0; i < bgn_nodes.size(); i++){
            int bgn_node_idx = bgn_nodes[i];
            PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec[0]);
            new_peak->setNode(bgn_node_idx);
            lists_diff_pos[bgn_node_idx].push_back(new_peak);
            bgn_bin_pos_list.push_back(0 - mng_ptr_->seq_peak_mass_list[bgn_node_idx]);
        }

        mng_ptr_->bgn_bin_pos_list_ = bgn_bin_pos_list;

        DistVec dist_vec_ = prot_data_ptr->dist_vec_;
        size_t prot_idx_min = 0, prot_idx_max = 0;
        int tole =  mng_ptr_->delta;
        for (size_t spec_idx = 0; spec_idx < sp_dist_vec.size(); spec_idx++) {
            Dist distance = sp_dist_vec[spec_idx];
            int sp_dist = distance.dist_;

            bool flag = true;
            while (prot_idx_min < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_min].dist_ >= sp_dist - tole) {
                    flag = false;
                } else {
                    prot_idx_min++;
                }
            }

            if (prot_idx_min >= dist_vec_.size()) continue;

            prot_idx_max = std::max(prot_idx_min, prot_idx_max);

            flag = true;

            while (prot_idx_max < dist_vec_.size() && flag) {
                if (dist_vec_[prot_idx_max].dist_ > sp_dist + tole) {
                    flag = false;
                } else {
                    prot_idx_max++;
                }
            }

            for (size_t t = prot_idx_min; t < prot_idx_max; t++) {
                Dist new_distance = dist_vec_[t];
                std::vector<std::pair<int, int>> & sp_pair_ij = distance.pair_ij_;
                std::vector<std::pair<int, int>> & pg_pair_ij = new_distance.pair_ij_;
                if (std::abs(sp_dist - new_distance.dist_) <= tole){
                    for (size_t k = 0; k < pg_pair_ij.size(); k++) {
                        for (size_t sp = 0; sp < sp_pair_ij.size(); sp++) {
                            int pr_v1 = pg_pair_ij[k].first;
                            int pr_v2 = pg_pair_ij[k].second;
                            int sp_v1 = sp_pair_ij[sp].first;
                            int sp_v2 = sp_pair_ij[sp].second;
                            PrmPeakPtr new_peak_1 = std::make_shared<PrmPeak>(peak_vec[sp_v1]);
                            new_peak_1->setNode(pr_v1);
                            lists_diff_pos[pr_v1].push_back(new_peak_1);

                            PrmPeakPtr new_peak_2 = std::make_shared<PrmPeak>(peak_vec[sp_v2]);
                            new_peak_2->setNode(pr_v2);
                            lists_diff_pos[pr_v2].push_back(new_peak_2);
                        }
                    }
                }

            }
        }

        for (int i = 0; i < lists_diff_pos.size(); i++) {
            sort(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_cmp);
            lists_diff_pos[i].erase(std::unique(lists_diff_pos[i].begin(), lists_diff_pos[i].end(), peak_id_equal),
                                    lists_diff_pos[i].end());
        }


        for (int i = lists_diff_pos.size() - 1; i >= 0; i--) {
            if (mng_ptr_->seq_peak_mass_list[i] >=
                peak_vec.back()->getIntPosition() - 2 * mng_ptr_->fix_tol_) {
                PrmPeakPtr new_peak = std::make_shared<PrmPeak>(peak_vec.back());
                int backpeak = peak_vec.size() - 1;
                new_peak->setNode(i);
//                new_peak->setSpecPeakVec({backpeak, backpeak, backpeak});
//                new_peak->setProtPeakVec({i, i, i});
                lists_diff_pos[i].push_back(new_peak);
            } else {
                //std::cout<<"i"<<i<<std::endl;
                break;
            }
        }

        Shifting(lists_diff_pos, proteo_ptr);
        //std::vector<std::pair<std::vector<BucketPtr>,int>> pathlist = getPath_Score_List(lists_diff_pos, mng_ptr_->mod_mass_table_, seq, mods_pos_list);
        //paths_of_subpeakvec.push_back(pathlist);

//        int max_score = getScore_1(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_);
        int max_score = getScore_2(lists_diff_pos, mng_ptr_->T_mass_comb_, seq, prot_data_ptr->mods_pos_list_, prot_data_ptr->modlist_for_pos_);

        end = clock();
        //}
        //}
        //std::cout<<paths_of_subpeakvec.size()<<std::endl;
        //int max_score = getBestConcat(paths_of_subpeakvec, prm_spec_vec[j]->getPeakVec());
        for(int i = 0; i < bgn_nodes.size(); i++){
            if(prm_spec->getCandQ().size() < mng_ptr_->filter_result_num_ ){
                prm_spec->InsertCandProt_direct(proteo_ptr, bgn_nodes[i], end_nodes[i], max_score_diff_bgn[i]);
            }else{
                if(max_score_diff_bgn[i] > get<3>(prm_spec->getCandQ().top())){
                    prm_spec->InsertCandProt_substitude(proteo_ptr, bgn_nodes[i], end_nodes[i], max_score_diff_bgn[i]);
                }
            }
        }
    }

} /* namespace toppic */
