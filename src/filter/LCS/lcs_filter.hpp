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

#ifndef TOPPIC_FILTER_LCS_LCS_FILTER_H_
#define TOPPIC_FILTER_LCS_LCS_FILTER_H_

#include "seq/proteoform.hpp"
#include "ms/spec/prm_ms.hpp"
#include "prsm/simple_prsm.hpp"
#include "filter/massmatch/mass_match.hpp"
#include "filter/mng/LCS_filter_mng.hpp"
#include "seq/proteoform_factory.hpp"
#include <unordered_map>

#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include "common/util/logger.hpp"
#include "search/graph/dist.hpp"
#include "ms/spec/deconv_ms.hpp"

namespace toppic {

class LCSFilter {
 public:
    static bool peak_posAfterMove_cmp(PrmPeakPtr &p1, PrmPeakPtr &p2) {
        return p1->getIntPosAfterMove() < p2->getIntPosAfterMove();
    }

  LCSFilter(const ProteoformPtrVec &proteo_ptrs, LCSFilterMngPtr mng_ptr, std::string block_str);

  SimplePrsmPtrVec getBestMatch(const PrmMsPtrVec &ms_ptr_vec);


  std::vector<int> FindMassInTable(int mass_dist, std::unordered_map<int, std::vector<int>> & mass_table);




  void Shifting(std::vector<PrmPeakPtrVec> & lists, ProteoformPtr & proteo_ptr);

  PrmPeakPtrVec getSortedMergeList_pos(std::vector<PrmPeakPtrVec>& lists);


  int getProteoformNum(){return proteo_ptrs_.size();}

  ProteoformPtrVec getProteoPtr(){return proteo_ptrs_;}

    struct cand_cmp
    {
        bool operator() (const std::pair<int,int> & p1,const std::pair<int,int> & p2)
        {
            //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
            return p1.second > p2.second;
        }
    };



    struct cand_cmp_1
    {
        bool operator() (const std::pair<ProteoformPtr ,int> & p1,const std::pair<ProteoformPtr ,int> & p2)
        {
            //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
            return p1.second > p2.second;
        }
    };


    struct cand_cmp_2
    {
        bool operator() (const std::tuple<ProteoformPtr ,int, int,int> & p1,const std::tuple<ProteoformPtr , int,int, int> & p2)
        {
            //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
            return get<3>(p1) > get<3>(p2);
        }
    };




  class SpPair{
  public:
      SpPair(int x, int y, int mass_dist):
      left_idx(x),
      right_idx(y),
      mass(mass_dist){}

      int left_idx;
      int right_idx;
      int mass;
  };
  typedef std::shared_ptr<SpPair> SpPairPtr;

    class ProtData{
    public:
        ProtData(std::unordered_map<int, std::vector<int>> mass_table,
                 std::unordered_map<int, std::vector<std::pair<int, int>>> mass_pos_table,
                 std::vector<std::pair<int,int>> mods_pos_list,
                 std::vector<int> prot_peak_mass_list,
                 int max_mass):
                mass_table_(mass_table),
                mass_pos_table_(mass_pos_table),
                mods_pos_list_(mods_pos_list),
                prot_peak_mass_list_(prot_peak_mass_list),
                max_mass_(max_mass){}

        ProtData(DistVec dist_vec,
                 std::vector<std::pair<int,int>> mods_pos_list,
                 std::vector<int> prot_peak_mass_list,
                 int max_mass):
                dist_vec_(dist_vec),
                mods_pos_list_(mods_pos_list),
                prot_peak_mass_list_(prot_peak_mass_list),
                max_mass_(max_mass){}

        std::unordered_map<int, std::vector<int>> mass_table_;
        DistVec dist_vec_;
        std::unordered_map<int, std::vector<std::pair<int, int>>> mass_pos_table_;
        std::vector<std::pair<int,int>> mods_pos_list_;
        std::vector<int> prot_peak_mass_list_;
        std::vector<std::pair<int, std::vector<int>>> modlist_for_pos_;
        int max_mass_;
    };
    typedef std::shared_ptr<ProtData> ProtDataPtr;

  class PrmPeakSpec{
  public:
      PrmPeakSpec(MsHeaderPtr header_ptr, PrmPeakPtrVec peak_vec, int filter_result_num, int sp_num) :
      header_(header_ptr),
      peak_list_(peak_vec),
      filter_result_num_(filter_result_num),
      spectrum_num_(sp_num){}


