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

#include "search/LCS/LCS_alignment_mng.hpp"

namespace toppic {



    LCSAlignmentMng::LCSAlignmentMng(PrsmParaPtr prsm_para_ptr,
            //std::vector<std::unordered_map<int, std::vector<std::string>>> & mass_tables,
            //std::vector<std::string> & res_perm,
            //std::unordered_map<int,int> & mod_mass_table,
                               std::unordered_map<int,std::vector<int>> & mod_mass_table,
            //std::unordered_map<std::string,int> & three_res_theo_mass,
                               int K_value,
                               int thread_num,
                               const std::string & input_file_ext,
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
            mod_mass_table_(mod_mass_table){}
    //three_res_theo_mass_(three_res_theo_mass){}



}  // namespace toppic
