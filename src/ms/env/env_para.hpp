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

#ifndef TOPPIC_TOPFD_ENV_ENV_PARA_HPP_
#define TOPPIC_TOPFD_ENV_ENV_PARA_HPP_

#include <memory>
#include <vector>
#include <string>

#include "topfd/common/topfd_para.hpp"

namespace toppic {

class EnvPara;
typedef std::shared_ptr<EnvPara> EnvParaPtr;

class EnvPara {
 public:
  EnvPara(){};

  EnvPara(TopfdParaPtr topfd_para_ptr); 

  int getMassGroup(double base_mass);

  void setMinInte(double min_inte, int ms_level);

  int compMinConsPeakNum(int peak_num, int mass_group);

  void setTolerance(double tolerance); 

  double getMzTolerance() {return mz_tolerance_;}

  double getScoreErrorTolerance() {return score_error_tolerance_;}

  double getPercentBound(int mass_group) {return percentage_bound_[mass_group];}

  double getMaxCharge() {return max_charge_;}

  double getMaxMass() {return max_mass_;} 

  static int getDefaultMaxCharge() {return 30;}

  static double getDefaultMaxMass() {return 70000;}

  // using input parameters to assign: max_chrg, max_mass 
  int max_charge_ = 30;
  double max_mass_ = 70000;
  // window size 1 m/z
  double window_size_ = 1.0;

  // preprocessing
  // estimate min intensity using thrash method. 
  bool estimate_min_inte_ = true;
  // signal noise ratio 
  double ms_two_sn_ratio_ = 1;
  // ms one signal noise ratio
  double ms_one_sn_ratio_ = 3;
  // minimum peak intensity 
  double min_inte_ = 0;
  // minimum base peak intensity 
  // min_refer_inte_ = min_inte * sn_ratio_ 
  double min_refer_inte_ = 0;

  // Envelope detection
  // min_inte and min_ref_inte are used in envelope detection

  // error tolerance for matching real peaks to theoretical peaks 
  double mz_tolerance_ = 0.02;

  // the minimum monoisotopic envelope for an envelope 
  double min_mass_ = 50;

  // Several parameters are related with the mass of envelopes. We classify
  // envelopes into 3 groups based on its base mass. See getMassGroup().
  std::vector<double> mass_group_boundary_ = {min_mass_, min_mass_, 1500, max_mass_};

  // perc bound is used for remove low intensities in theoretical envelopes
  std::vector<double> percentage_bound_ = {0.95, 0.95, 0.85};

  //  maximum number of peaks left and right to the base peak in theoretical envelopes
  int max_back_peak_num_ = 8;
  int max_forw_peak_num_ = 8;

  // Envelope filtering

  // 1. filtering based on real envelop an real envelope is valid if 1) peak
  // number >= 3 2) at most 1 missing peak 3) consecutive peak number >=
  // peak_num - 3, 3
  // minimum peak number in an envelope 
  std::vector<int> min_match_peak_num_ = {1, 2, 3 };
  int max_miss_peak_num_ = 1;
  // check consecutive peaks 
  bool check_consecutive_peak_num_ = true;
  int relative_consecutive_peak_num_ = -3;
  std::vector<int> min_consecutive_peak_num_ = { 1, 2, 3 };

  // 2. filtering using score

  // parameters for computing scores of matching envelopes 
  // Optimize the score using small shifts of theoretical m/z values 
  bool do_mz_shift_ = false;

  // when mz shift is used, the minimum shift is 0.001, shift_fold = 1/0.001
  int shift_scale_ = 1000;
  // Optimize the score using a scale ratio of theoretical peak intensities 
  bool do_inte_ratio_ = false;

  // when intensity ratio is used, the minimum shift is 0.01, ratio_fold =
  // 1/0.01. We enumerate all possible intensity ratios from bgn_ratio to
  // end_ratio
  int inte_ratio_scale_ = 100;
  double bgn_ratio_ = 0.8;
  double end_ratio_ = 1.2;
  // maximum error in computing m/z accuracy 
  double score_error_tolerance_ = mz_tolerance_; 
  // minimum score for matching envelopes 
  double min_match_env_score_ = 0;

  // 3. fitering using envelopes with charge Z, 2 times of Z, 3 times of Z, 
  // no parameters here

  // 4. filtering by comparing envelopes with similar charge.
  // If two envelopes uses the same reference peak and their
  // charge states are Z and Z+1, then one envelope is removed. 
  // This step is applied to only high charge state (Z>= 15) envelopes
  int charge_computation_bgn_ = 15;
  double charge_computation_mz_tolerance_ = 0.002;

  // 5. filtering by comparing envelopes with similar mz and same charge
  // If two envelopes have the same charge and their monoisotopic mass 
  // difference is less than 12 Dalton, then only the top scoring one is keep.
  double rank_peak_distance_ = 12;
  // Only keep the top envelope
  int max_similar_mz_env_rank_ = 0;

  // Envelope assigned to 1 m/z intervals
  // number of envelopes per window 
  // use a small number of envelopes to speed up computation
  int env_num_per_window_ = 5;

  // envelope final filtering
  // use filtering to keep only highest peaks. 
  bool do_final_filtering_ = true;
  // Monoisotopic masses are divided into two groups 
  // < 1500 and > 1500 in filtering
  double low_high_dividor_ = 1500;
  double aa_avg_mass_ = 120;
  double peak_density_ = 2;

  //  unused peaks
  bool keep_unused_peaks_ = false;

  // Output multiple masses 
  // For one envelope, if we cannot determine its
  // charge state and monoisotopic mass, we will 
  // add several envelopes with two consecutive charges
  // and envelopes with -1 and +1 Dalton shift
  // See match_env_util::addMultipleMass
  bool output_multiple_mass_ = false;
  double multiple_min_mass_ = 5000;
  int multiple_min_charge_ = 20;
  double multiple_min_ratio_ = 0.9;

  // precursor ion window size
  double prec_deconv_interval_ = 3.0;

  // Use EnvCNN
  bool use_env_cnn_ = false;
};

} /* namespace */

#endif 