      PrmPeakPtrVec getPeakVec(){return peak_list_;}
      MsHeaderPtr getHeader(){return header_;}
//      std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, cand_cmp> getCandQ(){return cand_q;}
//      std::priority_queue<std::pair<ProteoformPtr , int>, std::vector<std::pair<ProteoformPtr , int>>, cand_cmp_1> getCandQ(){return cand_q;}
//      std::priority_queue<std::tuple<ProteoformPtr , int,  int>, std::vector<std::tuple<ProteoformPtr , int,  int>>, cand_cmp_2> getCandQ(){return cand_q;}
      std::priority_queue<std::tuple<ProteoformPtr , int, int, int>, std::vector<std::tuple<ProteoformPtr , int, int,  int>>, cand_cmp_2> getCandQ(){return cand_q;}
      int getSpNum(){return spectrum_num_;}

//      void InsertCandProt(int prot_idx, int score){
//          if(cand_q.empty() || cand_q.size() < filter_result_num_){
//              std::pair<int, int> pair = std::make_pair(prot_idx, score);
//              cand_q.push(pair);
//          } else if (score > cand_q.top().second) {
//              cand_q.pop();
//              std::pair<int, int> pair = std::make_pair(prot_idx, score);
//              cand_q.push(pair);
//          }
//      }

//      void InsertCandProt(ProteoformPtr & prot, int score){
//          if(cand_q.empty() || cand_q.size() < filter_result_num_){
//              std::pair<ProteoformPtr , int> pair = std::make_pair(prot, score);
//              cand_q.push(pair);
//          } else if (score > cand_q.top().second) {
//              cand_q.pop();
//              std::pair<ProteoformPtr , int> pair = std::make_pair(prot, score);
//              cand_q.push(pair);
//          }
//      }

        void InsertCandProt(ProteoformPtr prot, int bgn_node, int end_node, int score){
            if(cand_q.empty() || cand_q.size() < filter_result_num_){
                std::tuple<ProteoformPtr , int, int ,int> tuple = std::make_tuple(prot, bgn_node, end_node, score);
                cand_q.push(tuple);
            } else if (score > get<3>(cand_q.top())) {
                cand_q.pop();
                std::tuple<ProteoformPtr , int, int, int> tuple = std::make_tuple(prot, bgn_node, end_node, score);
                cand_q.push(tuple);
            }
        }

        void InsertCandProt_direct(ProteoformPtr prot, int bgn_node, int end_node, int score){
            std::tuple<ProteoformPtr , int, int ,int> tuple = std::make_tuple(prot, bgn_node, end_node, score);
            cand_q.push(tuple);
      }

      void InsertCandProt_substitude(ProteoformPtr prot, int bgn_node, int end_node, int score){
          cand_q.pop();
          std::tuple<ProteoformPtr , int, int, int> tuple = std::make_tuple(prot, bgn_node, end_node, score);
          cand_q.push(tuple);
      }

//      void InsertCandProt(ProteoformPtr prot, int bgn_node, int score){
//          if(cand_q.empty() || cand_q.size() < filter_result_num_){
//              std::tuple<ProteoformPtr , int,  int> tuple = std::make_tuple(prot, bgn_node, score);
//              cand_q.push(tuple);
//          } else if (score > get<2>(cand_q.top())) {
//              cand_q.pop();
//              std::tuple<ProteoformPtr , int,  int> tuple = std::make_tuple(prot, bgn_node, score);
//              cand_q.push(tuple);
//          }
//      }
        void InsertCandtoVec(ProteoformPtr & prot){
          cand_proteo_vec_.push_back(prot);
      }


        DeconvMsPtrVec deconv_ms_ptr_vec_;
      void setBgn_ys(std::vector<int> bgn_ys){bgn_ys_ = bgn_ys;}
      std::vector<int> getBgn_ys(){return bgn_ys_;}
      std::vector<ProteoformPtr> cand_proteo_vec_ = {};
      PrmPeakPtrVec ori_peak_vec_;
        std::vector<int> ori_bgn_ys_;
      std::string spec_name_;
      int spec_id_;
      int offset_;
      int score_;
      std::string best_prot_name_ = "";
  private:
      int spectrum_num_;
      MsHeaderPtr header_;
      std::vector<int> bgn_ys_;

