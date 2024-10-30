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

#ifndef TOPPIC_FILTER_MNG_LCS_FILTER_MNG_HPP_
#define TOPPIC_FILTER_MNG_LCS_FILTER_MNG_HPP_

#include <boost/thread.hpp>
#include <map>
#include <unordered_map>

#include "para/prsm_para.hpp"
#include "ms/spec/prm_peak.hpp"


namespace toppic {

class LCSFilterMng {
 public:
    LCSFilterMng(PrsmParaPtr prsm_para_ptr,
                 //std::vector<std::unordered_map<int, std::vector<std::string>>> & mass_tables,
                 //std::vector<std::string> & res_perm,
                 //std::unordered_map<int,int> & mod_mass_table,
                 std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
                 //std::unordered_map<int,std::vector<int>> & mod_mass_table,
                 //std::unordered_map<std::string,int> & three_res_theo_mass,
                 int filtering_result_num,
                 int thread_num,
                 int K,
                 const std::string & output_file_ext,
                 const std::string & residue_mod_file_name = "",
                 int var_num = 0);

    LCSFilterMng(PrsmParaPtr prsm_para_ptr,
            //std::vector<std::unordered_map<int, std::vector<std::string>>> & mass_tables,
            //std::vector<std::string> & res_perm,
            //std::unordered_map<int,int> & mod_mass_table,
                 std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
            //std::unordered_map<std::string,int> & three_res_theo_mass,
                 const std::string & input_file_ext,
                 int K,
                 int thread_num,
                 const std::string & residue_mod_file_name = "",
                 int var_num = 0);



  std::string getIndexFilePara() {return index_file_para_;}


  std::vector<std::unordered_map<int, std::vector<std::string>>> mass_tables_;
   // std::vector<std::string> res_perm_;
  std::vector<std::tuple<int, std::vector<int>, int>> T_mass_comb_;
    std::unordered_map<int,std::vector<int>> mod_mass_table_;
  std::vector<int> mods_mass_list_;
    std::vector<int> mods_num_list_;
    //vec:[K,T,V]

    //std::unordered_map<std::string,int> & three_res_theo_mass_;

    // mass table min and max
    int min_2_mass_ = 31286 - 27;
    int max_2_mass_ = 102096 + 27;

    int min_3_mass_ = 46929 - 27;
    int max_3_mass_ = 153144 + 27;

    int min_4_mass_;
    int max_4_mass_;

    int min_mass_;
    int max_mass_;

    int min_res_mass = 15643;
    int max_res_mass = 51048;

    int min_mod_mass_ = 6882 - 27;
    int max_mod_mass_ = 11535 + 27;

    int max_3_mod_mass_ = 34605 + 27;
    int min_3_mod_mass_ = 20646 - 27;

  double convert_ratio = 274.335215;
  //std::vector<int> MODS_MASS_VEC = {-6882, -8233, -11535};
  std::vector<int> MODS_MASS_VEC = {-4669, -277};


  std::vector<int> CID_offset_b = {-4667,-4912,-7684,-12351,-12626};
  std::vector<int> CID_offset_y = {4667,4912};

  //std::vector<std::string> MODS_VEC = {"K", "T", "V"};
  std::vector<std::string> MODS_VEC = {"Q", "C"};

  int N_mod_num_ = 0;

  //maximal number of residues in a combination taken into account
  int K = 20;
  int alignment_K = -1;
  double filter_drop_prob = (1.0 / 3.0);

  PrsmParaPtr prsm_para_ptr_;

  std::string index_file_para_;

  int min_Dbond_dist_ = 49;

    bool forAntibody_ = false;
    bool use_fixed_tol = true;
    bool whole_protein_only = false;
    bool use_MS6 = false;
    bool use_adjusted_precmass = false;
    bool mass_filter_use_fixed_tole = true;

  int max_proteoform_mass_ = 50000;

 std::string resultpath = "/home/kunyili/Desktop/toppic-suite-main-copy/LCS-align/real/BSI_AB1/FD/";

  // Candidate protein number for each spectrum
  size_t filter_result_num_ = 150;
  size_t pre_filter_result_num_ = 15;
  int db_block_size_ = 500000000;
  int filter_scale_ = 100;
  int thread_num_ = 1;
  int search_cand_num = 200;
  int start_pos;


  std::vector<int> prec_error_vec = {0, -1, 1};


  float max_loss_prob = 0.7;
  float min_loss_prob = 0.5;
  int dense_window_num = 16;



  int min_res_ = 4;
  int max_res_ = 40;
  std::string output_file_ext_;
  std::string input_file_ext_;
  std::string residue_mod_file_name_;

  PrmPeakPtrVec peak_vec_;

  double average_res_mass = 110;

  int var_num_;
  int sp_id;
  std::string proteo_name;
  // for counting spectra
  int n_spec_block_ = 0;
  boost::mutex mutex_;
  std::vector<int> cnts_;
  int delta = 27;
  int fix_tol_ = 1 * delta;
  int bin_tol_ = delta;
  int max_bin_tol_ = round(2 * convert_ratio);

  int max_path_size_ = 5;
  int max_total_mods_num = 5;
  int prec_tole_;
  int tree_size = 0;
  std::vector<int> seq_peak_mass_list;
  std::vector<int> bgn_bin_pos_list_;
  // file name vector
  std::vector<std::string> multi_ptm_file_vec_{"multi_ptm_index"};


    std::unordered_map<std::string, int> indexforRes = {
            {"A", 0},
            {"R", 1},
            {"N", 2},
            {"D", 3},
            {"C", 4},
            {"E", 5},
            {"Q", 6},
            {"G", 7},
            {"H", 8},
            {"I", 9},
            {"L", 10},
            {"K", 11},
            {"M", 12},
            {"F", 13},
            {"P", 14},
            {"S", 15},
            {"T", 16},
            {"U", 17},
            {"W", 18},
            {"Y", 19},
            {"V",20}
    };


};

typedef std::shared_ptr<LCSFilterMng> LCSFilterMngPtr;

}  // namespace toppic

#endif /* LCS_FILTER_MNG_HPP_ */
