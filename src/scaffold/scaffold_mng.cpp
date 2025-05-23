//
// Created by kunyili on 9/20/24.
//

#include "scaffold_mng.hpp"

namespace toppic {
    ScaffoldMng::ScaffoldMng(PrsmParaPtr prsm_para_ptr,
            //std::vector<std::unordered_map<int, std::vector<std::string>>> & mass_tables,
            //std::vector<std::string> & res_perm,
            //std::unordered_map<int,int> & mod_mass_table,
            //std::unordered_map<int,std::vector<int>> & mod_mass_table,
                               std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
            //std::unordered_map<std::string,int> & three_res_theo_mass,
                               int filtering_result_num,
                               int thread_num,
                               int K_value,
                               const std::string & output_file_ext,
                               const std::string & residue_mod_file_name,
                               int var_num):
            prsm_para_ptr_(prsm_para_ptr),
            filter_result_num_(filtering_result_num),
            thread_num_(thread_num),
            output_file_ext_(output_file_ext),
            residue_mod_file_name_(residue_mod_file_name),
            var_num_(var_num),
            K(K_value),
            T_mass_comb_(T_mass_comb){}
    //mass_tables_(mass_tables),
    //res_perm_(res_perm),
    //mod_mass_table_(mod_mass_table){}
    //three_res_theo_mass_(three_res_theo_mass){}

    ScaffoldMng::ScaffoldMng(PrsmParaPtr prsm_para_ptr,
            //std::vector<std::unordered_map<int, std::vector<std::string>>> & mass_tables,
            //std::vector<std::string> & res_perm,
            //std::unordered_map<int,int> & mod_mass_table,
            //std::unordered_map<int,std::vector<int>> & mod_mass_table,
                               std::vector<std::tuple<int, std::vector<int>, int>> & T_mass_comb,
            //std::unordered_map<std::string,int> & three_res_theo_mass,
                               const std::string & input_file_ext,
                               int K_value,
                               int thread_num,
                               const std::string & residue_mod_file_name,
                               int var_num):
            prsm_para_ptr_(prsm_para_ptr),
            K(K_value),
            thread_num_(thread_num),
            input_file_ext_(input_file_ext),
            residue_mod_file_name_(residue_mod_file_name),
            var_num_(var_num),
            //mass_tables_(mass_tables),
            //res_perm_(res_perm),
            T_mass_comb_(T_mass_comb){}
    //mod_mass_table_(mod_mass_table){}
    //three_res_theo_mass_(three_res_theo_mass){}
} // toppic