      PrmPeakPtrVec peak_list_;
      int filter_result_num_ = 0;
//      std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, cand_cmp> cand_q;
//      std::priority_queue<std::pair<ProteoformPtr , int>, std::vector<std::pair<ProteoformPtr , int>>, cand_cmp_1> cand_q;
      //std::priority_queue<std::tuple<ProteoformPtr , int,  int>, std::vector<std::tuple<ProteoformPtr , int,  int>>, cand_cmp_2> cand_q;
        std::priority_queue<std::tuple<ProteoformPtr , int, int, int>, std::vector<std::tuple<ProteoformPtr , int, int, int>>, cand_cmp_2> cand_q;
  };
  typedef std::shared_ptr<PrmPeakSpec> PrmPeakSpecPtr;

  class Node{
  public:
      PrmPeakPtr p;
      int arrayIdx;
      int idx;
      Node(PrmPeakPtr & peak, int arrayIdx, int idx) : p(peak), arrayIdx(arrayIdx), idx(idx) {}
  };
  typedef std::shared_ptr<Node> NodePtr;

  class CompareNode_PosAfterMove {
  public:
      bool operator()(NodePtr & n1, NodePtr & n2) {
          return n1->p->getIntPosAfterMove() > n2->p->getIntPosAfterMove();
      }
  };

    class massNode{
    public:
        int mass;
        int mass_i;
        int mass_j;
        int idx;
        massNode(int mass, int mass_i, int mass_j, int idx) : mass(mass), mass_i(mass_i), mass_j(mass_j), idx(idx) {}
    };
    typedef std::shared_ptr<massNode> massNodePtr;
    class CompareMassNode{
    public:
        bool operator()(massNodePtr & n1, massNodePtr & n2) {
            return n1->mass > n2->mass;
        }
    };

    class CompareNode_TdIdx {
    public:
        bool operator()(NodePtr &n1, NodePtr &n2) {
            return n1->p->getPeakId() > n2->p->getPeakId();
        }
    };

    class Interval;
    typedef std::shared_ptr<Interval> IntervalPtr;
    class Interval{
    public:
        Interval(){};

        Interval(IntervalPtr interval_ptr) :
        ends(interval_ptr->ends),
        peak_num_(interval_ptr->peak_num_){}

        std::vector<int> ends;
        //std::vector<int> peaks;
        int peak_num_ = 0;
    };

    class Bucket;
    typedef std::shared_ptr<Bucket> BucketPtr;
  class Bucket{
  public:
      Bucket(PrmPeakPtrVec peak_ptr_vec, int seq_peak_num) :
      peak_ptr_vec_(peak_ptr_vec),
      seq_peak_num_(seq_peak_num){};

      Bucket(PrmPeakPtrVec peak_ptr_vec) :
              peak_ptr_vec_(peak_ptr_vec){};

      Bucket(){};

      static bool cmp(PrmPeakPtr &p1, PrmPeakPtr &p2) {
          return p1->getProtPeakVec()[0] < p2->getProtPeakVec()[0];
      }

      static bool node_cmp(PrmPeakPtr &p1, PrmPeakPtr &p2) {
          return p1->getNode() < p2->getNode();
      }

      static  bool node_equal(PrmPeakPtr &p1, PrmPeakPtr &p2) {
          return p1->getNode() == p2->getNode();
      }

      Bucket(BucketPtr bucket_ptr) :
      peak_ptr_vec_(bucket_ptr->peak_ptr_vec_),
      interval_list_(bucket_ptr->interval_list_),
      seq_peak_num_(bucket_ptr->seq_peak_num_),
      bucket_position(bucket_ptr->bucket_position),
      self_score(bucket_ptr->self_score),
      left_end(bucket_ptr->left_end),
      right_end(bucket_ptr->right_end),
      idx_(bucket_ptr->idx_){}


      void sort_bucket() {
          sort(peak_ptr_vec_.begin(), peak_ptr_vec_.end(), cmp);
          left_end = peak_ptr_vec_[0]->getProtPeakVec()[0];
          //right_end = peak_ptr_vec_.back()->getProtPeakVec()[2];
          if(peak_ptr_vec_[0]->getSpecPeakVec()[0] == 0){
              bgn_bin = true;
          }
      }

      void arrange_bucket(){
          sort(peak_ptr_vec_.begin(), peak_ptr_vec_.end(), node_cmp);
          peak_ptr_vec_.erase(std::unique(peak_ptr_vec_.begin(), peak_ptr_vec_.end(), node_equal), peak_ptr_vec_.end());
          for(int i = 0; i < peak_ptr_vec_.size(); i++){
              seq_peak_vec_.push_back(peak_ptr_vec_[i]->getNode());
              spec_peak_vec_.push_back(peak_ptr_vec_[i]->getPeakId());
          }
          left_end = seq_peak_vec_[0];
          right_end = seq_peak_vec_.back();
          self_score = seq_peak_vec_.size();
          if(spec_peak_vec_[0] == 0){
              bgn_bin = true;
          }
      }

