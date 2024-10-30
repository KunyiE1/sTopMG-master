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
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>

#include "common/base/mod_util.hpp"
#include "common/base/base_data.hpp"

#include "common/util/file_util.hpp"
#include "common/util/mem_check.hpp"
#include "common/util/version.hpp"

#include "seq/fasta_reader.hpp"
#include "seq/fasta_util.hpp"

#include "ms/spec/msalign_frac_merge.hpp"
#include "ms/spec/msalign_util.hpp"
#include "ms/spec/deconv_json_merge.hpp"
#include "ms/feature/feature_merge.hpp"

#include "para/prsm_para.hpp"
#include "prsm/prsm_str_merge.hpp"
#include "prsm/prsm_form_filter.hpp"
#include "prsm/prsm_top_selector.hpp"
#include "prsm/prsm_cutoff_selector.hpp"
#include "prsm/prsm_simple_cluster.hpp"
#include "prsm/prsm_feature_cluster.hpp"
#include "prsm/prsm_fdr.hpp"
#include "prsm/prsm_form_filter.hpp"
#include "prsm/prsm_match_table_writer.hpp"
#include "prsm/prsm_util.hpp"
#include "prsm/simple_prsm_reader.hpp"
#include "prsm/simple_prsm_xml_writer.hpp"
#include "prsm/simple_prsm_util.hpp"
#include "prsm/simple_prsm.hpp"
#include "prsm/simple_prsm_str_merge.hpp"

#include "filter/mng/one_ptm_filter_mng.hpp"
#include "filter/mng/index_file_name.hpp"
#include "filter/oneptm/one_ptm_filter_processor.hpp"

#include "filter/mng/var_ptm_filter_mng.hpp"
#include "filter/varptm/var_ptm_filter_processor.hpp"

#include "filter/mng/diag_filter_mng.hpp"
#include "filter/diag/diag_filter_processor.hpp"

#include "filter/mng/LCS_filter_mng.hpp"
#include "filter/LCS/lcs_filter_processor.hpp"
#include "search/LCS/lcs_alignment_processor.hpp"
#include "search/LCS/LCS_alignment_mng.hpp"

#include "search/graph/graph.hpp"
#include "search/graph/proteo_graph.hpp"
#include "search/graphalign/graph_align_mng.hpp"
#include "search/graphalign/graph_align_processor.hpp"
#include "search/graphalign/graph_post_processor.hpp"

#include "common/base/amino_acid_base.hpp"
#include "common/base/ptm_base.hpp"
#include "common/base/residue_base.hpp"

#include "stat/mcmc/mcmc_mng.hpp"
#include "stat/mcmc/mcmc_dpr_processor.hpp"

#include "visual/xml_generator.hpp"
#include "visual/json_transformer.hpp"

#include "console/topmg_argument.hpp"
#include "console/topmg_process.hpp"

