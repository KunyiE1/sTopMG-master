//
// Created by kunyili on 9/20/24.
//

#ifndef TOPPIC_SCAFFOLD_MNG_HPP
#define TOPPIC_SCAFFOLD_MNG_HPP

#include <boost/thread.hpp>
#include <map>
#include <unordered_map>

#include "para/prsm_para.hpp"
#include "ms/spec/prm_peak.hpp"

namespace toppic {

    class ScaffoldMng {
    public:
        ScaffoldMng(PrsmParaPtr prsm_para_ptr,
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

        ScaffoldMng(PrsmParaPtr prsm_para_ptr,
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
        std::vector<int> MODS_MASS_VEC = {-6882, -8233, -11535};



        std::vector<int> CID_offset_b = {-4667,-4912,-7684,-12351,-12626};
        std::vector<int> CID_offset_y = {4667,4912};

        std::vector<std::string> MODS_VEC = {"K", "T", "V"};

        //maximal number of residues in a combination taken into account
        int K = 20;

        double filter_drop_prob = (1.0 / 3.0);

        PrsmParaPtr prsm_para_ptr_;

        std::string index_file_para_;

        /** parameters for fast filteration */
        int max_proteoform_mass_ = 50000;

        std::string resultpath = "/home/kunyili/Desktop/toppic-suite-main-copy/LCS-filter+align/real/BSI_AB1/LC/";

        // Candidate protein number for each spectrum
        size_t filter_result_num_ = 150;
        size_t pre_filter_result_num_ = 15;
        int db_block_size_ = 500000000;
        int filter_scale_ = 100;
        int thread_num_ = 1;
        int search_cand_num = 200;
        int start_pos;

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
        int max_path_size_ = 5;
        int max_total_mods_num = 5;
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
    typedef std::shared_ptr<ScaffoldMng> ScaffoldMngPtr;
} // toppic

#endif //TOPPIC_SCAFFOLD_MNG_HPP