      void sort_bucket(PrmPeakPtrVec & peak_list) {
          sort(peak_list.begin(), peak_list.end(), cmp);
          left_end = peak_list[0]->getProtPeakVec()[0];
          right_end = peak_list.back()->getProtPeakVec()[2];
      }



      void InsertSeqPeak(std::vector<int> protpeakvec, std::vector<int> specpeakvec, std::vector<int> & idx_list, int & score, std::vector<int> & spec_peak_list, int & right_end){
          for(int i = 0; i < 3; i++){
              int idx = protpeakvec[i];
              if(idx_list[idx] == 0){
                  idx_list[idx] = 1;
                  spec_peak_list.push_back(specpeakvec[i]);
                  if(protpeakvec[i] > right_end){
                      right_end = protpeakvec[i];
                  }
                  score = score + 1;
              }
          }
      }

      void checkEnd(int spec_peak_num){
          for(int i = peak_ptr_vec_.size() - 1; i >= 0; i--){
              if(peak_ptr_vec_[i]->getSpecPeakVec()[2] == spec_peak_num - 1){
                  end_bin = true;
              }
          }
      }

      void getSeqPeakVec(){
          int score = 0;
          std::vector<int> seq_peak_vec;
          std::vector<int> spec_peak_vec;
          int right = 0;
          std::vector<int> idx_list(seq_peak_num_, 0);
          for(int i = 0; i < peak_ptr_vec_.size(); i ++){
              InsertSeqPeak(peak_ptr_vec_[i]->getProtPeakVec(), peak_ptr_vec_[i]->getSpecPeakVec(), idx_list, score, spec_peak_vec, right);
          }
          right_end = right;
          for(int j = left_end; j <= right_end; j++){
              if(idx_list[j] > 0){
                  seq_peak_vec.push_back(j);
              }
          }
          self_score = score;
          seq_peak_vec_ = seq_peak_vec;
          std::sort(spec_peak_vec.begin(), spec_peak_vec.end());
          spec_peak_vec_ = spec_peak_vec;
      }



      PrmPeakPtrVec peak_ptr_vec_;
      std::vector<IntervalPtr> interval_list_;
      std::vector<IntervalPtr> gap_list_;
      std::vector<int> seq_peak_vec_;
      std::vector<int> spec_peak_vec_;
      int seq_peak_num_;
      //int seq_idx_list;
      int bucket_position;
      int self_score;
      //int local_max_score;
      //int corr_res_num;
      int left_end;
      int right_end;
      int idx_;
      bool bgn_bin = false;
      bool end_bin = false;
  };






  class Comb;
  typedef std::shared_ptr<Comb> CombPtr;
  class Comb{
  public:
      Comb(int mass, int start_pos, int res_num) :
      mass_(mass),
      start_pos_(start_pos),
      res_num_(res_num){};
  private:
      int mass_;
      int start_pos_;
      int res_num_;
  };






    class ModsFound;
    class ModsFound{
    public:
        ModsFound(){
            left_ = -1;
            right_ = -1;
            local_score_ = 0;
            pre_mods_ = -1;
            mods_mass = 0;
        };
        ModsFound(int left, int right, int local_score, int pre_mods): left_(left), right_(right), local_score_(local_score), pre_mods_(pre_mods){};
        std::vector<int> mods_vec_;
        int left_;
        int right_;
        int local_score_;
        int pre_mods_;
//        int mods_dist_ = 0;
        int mods_mass;
    };



  std::unordered_map<int, std::vector<int>> getSigma_prot(ProteoformPtr & proteo_ptr, int k);
  std::vector<std::vector<std::pair<int,int>>> getSigma_spec(PrmPeakPtrVec & peak_vec, std::unordered_map<int, std::vector<int>> & mass_table);
    std::vector<LCSFilter::massNodePtr> getMergeFollowMasses(int cur_mass, std::unordered_map<int, std::vector<std::pair<int, int>>> & mass_pos_table,
                                                                        std::vector<std::vector<int>> & follow_mass_table);



    typedef struct edgenode{
        int bin_idx;
        int mod_dist;
        //edgenode *next = NULL;
    }edgeNode;

