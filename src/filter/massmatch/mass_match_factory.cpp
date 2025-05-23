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

#include "common/util/logger.hpp"
#include "filter/massmatch/mass_match_factory.hpp"

namespace toppic {

namespace mass_match_factory {

inline std::vector<int> getScaledSrmMasses(ProteoformPtr proteo_ptr,
                                           std::vector<double> &n_ace_shifts,
                                           double scale) {
  std::vector<int> masses = proteo_ptr->getBpSpecPtr()->getScaledPrmMasses(scale);
  for (size_t i = 0; i < n_ace_shifts.size(); i++) {
    int ace_mass = static_cast<int>(std::round(- n_ace_shifts[i] * scale));
    masses.push_back(ace_mass);
  }
  std::sort(masses.begin(), masses.end(), std::less<int>());
  std::vector<int> rev_masses;
  int len = masses.size();
  for (int i = len -1 ; i >= 0; i--) {
    rev_masses.push_back(masses[len-1] - masses[i]);
  }
  return rev_masses;
}

inline MassMatchPtr getMassMatchPtr(const ProteoformPtrVec &proteo_ptrs,
                                    std::vector<std::vector<int>> &mass_2d,
                                    double max_proteoform_mass, double scale, 
                                    bool prm) {
  std::vector<double> proteo_minus_water_masses;
  std::vector<std::vector<double>> float_shift_2d;
  std::vector<std::vector<int>> pos_2d;
  for (size_t i = 0; i < proteo_ptrs.size(); i++) {
    proteo_minus_water_masses.push_back(proteo_ptrs[i]->getMinusWaterMass());
    std::vector<double> masses;
    if (prm) {
      masses = proteo_ptrs[i]->getBpSpecPtr()->getPrmMasses();
    } else {
      masses = proteo_ptrs[i]->getBpSpecPtr()->getSrmMasses();
    }
    std::vector<double> shifts;
    std::vector<int> positions;
    for (size_t j = 0; j < masses.size() -1; j++) {
      shifts.push_back(-masses[j]);
      positions.push_back(j);
    }
    float_shift_2d.push_back(shifts);
    pos_2d.push_back(positions);
  }

  LOG_DEBUG("pos 2d ver 1 complete");
  MassMatchPtr index_ptr = std::make_shared<MassMatch>(proteo_minus_water_masses, 
                                                       mass_2d, float_shift_2d, pos_2d,
                                                       max_proteoform_mass, scale);
 
  return index_ptr;
}


MassMatchPtr getPrmDiagMassMatchPtr(const ProteoformPtrVec &proteo_ptrs,
                                    double max_proteoform_mass, double scale) {
  std::vector<std::vector<int>> mass_2d;
  for (size_t i = 0; i < proteo_ptrs.size(); i++) {
    std::vector<int> masses = proteo_ptrs[i]->getBpSpecPtr()->getScaledPrmMasses(scale);
    mass_2d.push_back(masses);
  }
  LOG_DEBUG("mass 2d ver 1 complete");
  bool prm = true;
  return getMassMatchPtr(proteo_ptrs, mass_2d, max_proteoform_mass, scale, prm);
}

MassMatchPtr getSrmDiagMassMatchPtr(const ProteoformPtrVec &proteo_ptrs,
                                    std::vector<std::vector<double>> &n_ace_shift_2d,
                                    double max_proteoform_mass, double scale) {
  std::vector<std::vector<int>> mass_2d;
  for (size_t i = 0; i < proteo_ptrs.size(); i++) {
    std::vector<int> masses = getScaledSrmMasses(proteo_ptrs[i], n_ace_shift_2d[i], scale);
    mass_2d.push_back(masses);
  }
  LOG_DEBUG("mass 2d ver 1 complete");
  bool prm = false;
  return getMassMatchPtr(proteo_ptrs, mass_2d, max_proteoform_mass, scale, prm);
}

inline void getPos2d(const ProteoformPtrVec &proteo_ptrs,
                     std::vector<std::vector<double>> &float_shift_2d,
                     std::vector<std::vector<int>> &pos_2d) {
  for (size_t i = 0; i < proteo_ptrs.size(); i++) {
    int n = float_shift_2d[i].size();
    std::vector<int> positions(n, 0);
    pos_2d.push_back(positions);
  }
  LOG_DEBUG("pos 2d complete");
}

MassMatchPtr getPrmTermMassMatchPtr(const ProteoformPtrVec &proteo_ptrs,
                                    std::vector<std::vector<double>> &float_shift_2d,
                                    double max_proteoform_mass, double scale) {
  std::vector<double> proteo_minus_water_masses;
  std::vector<std::vector<int>> mass_2d;
  for (size_t i = 0; i < proteo_ptrs.size(); i++) {
    std::vector<int> masses = proteo_ptrs[i]->getBpSpecPtr()->getScaledPrmMasses(scale);
    mass_2d.push_back(masses);
    proteo_minus_water_masses.push_back(proteo_ptrs[i]->getMinusWaterMass());
  }
  LOG_DEBUG("mass 2d complete");
  LOG_DEBUG("proteo num " << proteo_ptrs.size());
  LOG_DEBUG("shift num " << float_shift_2d.size());

  std::vector<std::vector<int>> pos_2d;
  getPos2d(proteo_ptrs, float_shift_2d, pos_2d);
  MassMatchPtr index_ptr = std::make_shared<MassMatch>(proteo_minus_water_masses, 
                                                       mass_2d, float_shift_2d, pos_2d,
                                                       max_proteoform_mass, scale);
  return index_ptr;
}

MassMatchPtr getSrmTermMassMatchPtr(const ProteoformPtrVec &proteo_ptrs,
                                    std::vector<std::vector<double>> &float_shift_2d,
                                    std::vector<std::vector<double>> &n_ace_shift_2d,
                                    double max_proteoform_mass, double scale) {
  std::vector<double> proteo_minus_water_masses;
  std::vector<std::vector<int>> mass_2d;
  for (size_t i = 0; i < proteo_ptrs.size(); i++) {
    std::vector<int> masses = getScaledSrmMasses(proteo_ptrs[i], n_ace_shift_2d[i], scale);
    mass_2d.push_back(masses);
    proteo_minus_water_masses.push_back(proteo_ptrs[i]->getMinusWaterMass());
  }
  LOG_DEBUG("mass 2d complete");
  LOG_DEBUG("proteo num " << proteo_ptrs.size());
  LOG_DEBUG("shift num " << float_shift_2d.size());

  std::vector<std::vector<int>> pos_2d;
  getPos2d(proteo_ptrs, float_shift_2d, pos_2d);
  MassMatchPtr index_ptr = std::make_shared<MassMatch>(proteo_minus_water_masses, 
                                                       mass_2d, float_shift_2d, pos_2d,
                                                       max_proteoform_mass, scale);
  return index_ptr;
}

}

}  // namespace toppic