namespace toppic {

void copyTopMSV(std::map<std::string, std::string> &arguments) {
  std::string spectrum_file_name = arguments["spectrumFileName"];
  std::string base_name = file_util::basename(spectrum_file_name);
  std::string base_name_short = base_name.substr(0, base_name.length() - 4);
  std::string topmsv_dir = base_name_short + "_html" +  file_util::getFileSeparator() + "topmsv";
  if (file_util::exists(topmsv_dir)) {
    LOG_WARN("The TopMSV directory " << topmsv_dir << " exists!");
    //file_util::delDir(topmsv_dir);
  }
  else{
    if (!file_util::exists(base_name_short + "_html")){//if _html folder was not created before
      file_util::createFolder(base_name_short + "_html");
    }
    std::string resource_dir = arguments["resourceDir"];
    // copy resources 
    std::string from_path(resource_dir + file_util::getFileSeparator() + "topmsv");
    file_util::copyDir(from_path, topmsv_dir);
  }
}

void cleanTopmgDir(const std::string &fa_name, 
                   const std::string & sp_name,
                   bool keep_temp_files) {
  std::string abs_sp_name = file_util::absoluteName(sp_name);
  std::string sp_base = file_util::basename(abs_sp_name); 
  std::replace(sp_base.begin(), sp_base.end(), '\\', '/');

  file_util::delFile(sp_base + "_topmg_proteoform.xml");
  file_util::rename(sp_base + ".topmg_form_cutoff_form", 
                    sp_base + "_topmg_proteoform.xml");

  file_util::delFile(sp_base + "_topmg_prsm_cutoff.xml");
  file_util::rename(sp_base + ".topmg_prsm_cutoff",
                    sp_base + "_topmg_prsm.xml");
  if (!keep_temp_files) {
    file_util::cleanPrefix(sp_name, sp_base + ".msalign_");
    file_util::delFile(abs_sp_name + "_index");
    file_util::delFile(sp_base + ".topmg_one_filter");
    file_util::cleanPrefix(sp_name, sp_base + ".topmg_one_filter_");
    file_util::delFile(sp_base + ".topmg_multi_filter");
    file_util::cleanPrefix(sp_name, sp_base + ".topmg_multi_filter_");
    file_util::delFile(sp_base + ".topmg_graph_filter");
    file_util::cleanPrefix(sp_name, sp_base + ".topmg_graph_filter_");
    file_util::delFile(sp_base + ".topmg_graph_align");
    file_util::cleanPrefix(sp_name, sp_base + ".topmg_graph_align_");
    file_util::delFile(sp_base + ".topmg_graph_post");
    file_util::delFile(sp_base + ".topmg_graph");
    file_util::delFile(sp_base + ".topmg_evalue");
    file_util::cleanPrefix(sp_name, sp_base + ".topmg_evalue_");
    file_util::delFile(sp_base + ".topmg_cluster");
    file_util::delFile(sp_base + ".topmg_cluster_fdr");
    file_util::delFile(sp_base + ".topmg_prsm");
    file_util::delFile(sp_base + ".topmg_form_cutoff");
    file_util::delDir(sp_base + "_topmg_proteoform_cutoff_xml");
    file_util::delDir(sp_base + "_topmg_prsm_cutoff_xml");
  }
}

int TopMG_testModFile(std::map<std::string, std::string> & arguments) {
  try {
    base_data::init();
    LOG_DEBUG("Init base data completed");

    // Test arguments
    PrsmParaPtr prsm_para_ptr = std::make_shared<PrsmPara>(arguments);

    if (arguments["varModFileName"] != "") {
      mod_util::readModTxt(arguments["varModFileName"]);
    }
  } catch (const char* e) {
    std::cout << "[Exception]" << std::endl;
    std::cout << e << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

    static bool tuplecmp(const std::tuple<int, std::vector<int>, int>& a, const std::tuple<int, std::vector<int>, int>& b){
        // 以pair对的第2个数的大小从大往小排序
        return get<0>(a) < get<0>(b);
    }

std::vector<ModPtr> getCombinations(std::vector<ModPtr> & mods_list, std::vector<int> indices){
    std::vector<ModPtr> temp;
    for(int i = 0; i < indices.size(); i++){
        temp.push_back(mods_list[indices[i]]);
    }
    return temp;
}

std::vector<std::vector<ModPtr>> combinations_with_replacement(std::vector<ModPtr> & mods_list, int r){
    std::vector<std::vector<ModPtr>> T;
    int n = mods_list.size();
    if (! n&&r){
        return T;
    }
    std::vector<int> indices(r);
    T.push_back(getCombinations(mods_list, indices));
    while(true){
        int j;
        for(j = r - 1; j >= 0; j--){
            if(indices[j] != n - 1){
                break;
            }
        }
        if(j < 0){
            return T;
        }
        int ind = indices[j] + 1;
        for(int k = j; k < indices.size(); k++){
            indices[k] = ind;
        }
        T.push_back(getCombinations(mods_list, indices));
    }
}

std::unordered_map<int, std::vector<std::string>> GenerateMassTable(std::string file_name){
    std::ifstream in(file_name);
    std::string line;
    std::unordered_map<int, std::vector<std::string>> res_mass_table;
    while(std::getline(in,line)){
        std::stringstream ss(line.substr(0, line.size()-1));
        std::string tmp;
        std::vector<std::string> tmp_vec;
        std::vector<std::string> combs;
        while(std::getline(ss, tmp, '\t')){
            tmp_vec.push_back(tmp);
        }
        int mass_key = stoi(tmp_vec[0]);
        for(int c = 1; c < tmp_vec.size(); c++){
            combs.push_back(tmp_vec[c]);
        }
        res_mass_table.insert(std::make_pair(mass_key,combs));
    }
    return res_mass_table;
}

    std::unordered_map<int, std::vector<int>> GenerateModsVec(std::string file_name){
        std::ifstream in(file_name);
        std::string line;
        std::unordered_map<int, std::vector<int>> res_mass_table;
        while(std::getline(in,line)){
            std::stringstream ss(line.substr(0, line.size()-1));
            std::string tmp;
            std::vector<std::string> tmp_vec;
            std::vector<std::vector<int>> mods_vec_list;

            while(std::getline(ss, tmp, '\t')){
                tmp_vec.push_back(tmp);
            }
            int mass_key = stoi(tmp_vec[0]);
            std::vector<int> mods_vec;
            for(int c = 1; c < tmp_vec.size(); c++){
                std::istringstream iss(tmp_vec[c]);
                std::string token;
                while(getline(iss, token, ',')){
                    mods_vec.push_back(stoi(token));
                }
                //mods_vec_list.push_back(mods_vec);
            }
            res_mass_table.insert(std::make_pair(mass_key,mods_vec));
        }
        return res_mass_table;
    }

    std::vector<std::string> GeneratePermutation(std::string file_name){
        std::ifstream in(file_name);
        std::string line;
        std::vector<std::string> three_res_perm(9261);
        int i = 0;
        while(std::getline(in,line)){
            three_res_perm[i] = line.substr(0,3);
            i++;
        }
        return three_res_perm;
    }

    std::unordered_map<int, int> GenerateModTable(std::string file_name){
        std::ifstream in(file_name);
        std::string line;
        std::unordered_map<int, int> mod_mass_table;
        while(std::getline(in,line)){
            std::stringstream ss(line);
            std::string tmp;
            std::vector<std::string> tmp_vec;
            std::vector<std::string> combs;
            while(std::getline(ss, tmp, '\t')){
                tmp_vec.push_back(tmp);
            }
            int mass_key = stoi(tmp_vec[0]);
            int mod_num = stoi(tmp_vec[1]);
            mod_mass_table.insert(std::make_pair(mass_key,mod_num));
        }
        return mod_mass_table;
    }

int TopMG_identify(std::map<std::string, std::string> & arguments) {
  try {
    std::time_t start = time(nullptr);
    char buf[50];
    std::strftime(buf, 50, "%a %b %d %H:%M:%S %Y", std::localtime(&start));

    arguments["startTime"] = buf;
    TopmgArgument::outputArguments(std::cout, " ", arguments);

    base_data::init();

    LOG_DEBUG("Init base data completed");

    std::string db_file_name = arguments["databaseFileName"];
    std::string sp_file_name = arguments["spectrumFileName"];
    std::string ori_db_file_name = arguments["oriDatabaseFileName"];
    std::string var_mod_file_name = arguments["varModFileName"];

    std::string feature_file_name = file_util::basename(sp_file_name) + ".feature";

    if (arguments["useFeatureFile"] == "true") {
      if (!file_util::exists(feature_file_name)) {
        LOG_ERROR("TopFD feature file does not exist!. Please use -x option in command line or select 'Missing MS1 feature file in GUI'.");
        return 1;
      }
    }

    int shift_num = std::stoi(arguments["shiftNumber"]);
    LOG_DEBUG("num of unknown shfit " << shift_num);
    int filter_result_num = std::stoi(arguments["filteringResultNumber"]);
    double max_shift_mass = std::stod(arguments["maxShiftMass"]);

    int thread_num = std::stoi(arguments["threadNumber"]);
    // Filter steps requires a large amount of memory. 
    // We use only one thread to reduce the memory requirement.
    int filter_thread_num = mem_check::getMaxThreads("zero_one_shift_filter");
    if (filter_thread_num > thread_num) {
      filter_thread_num = thread_num;
    }
    LOG_DEBUG("Filter thread number " << filter_thread_num);
    int diag_filter_thread_num = mem_check::getMaxThreads("diag_filter");
    if (diag_filter_thread_num > thread_num) {
      diag_filter_thread_num = thread_num;
    }

    bool decoy = false;
    if (arguments["searchType"] == "TARGET+DECOY") {
      decoy = true;
    }
    LOG_DEBUG("Decoy " << decoy);
    LOG_DEBUG("block size " << arguments["databaseBlockSize"]);
    int db_block_size = std::stoi(arguments["databaseBlockSize"]);
    int max_frag_len = std::stoi(arguments["maxFragmentLength"]);
    int min_block_num = std::stoi(arguments["minBlockNum"]);
    fasta_util::dbPreprocess(ori_db_file_name, db_file_name, decoy, 
                             db_block_size, max_frag_len, min_block_num);
    msalign_util::geneSpIndex(sp_file_name);

    PrsmParaPtr prsm_para_ptr = std::make_shared<PrsmPara>(arguments);

    // index file name
    IndexFileNamePtr file_name_ptr = std::make_shared<IndexFileName>();
    std::string index_file_para = file_name_ptr->geneFileName(arguments);


    std::vector<std::string> input_exts;

//    clock_t starttime1,endtime1,endtime2;
//    starttime1 = clock();
//    std::cout << "ASF-var PTM filtering - started." << std::endl;
//    int var_ptm_num_in_filtering = 5;
//    VarPtmFilterMngPtr var_ptm_filter_mng_ptr =
//      std::make_shared<VarPtmFilterMng>(prsm_para_ptr, index_file_para,
//                                        var_mod_file_name,
//                                        var_ptm_num_in_filtering,
//                                        filter_thread_num,
//                                        false,
//                                        "topmg_var_filter");
//
//    endtime1 = clock();
//    var_ptm_filter_processor::process(var_ptm_filter_mng_ptr);
//    std::cout << "ASF-var PTM filtering - finished." << std::endl;
//    endtime2 = clock();
//    std::cout<<"ASF-var PTM mng time = "<<double(endtime1-starttime1)/CLOCKS_PER_SEC<<"s"<<std::endl;
//    std::cout<<"ASF-var PTM total time = "<<double(endtime2-starttime1)/CLOCKS_PER_SEC<<"s"<<std::endl;
//
//    input_exts.push_back("topmg_one_filter_complete");
//    input_exts.push_back("topmg_one_filter_prefix");
//    input_exts.push_back("topmg_one_filter_suffix");
//    input_exts.push_back("topmg_one_filter_internal");


//    clock_t starttime1,endtime1,endtime2;
//    starttime1 = clock();
//    std::cout << "ASF-One PTM filtering - started." << std::endl;
//    int var_ptm_num_in_filtering = 1;
//    OnePtmFilterMngPtr one_ptm_filter_mng_ptr =
//      std::make_shared<OnePtmFilterMng>(prsm_para_ptr, index_file_para,
//                                        "topmg_one_filter",
//                                        filter_thread_num,
//                                        -max_shift_mass, max_shift_mass,
//                                        var_mod_file_name,
//                                        var_ptm_num_in_filtering);
//    one_ptm_filter_mng_ptr->inte_num_ = 4;
//    one_ptm_filter_mng_ptr->pref_suff_num_ = 4;
//    one_ptm_filter_mng_ptr->comp_num_ = 4;
//    endtime1 = clock();
//    one_ptm_filter_processor::process(one_ptm_filter_mng_ptr);
//    std::cout << "ASF-One PTM filtering - finished." << std::endl;
//    endtime2 = clock();
//    std::cout<<"ASF-one PTM mng time = "<<double(endtime1-starttime1)/CLOCKS_PER_SEC<<"s"<<std::endl;
//    std::cout<<"ASF-one PTM total time = "<<double(endtime2-starttime1)/CLOCKS_PER_SEC<<"s"<<std::endl;
//
//    input_exts.push_back("topmg_one_filter_complete");
//    input_exts.push_back("topmg_one_filter_prefix");
//    input_exts.push_back("topmg_one_filter_suffix");
//    input_exts.push_back("topmg_one_filter_internal");

//    clock_t starttime2,endtime2;
//    starttime2 = clock();
//      std::cout << "ASF-Diagonal PTM filtering - started." << std::endl;
//      filter_result_num = 200;
//
//      DiagFilterMngPtr diag_filter_mng_ptr
//          = std::make_shared<DiagFilterMng>(prsm_para_ptr, index_file_para,
//                                            filter_result_num,
//                                            diag_filter_thread_num,
//                                            "topmg_multi_filter",
//                                            var_mod_file_name, 1);
//      DiagFilterProcessorPtr diag_filter_processor
//          = std::make_shared<DiagFilterProcessor>(diag_filter_mng_ptr);
//      diag_filter_processor->process();
//      diag_filter_processor = nullptr;
//      std::cout << "ASF-Diagonal filtering - finished." << std::endl;
//      endtime2 = clock();
//      std::cout<<"ASF-Diag total time = "<<double(endtime2 - starttime2)/CLOCKS_PER_SEC<<"s"<<std::endl;
      input_exts.push_back("topmg_multi_filter");


      std::cout << "Combining filtering results - started." << std::endl;

      SimplePrsmStrMergePtr asf_filter_merger
              = std::make_shared<SimplePrsmStrMerge>(sp_file_name,
                                                     input_exts,
                                                     "topmg_graph_filter", 200 * input_exts.size());
      asf_filter_merger->process();
      asf_filter_merger = nullptr;
      std::cout << "Combining filtering results - finished." << std::endl;




//      std::vector<std::string> three_res_perm = GeneratePermutation("/home/kunyili/toppic-suite-main/three_res_perm.txt");
//      std::unordered_map<std::string,int> theo_mass_three;
//      double convert_ratio = 274.335215;
//      for(int x = 0; x < three_res_perm.size(); x++){
//          double theo_mass = 0;
//          for(int c = 0; c < 3; c ++){
//              AminoAcidPtr acid_ptr = AminoAcidBase::getAminoAcidPtrByOneLetter(three_res_perm[x].substr(c, 1));
//              ResiduePtr residue_ptr = ResidueBase::getBaseResiduePtr(acid_ptr);
//              double res_mass = residue_ptr->getMass();
//              theo_mass = theo_mass + res_mass;
//          }
//          theo_mass = round(theo_mass * convert_ratio);
//          theo_mass_three[three_res_perm[x]] = int(theo_mass);
//      }
////
//      std::unordered_map<int, std::vector<std::string>> one_res_mass_table = GenerateMassTable("/home/kunyili/toppic-suite-main/one_mass_table_tol_27.txt");
//      std::unordered_map<int, std::vector<std::string>> two_res_mass_table = GenerateMassTable("/home/kunyili/toppic-suite-main/two_mass_perm_table_tol_27.txt");
//      std::unordered_map<int, std::vector<std::string>> three_res_mass_table = GenerateMassTable("/home/kunyili/toppic-suite-main/three_mass_table_tol_27.txt");
//      std::unordered_map<int, std::vector<std::string>> three_res_mass_perm_table = GenerateMassTable("/home/kunyili/toppic-suite-main/three_mass_perm_table_tol_27.txt");
//      std::vector<std::unordered_map<int, std::vector<std::string>>> mass_tables;
//      mass_tables.push_back(one_res_mass_table);
//      mass_tables.push_back(two_res_mass_table);
//      mass_tables.push_back(three_res_mass_table);
//      mass_tables.push_back(three_res_mass_perm_table);

//      std::unordered_map<int, int> mod_table = GenerateModTable("/home/kunyili/toppic-suite-main/one_mod_table_tol_27.txt");




      //get mods info
      int t = 5;
      double convert_ratio = 274.335215;
      bool disulfide_bond = false;

      ModPtrVec N_mods_list = mod_util::readModTxt(var_mod_file_name)[3];
      int N_mods_num = N_mods_list.size();
      std::vector<int> N_mods_mm_list;
      std::vector<std::string> N_mod_ori_res_list;
      for(int i = 0; i < N_mods_list.size(); i++){
          N_mod_ori_res_list.push_back(N_mods_list[i]->getOriResiduePtr()->toString());
          N_mods_mm_list.push_back(round(N_mods_list[i]->getShift()*convert_ratio));
      }


      ModPtrVec mods_list = mod_util::readModTxt(var_mod_file_name)[2];
      std::vector<std::string> mod_ori_res_list;
      std::vector<int> Vmods_mm_list;
      for(int i = 0; i < mods_list.size(); i++){
          mod_ori_res_list.push_back(mods_list[i]->getOriResiduePtr()->toString());
          Vmods_mm_list.push_back(round(mods_list[i]->getShift()*convert_ratio));
      }

      std::vector<int> mods_mm_list = N_mods_mm_list;

      std::vector<std::string> mod_ori_res_identical;
      for(int i = 0; i < N_mod_ori_res_list.size(); i++){
          mod_ori_res_identical.push_back(N_mod_ori_res_list[i]);
      }

      for(int i = 0; i < mod_ori_res_list.size(); i ++){
          bool insert = true;
          for(int j = 0; j < mod_ori_res_identical.size(); j++){
              if(mod_ori_res_list[i] == mod_ori_res_identical[j]){
                  insert = false;
                  break;
              }
          }
          if(insert) {
              mod_ori_res_identical.push_back(mod_ori_res_list[i]);
              mods_mm_list.push_back(Vmods_mm_list[i]);
          }
      }
      std::vector<std::vector<ModPtr>> T;
      for(int i = 1; i <= t; i++){
          std::vector<std::vector<ModPtr>> combinations = combinations_with_replacement(mods_list, i);
          T.insert(T.end(),combinations.begin(),combinations.end());
      }

      std::vector<std::vector<ModPtr>> T_addN = T;
      for(int i = 0; i < N_mods_list.size(); i ++){
          T_addN.push_back({N_mods_list[i]});
          for(int j = 0; j < T.size(); j++){
              std::vector<ModPtr> plus_N_mod = T[i];
              plus_N_mod.push_back(N_mods_list[i]);
              T_addN.push_back(plus_N_mod);
          }
      }
      T = T_addN;

      if(disulfide_bond){
          std::vector<std::vector<ModPtr>> new_T = T;
          ModPtr m = mod_util::getDisulfideBondCMod();
          mod_ori_res_identical.push_back(m->getOriResiduePtr()->toString());
          mods_mm_list.push_back(round(m->getShift()*convert_ratio));
          new_T.push_back({m,m});
          new_T.push_back({m,m,m,m});
          for(int i = 0; i < T.size(); i++){
              std::vector<ModPtr> plus_one_disbond = T[i];
              std::vector<ModPtr> plus_two_disbond = T[i];
              for(int j = 0; j < 2; j ++){
                  plus_one_disbond.push_back(m);
              }
              for(int j = 0; j < 4; j ++){
                  plus_two_disbond.push_back(m);
              }
              new_T.push_back(plus_one_disbond);
              new_T.push_back(plus_two_disbond);
          }
          T = new_T;
      }

      std::vector<std::tuple<int, std::vector<int>, int>> T_mass_comb;
      for(int i = 0; i < T.size(); i++){
          if(T[i].size() <= t) {
              std::vector<int> mods_vec(mod_ori_res_identical.size());
              double comb_mm = 0;
              for (int j = 0; j < T[i].size(); j++) {
                  double mass = T[i][j]->getShift();
                  comb_mm = comb_mm + mass;
                  std::string ori_res = T[i][j]->getOriResiduePtr()->toString();
                  for (int k = 0; k < mod_ori_res_identical.size(); k++) {
                      if (mod_ori_res_identical[k] == ori_res) {
                          mods_vec[k] = mods_vec[k] + 1;
                      }
                  }
              }

              T_mass_comb.push_back(std::make_tuple(round(comb_mm * convert_ratio), mods_vec, T[i].size()));
          }
      }

      std::cout<<"T size: "<<T.size()<<std::endl;
      std::sort(T_mass_comb.begin(), T_mass_comb.end(), tuplecmp);



//      std::unordered_map<int, std::vector<int>> mod_table = GenerateModsVec("/home/kunyili/toppic-suite-main/mod_table_5_vec.txt");
//      std::vector<int> mods_mass_list;


      std::string sp_directory = sp_file_name;
      size_t pos = sp_directory.find_last_of('/');
      if (pos != std::string::npos) {
          sp_directory.erase(pos + 1);
      }


      //LCS-Filter + alignment
      clock_t sTopMGs,sTopMGe;
      sTopMGs = clock();

    if (arguments["useLCSFiltering"] == "true") {
        std::cout << "LCS PTM filtering - started." << std::endl;
        filter_result_num = 200;
        int K_value = 20;
        LCSFilterMngPtr lcs_filter_mng_ptr
        = std::make_shared<LCSFilterMng>(prsm_para_ptr,T_mass_comb,
                                          filter_result_num,
                                          diag_filter_thread_num,
                                          K_value,
                                          "topmg_multi_filter",
                                          var_mod_file_name, 1);

        lcs_filter_mng_ptr->resultpath = sp_directory;
        lcs_filter_mng_ptr->MODS_VEC = mod_ori_res_identical;
        std::cout<<"mods: "<<mod_ori_res_identical[0] << mod_ori_res_identical[1] << std::endl;
        lcs_filter_mng_ptr->MODS_MASS_VEC = mods_mm_list;
        lcs_filter_mng_ptr->N_mod_num_ = N_mods_num;
        lcs_filter_mng_ptr->max_total_mods_num = t;
        LCSFilterProcessorPtr lcs_filter_processor
            = std::make_shared<LCSFilterProcessor>(lcs_filter_mng_ptr);
        lcs_filter_processor->process();
        lcs_filter_processor = nullptr;
        std::cout << "sTopMG filtering + alignment  finished." << std::endl;

        input_exts.push_back("topmg_multi_filter");
    }

//    std::cout << "Combining filtering results - started." << std::endl;

//    SimplePrsmStrMergePtr asf_filter_merger
//        = std::make_shared<SimplePrsmStrMerge>(sp_file_name,
//                                               input_exts,
//                                               "topmg_graph_filter", 20 * input_exts.size());
//    asf_filter_merger->process();
//    asf_filter_merger = nullptr;
//    std::cout << "Combining filtering results - finished." << std::endl;


    //LCS alignment / search   sTopMG
      if (arguments["useLCSFiltering"] == "false") {
          std::cout << "sTopMG alignment - started." << std::endl;
          int K_value = -1;
          LCSFilterMngPtr lcs_alignment_mng_ptr
                  = std::make_shared<LCSFilterMng>(prsm_para_ptr, T_mass_comb,
                                                   "topmg_multi_filter",
                                                   K_value,
                                                   diag_filter_thread_num,
                                                   var_mod_file_name, 1);
          lcs_alignment_mng_ptr->MODS_VEC = mod_ori_res_identical;
          lcs_alignment_mng_ptr->MODS_MASS_VEC = mods_mm_list;
          lcs_alignment_mng_ptr->resultpath = sp_directory;
          lcs_alignment_mng_ptr->N_mod_num_ = N_mods_num;
          lcs_alignment_mng_ptr->max_total_mods_num = t;
          LCSAlignmentProcessorPtr lcs_alignment_processor
                  = std::make_shared<LCSAlignmentProcessor>(lcs_alignment_mng_ptr);
          lcs_alignment_processor->process();
          lcs_alignment_processor = nullptr;
          std::cout << "sTopMG alignment - finished." << std::endl;
      }
//    input_exts.push_back("topmg_multi_filter");
//    sTopMGe = clock();
//    //std::cout<<"sTopMG time: "<<double(sTopMGe - sTopMGs)/CLOCKS_PER_SEC<<"s"<<std::endl;
//
//
//
//
//    int max_mod_num = std::stoi(arguments["varPtmNumber"]);
//    int gap = std::stoi(arguments["proteoGraphGap"]);
//    int var_ptm_in_gap = std::min(std::stoi(arguments["varPtmNumInGap"]), max_mod_num);
//    bool whole_protein_only = (arguments["wholeProteinOnly"] == "true");
//    GraphAlignMngPtr ga_mng_ptr
//        = std::make_shared<GraphAlignMng>(prsm_para_ptr,
//                                          var_mod_file_name,
//                                          shift_num, max_mod_num,
//                                          gap, var_ptm_in_gap, max_shift_mass,
//                                          thread_num, whole_protein_only,
//                                          "topmg_graph_filter", "topmg_graph_align");
//    std::cout << "Graph alignment - started." << std::endl;
//    GraphAlignProcessorPtr ga_processor_ptr = std::make_shared<GraphAlignProcessor>(ga_mng_ptr);
//    ga_processor_ptr->process();
//    ga_processor_ptr = nullptr;
//    std::cout << "Graph alignment - finished." << std::endl;
//
//    std::cout << "Graph alignment post-processing - started." << std::endl;
//    GraphPostProcessorPtr ga_post_processor_ptr
//        = std::make_shared<GraphPostProcessor>(ga_mng_ptr, "topmg_graph_align", "topmg_graph_post");
//    ga_post_processor_ptr->process();
//    ga_post_processor_ptr = nullptr;
//    std::cout << "Graph alignment post-processing - finished." << std::endl;
//
//    std::cout << "E-value computation using MCMC - started." << std::endl;
//    MCMCMngPtr mcmc_mng_ptr
//        = std::make_shared<MCMCMng>(prsm_para_ptr, "topmg_graph_post", "topmg_evalue",
//                                    var_mod_file_name, max_mod_num, thread_num);
//    DprProcessorPtr processor = std::make_shared<DprProcessor>(mcmc_mng_ptr);
//    processor->process();
//    processor = nullptr;
//    std::cout << "E-value computation using MCMC - finished." << std::endl;
//
//    int n_top = std::stoi(arguments["numOfTopPrsms"]);
//
//    std::cout << "Top PrSM selecting - started" << std::endl;
//    prsm_top_selector::process(sp_file_name, "topmg_evalue", "topmg_prsm", n_top);
//    std::cout << "Top PrSM selecting - finished." << std::endl;
  } catch (const char* e) {
    std::cout << "[Exception]" << std::endl;
    std::cout << e << std::endl;
  }
  return 0;
}

int TopMG_post(std::map<std::string, std::string> & arguments) {
  try {
    std::string resource_dir = arguments["resourceDir"];

    base_data::init();
    LOG_DEBUG("Initialization completed");
    std::string ori_db_file_name = arguments["oriDatabaseFileName"];
    std::string db_file_name = ori_db_file_name + "_idx" + file_util::getFileSeparator() + file_util::filenameFromEntirePath(arguments["databaseFileName"]);    
    std::string sp_file_name = arguments["spectrumFileName"];
    std::string var_mod_file_name = arguments["varModFileName"];

    PrsmParaPtr prsm_para_ptr = std::make_shared<PrsmPara>(arguments);
    msalign_util::geneSpIndex(sp_file_name);

    std::cout << "Finding PrSM clusters - started." << std::endl;
    //double form_error_tole = std::stod(arguments["proteoformErrorTolerance"]);
      double form_error_tole = 1.2;
    if (arguments["useFeatureFile"] == "true") {
      // TopFD msalign file with feature ID
      ModPtrVec fix_mod_list = prsm_para_ptr->getFixModPtrVec();
      prsm_feature_cluster::process(sp_file_name,
                                    "topmg_prsm",
                                    "topmg_cluster",
                                    form_error_tole);
    } 
    else {
      prsm_simple_cluster::process(db_file_name, 
                                   sp_file_name,
                                   "topmg_prsm", 
                                   prsm_para_ptr->getFixModPtrVec(),
                                   "topmg_cluster", 
                                   form_error_tole);
    }
    std::cout << "Finding PrSM clusters - finished." << std::endl;

    std::string cur_suffix = "topmg_cluster";

    if (arguments["searchType"] == "TARGET+DECOY") {
      std::cout << "FDR computation - started. " << std::endl;
      prsm_fdr::process(sp_file_name, "topmg_cluster", "topmg_cluster_fdr", arguments["keepDecoyResults"]);
      std::cout << "FDR computation - finished." << std::endl;
      cur_suffix = "topmg_cluster_fdr";
    }

    std::string cutoff_type = arguments["cutoffSpectralType"];
    std::cout << "PrSM filtering by " << cutoff_type << " - started." << std::endl;
    double cutoff_value = std::stod(arguments["cutoffSpectralValue"]);
    prsm_cutoff_selector::process(db_file_name, sp_file_name, cur_suffix,
                                  "topmg_prsm_cutoff", cutoff_type, cutoff_value);
    std::cout << "PrSM filtering by " << cutoff_type << " - finished." << std::endl;

    std::time_t end = time(nullptr);
    char buf[50];
    std::strftime(buf, 50, "%a %b %d %H:%M:%S %Y", std::localtime(&end));
    arguments["endTime"] = buf;

    std::string argu_str = TopmgArgument::outputTsvArguments(arguments);

    std::cout << "Outputting PrSM table - started." << std::endl;
    PrsmMatchTableWriterPtr table_out
        = std::make_shared<PrsmMatchTableWriter>(prsm_para_ptr, argu_str, "topmg_prsm_cutoff", "_topmg_prsm_single.tsv", false);
    table_out->write();

    table_out->setOutputName("_topmg_prsm.tsv");
    table_out->setWriteMultiMatches(true);
    table_out->write();
    table_out = nullptr;
    std::cout << "Outputting PrSM table - finished." << std::endl;

    XmlGeneratorPtr xml_gene = std::make_shared<XmlGenerator>(prsm_para_ptr, resource_dir, "topmg_prsm_cutoff", "topmg_prsm_cutoff");
    
    if (arguments["geneHTMLFolder"] == "true"){//only when the parameter is set to true
    
      std::cout << "Generating PrSM xml files - started." << std::endl;
      xml_gene->process();
      xml_gene = nullptr;
      std::cout << "Generating PrSM xml files - finished." << std::endl;

      copyTopMSV(arguments);

      std::cout << "Converting PrSM xml files to html files - started." << std::endl;
      jsonTranslate(arguments, "topmg_prsm_cutoff");
      std::cout << "Converting PrSM xml files to html files - finished." << std::endl;  
    }
    
    cutoff_type = (arguments["cutoffProteoformType"] == "FDR") ? "FORMFDR": "EVALUE";
    std::cout << "PrSM filtering by " << cutoff_type << " - started." << std::endl;
    std::istringstream(arguments["cutoffProteoformValue"]) >> cutoff_value;
    prsm_cutoff_selector::process(db_file_name, sp_file_name, cur_suffix,
                                  "topmg_form_cutoff", cutoff_type, cutoff_value);
    std::cout << "PrSM filtering by " << cutoff_type << " - finished." << std::endl;

    std::cout << "Selecting top PrSMs for proteoforms - started." << std::endl;
    prsm_form_filter::process(db_file_name, sp_file_name, "topmg_form_cutoff",
                              "topmg_form_cutoff_form");
    std::cout << "Selecting top PrSMs for proteoforms - finished." << std::endl;

    std::cout << "Outputting proteoform table - started." << std::endl;
    PrsmMatchTableWriterPtr form_out
        = std::make_shared<PrsmMatchTableWriter>(prsm_para_ptr, argu_str,
                                            "topmg_form_cutoff_form", 
                                            "_topmg_proteoform_single.tsv", false);
    form_out->write();

    form_out->setOutputName("_topmg_proteoform.tsv");
    form_out->setWriteMultiMatches(true);
    form_out->write();

    form_out = nullptr;
    std::cout << "Outputting proteoform table - finished." << std::endl;

    if (arguments["geneHTMLFolder"] == "true"){//only when the parameter is set to true
      std::cout << "Generating proteoform xml files - started." << std::endl;
      xml_gene = std::make_shared<XmlGenerator>(prsm_para_ptr, resource_dir, "topmg_form_cutoff", "topmg_proteoform_cutoff");

      xml_gene->process();
      xml_gene = nullptr;
      std::cout << "Generating proteoform xml files - finished." << std::endl;

      std::cout << "Converting proteoform xml files to html files - started." << std::endl;
      jsonTranslate(arguments, "topmg_proteoform_cutoff");
      std::cout << "Converting proteoform xml files to html files - finished." << std::endl;
    }
  } catch (const char* e) {
    LOG_ERROR("[Exception]" << e);
  }
  return 0;
}

int TopMGProcess(std::map<std::string, std::string> & arguments) {
  if (TopMG_identify(arguments) != 0) {
    return 1;
  }
  return TopMG_post(arguments);
}

int TopMGProgress_multi_file(std::map<std::string, std::string> & arguments,
                             const std::vector<std::string> & spec_file_lst) {

  std::string base_path = file_util::absoluteDir(spec_file_lst[0]);
  std::string full_combined_name = base_path + file_util::getFileSeparator() 
      +  arguments["combinedOutputName"];

  std::time_t start = time(nullptr);
  char buf[50];
  std::strftime(buf, 50, "%a %b %d %H:%M:%S %Y", std::localtime(&start));
  std::string combined_start_time = buf;

  std::cout << "TopMG " << Version::getVersion() << std::endl;
  arguments["version"] = Version::getVersion();

  xercesc::XMLPlatformUtils::Initialize(); 
  TopMG_testModFile(arguments);

  //check if a combined file name given in -c parameter is the same as one of the input spectrum file. If so, throw error.
  if (arguments["combinedOutputName"] != "") {
    std::string merged_file_name = arguments["combinedOutputName"] + "_ms2.msalign"; 
    for (size_t k = 0; k < spec_file_lst.size(); k++) {
      if (merged_file_name == spec_file_lst[k]) {
        std::string raw_file_name = spec_file_lst[k].substr(0, spec_file_lst[k].find("_ms2.msalign"));
        LOG_ERROR("A combined file name cannot be the same as one of the input file names '" << raw_file_name << "'. Please choose a different name for a combined file and retry.");
        return 1;
      }
    }
  }

  for (size_t k = 0; k < spec_file_lst.size(); k++) {
    std::strftime(buf, 50, "%a %b %d %H:%M:%S %Y", std::localtime(&start));
    std::string start_time = buf;
    arguments["startTime"] = start_time;
    arguments["spectrumFileName"] = spec_file_lst[k];
    if (TopMGProcess(arguments) != 0) {
      return 1;
    }
  }

  if (arguments["combinedOutputName"] != "") {
    std::string para_str = "";
    std::cout << "Merging files started." << std::endl;
    std::cout << "Merging msalign files started." << std::endl;
    MsAlignFracMerge::mergeFiles(spec_file_lst, full_combined_name + "_ms2.msalign", para_str);
    std::cout << "Merging msalign files finished." << std::endl;
    if (arguments["geneHTMLFolder"] == "true"){
      std::cout << "Merging json files started." << std::endl;
      DeconvJsonMergePtr json_merger 
          = std::make_shared<DeconvJsonMerge>(spec_file_lst, full_combined_name);
      json_merger->process();
      json_merger = nullptr;
      std::cout << "Merging json files finished." << std::endl;
    }
	if (arguments["useFeatureFile"] == "true") {//only when feature files are being used
      std::cout << "Merging feature files started." << std::endl;
      feature_merge::process(spec_file_lst, full_combined_name, para_str);
      std::cout << "Merging feature files finished." << std::endl;
    }
    // merge TOP files
    std::cout << "Merging identification files started." << std::endl;
    std::vector<std::string> prsm_file_lst(spec_file_lst.size());
    for (size_t i = 0; i < spec_file_lst.size(); i++) {
      prsm_file_lst[i] = file_util::basename(spec_file_lst[i]) + ".topmg_prsm"; 
    }
    int N = 1000000;
    prsm_util::mergePrsmFiles(prsm_file_lst, N , full_combined_name + "_ms2.topmg_prsm");
    std::cout << "Merging identification files finished." << std::endl;
    std::cout << "Merging files - finished." << std::endl;

    std::string sp_file_name = full_combined_name + "_ms2.msalign";
    arguments["spectrumFileName"] = sp_file_name;
    arguments["startTime"] = combined_start_time;
    TopMG_post(arguments);
  }

  bool keep_temp_files = (arguments["keepTempFiles"] == "true");
  std::cout << "Deleting temporary files - started." << std::endl;
  std::string ori_db_file_name = arguments["oriDatabaseFileName"];

  for (size_t k = 0; k < spec_file_lst.size(); k++) {
    std::string sp_file_name = spec_file_lst[k];
    cleanTopmgDir(ori_db_file_name, sp_file_name, keep_temp_files);
  }

  if (arguments["combinedOutputName"] != "") {
    std::string sp_file_name = full_combined_name + "_ms2.msalign";
    cleanTopmgDir(ori_db_file_name, sp_file_name, keep_temp_files);
  }
  std::cout << "Deleting temporary files - finished." << std::endl; 

  base_data::release();

  std::cout << "TopMG finished." << std::endl << std::flush;
  return 0; 
}

}  // namespace toppic