    typedef struct vertexnode{
        int bin_idx;
        //edgenode *firstEdge;
        std::vector<edgeNode> edgelist;
    }vertexNode;



    class SearchProtCand;
    class SearchProtCand{
    public:
        SearchProtCand(ProteoformPtr prot,
                       std::vector<LCSFilter::BucketPtr> path,
                       std::vector<std::vector<LCSFilter::ModsFound>> dp_array,
                       std::vector<int> seq_peak_mass_list):
                       prot_(prot),
                       path_(path),
                       dp_array_(dp_array),
                       mass_list_(seq_peak_mass_list){};

        ProteoformPtr prot_;
        std::vector<LCSFilter::BucketPtr> path_;
        std::vector<std::vector<LCSFilter::ModsFound>> dp_array_;
        std::vector<int> mass_list_;
    };

    struct search_cand_cmp
    {
        bool operator() (const std::pair<int,LCSFilter::SearchProtCand> & p1,const std::pair<int,LCSFilter::SearchProtCand> & p2)
        {
            //if(p1.first.second == p2.first.second) {return p1.second > p2.second;}
            return p1.first > p2.first;
        }
    };

    class SearchSet;
    class SearchSet{
    public:
//        SearchSet(std::string & seq_string,
//                  PrmPeakPtrVec & merge_list,
//                  std::vector<std::pair<int,int>> & mods_pos_list,
//                  std::unordered_map<int,std::vector<int>> & mod_mass_table,
//                  std::vector<int> & mods_mass_list,
//                  int prot_peak_num,
//                  int spec_peak_num):
//        seq_string_(seq_string),
//        merge_list_(merge_list),
//        mods_pos_list_(mods_pos_list),
//        mod_mass_table_(mod_mass_table),
//        mods_mass_list_(mods_mass_list),
//        prot_peak_num_(prot_peak_num),
//        spec_peak_num_(spec_peak_num){};

        SearchSet(std::string & seq_string,
                  PrmPeakPtrVec & merge_list,
                  std::vector<std::pair<int,int>> & mods_pos_list,
                  std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb):
//                  std::unordered_map<int,std::vector<int>> & mod_mass_table,
//                  std::vector<int> & mods_mass_list):
                seq_string_(seq_string),
                merge_list_(merge_list),
                T_mass_comb_(T_mass_comb),
                mods_pos_list_(mods_pos_list){}

//                mod_mass_table_(mod_mass_table),
//                mods_mass_list_(mods_mass_list){};

        std::vector<std::tuple<int, std::vector<int>, int>> T_mass_comb_;
        std::vector<std::vector<BucketPtr>> path_list;
        std::vector<std::pair<std::vector<BucketPtr>,int>> path_score_pair_list;
        std::string  seq_string_;
        std::vector<std::pair<int,int>>  mods_pos_list_;
        std::unordered_map<int,std::vector<int>>  mod_mass_table_;
        std::vector<std::pair<int,std::vector<int>>> modlist_for_pos_;
        std::vector<int> mods_mass_list_;
        PrmPeakPtrVec & merge_list_;
        //int prot_peak_num_ = -1;
        //int spec_peak_num_ = -1;
        int max_score = 0;
        int max_score_for_cur_bgn;
        std::vector<BucketPtr> best_path;
        std::vector<std::vector<ModsFound>> best_dp;
        int tree_size_ = 0;
        std::vector<int> max_score_for_each_bgn_nodes;
    };

    struct pair_hash {
        template<class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2;
        }
    };



    class CandPSM{
    public:
        CandPSM(int score, std::vector<LCSFilter::BucketPtr> path, std::vector<std::vector<LCSFilter::ModsFound>> dp, ResSeqPtr seq, ProteoformPtr proteo) :
        score_(score),
        path_(path),
        dp_(dp),
        seq_(seq),
        proteo_(proteo){};

        int score_;
        std::vector<LCSFilter::BucketPtr> path_;
        std::vector<std::vector<LCSFilter::ModsFound>> dp_;
        ResSeqPtr seq_;
        ProteoformPtr proteo_;
    };
    typedef std::shared_ptr<CandPSM> CandPSMPtr;

    class ComparePSM{
    public:
        bool operator()(CandPSMPtr & psm_1, CandPSMPtr & psm_2) {
            return psm_1->score_ > psm_2->score_;
        }
    };


    BucketPtr getBinAtPos(PrmPeakPtrVec & merge_list, int pos, bool fixed_tol);


    int getScore_1(std::vector<PrmPeakPtrVec>& lists_for_res, std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
                   ResSeqPtr & seq, std::vector<std::pair<int,int>> & mods_pos_list);

    int getScore_2(std::vector<PrmPeakPtrVec>& lists_for_res, std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
                   ResSeqPtr & seq, std::vector<std::pair<int,int>> & mods_pos_list, std::vector<std::pair<int,std::vector<int>>> & modlist_for_pos);


    void backtrack_1(std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, ResSeqPtr & seq);



    void Filtering(PrmPeakSpecPtr & prm_spec, ProteoformPtr & proteo_ptr, DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes, std::vector<int> & end_nodes, int prot_idx);



    void SearchPaths_1(BucketPtr u, int mod_dist, int mod_num, int isotope_value, int isotope_shift_num, std::vector<int> mods_vec,int &total_mods_num, int & iso_sum, int &total_iso_num, std::vector<BucketPtr> &path,
                       std::vector<std::vector<ModsFound>> &dp_array, SearchSet &ss, std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map);

    void SearchPaths_Antibody(BucketPtr u, int mod_dist, int mod_num, std::vector<int> mods_vec, int &total_mods_num, std::vector<BucketPtr> &path,
                       std::vector<std::vector<ModsFound>> &dp_array, SearchSet &ss, std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map);

    int Compute(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr,  DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, int prot_idx);
    int Compute(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr,  DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes, int prot_idx);

    int Compute_noK(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes);
    int Compute(PrmPeakPtrVec & peak_vec, ProteoformPtr & proteo_ptr,  DistVec & sp_dist_vec, ProtDataPtr & prot_data_ptr, std::vector<int> & bgn_nodes);
    void backtrack_2(std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, ResSeqPtr & seq);
    void backtrack_empty();
    int getTotalShifting(std::vector<BucketPtr> & path, std::vector<std::vector<ModsFound>> &dp_array, ResSeqPtr & seq, ProtDataPtr & prot_data, PrmPeakPtrVec & peak_vec);
    void
    HandleOverlap_1(int mod_dist, BucketPtr &pre_bin, BucketPtr &cur_bin, int head, int j,
                    std::vector<std::vector<ModsFound>> &dp_array,
                    std::vector<int> &mods_vec, std::string &seq_string,
                    std::vector<std::pair<int, int>> &mods_pos_list,
                    std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map);


    void HandleOverlap_2(int mod_dist, BucketPtr &pre_bin, BucketPtr &cur_bin, int head, int j,
                    std::vector<std::vector<ModsFound>> &dp_array,
                    std::vector<int> &mods_vec, std::string &seq_string,
                    std::vector<std::pair<int, std::vector<int>>> & modlist_for_pos,
                    std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map);

    void
    HandleNonOverlap_1(int mod_dist, BucketPtr &pre_bin, BucketPtr &cur_bin, int head, int j,
                    std::vector<std::vector<ModsFound>> &dp_array,
                    std::vector<int> &mods_vec, std::string &seq_string,
                    std::vector<std::pair<int, int>> &mods_pos_list,
                       std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map);

    void
    HandleNonOverlap_2(int mod_dist, BucketPtr &pre_bin, BucketPtr &cur_bin, int head, int j,
                       std::vector<std::vector<ModsFound>> &dp_array,
                       std::vector<int> &mods_vec, std::string &seq_string,
                       std::vector<std::pair<int, std::vector<int>>> &modlist_for_pos,
                       std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map);

    void HandleAntibody(int mod_dist, BucketPtr & pre_bin, BucketPtr & cur_bin, int head,
                                   int j,
                                   std::vector<std::vector<ModsFound>> & dp_array,
                                   std::vector<int> & mods_vec,
                                   std::string & seq_string,
                                   std::vector<std::pair<int,int>> & mods_pos_list,
                                   std::unordered_map<std::pair<int,int>, std::vector<ModsFound>, pair_hash> & puv_map);

    std::vector<BucketPtr> best_ss_path;
    std::vector<std::vector<ModsFound>> best_ss_dp;
    std::vector<int> max_score_diff_bgn;

 private:
  LCSFilterMngPtr mng_ptr_;
  const static int filter_result_num = 150;
  ProteoformPtrVec proteo_ptrs_;
  MassMatchPtr index_ptr_;

  SimplePrsmPtrVec compute(const PrmMsPtrVec &ms_ptr_vec);



};

typedef std::shared_ptr<LCSFilter> LCSFilterPtr;
} /* namespace toppic */

#endif
