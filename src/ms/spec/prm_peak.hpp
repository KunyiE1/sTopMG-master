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

#ifndef TOPPIC_MS_SPEC_PRM_PEAK_HPP_
#define TOPPIC_MS_SPEC_PRM_PEAK_HPP_

#include "ms/spec/deconv_peak.hpp"
#include "ms/spec/support_peak.hpp"
#include "ms/spec/base_peak_type.hpp"
#include "ms/spec/rm_break_type.hpp"
#include <math.h>

namespace toppic {

class PrmPeak;
typedef std::shared_ptr<PrmPeak> PrmPeakPtr;

class PrmPeak : public Peak {
 public:
  PrmPeak(int spec_id, DeconvPeakPtr base_peak_ptr,
          BasePeakTypePtr base_type,
          double mono_mass, double score,
          double strict_tolerance = 0.0,
          double n_strict_c_relax_tolerance = 0.0,
          double n_relax_c_strict_tolerance = 0.0);

  PrmPeak(PrmPeakPtr prm_peak_ptr) : Peak(prm_peak_ptr->getMonoMass(), prm_peak_ptr->getBasePeakPtr()->getIntensity()){
      spec_id_ = prm_peak_ptr->getSpectrumId();
      peak_id_ = prm_peak_ptr->getPeakId();
      base_peak_ptr_ = prm_peak_ptr->getBasePeakPtr();
      mono_mass_ = prm_peak_ptr->getMonoMass();
      base_type_ = prm_peak_ptr->getBaseTypePtr();
      score_ = prm_peak_ptr->getScore();
      strict_tolerance_ = prm_peak_ptr->getStrictTolerance();
      n_strict_c_relax_tolerance_ = prm_peak_ptr->getNStrictCRelaxTolerance();
      n_relax_c_strict_tolerance_ = prm_peak_ptr->getNRelaxCStrictTolerance();
      //list_idx_ = prm_peak_ptr->getListIdx();
      //pos_after_move_ = prm_peak_ptr->getPosAfterMove();
      //pair_peak_ptr_ = prm_peak_ptr->getPairPeak();
      //Td_Idx = prm_peak_ptr->getTdIdx();
      Int_pos = round(mono_mass_ * convert_ratio);
      Int_pos_after_move_ = prm_peak_ptr->getIntPosAfterMove();
      //pair_type_ = prm_peak_ptr->getPairType();
      //interpeak_vec_ = prm_peak_ptr->getInterPeakVec();
      peak_in_pair_ = prm_peak_ptr->getPeakInPair();
      mass_dist_ = prm_peak_ptr->getMassDist();
      spec_peak_vec_ = prm_peak_ptr->getSpecPeakVec();
      prot_peak_vec_ = prm_peak_ptr->getProtPeakVec();
      int_tolerance_ = prm_peak_ptr->getIntTolerance();
  }

  void addNghbEdge(DeconvPeakPtr deconv_peak_ptr, double offset,
                   SPTypePtr peak_type, double score);

  int getNeighborSize() {return neighbor_list_.size();}

  DeconvPeakPtr getBasePeakPtr() {return base_peak_ptr_;}

  double getMonoMass() {return mono_mass_;}


  void setMonoMass(double m);

  double getScore() {return score_;}

  double getStrictTolerance() {return strict_tolerance_;}

  BasePeakTypePtr getBaseTypePtr() {return base_type_;}

  double getNStrictCRelaxTolerance() {return n_strict_c_relax_tolerance_;}

  double getNRelaxCStrictTolerance() {return n_relax_c_strict_tolerance_;}

  int getSpectrumId() {return spec_id_;}

  int getPeakId() {return peak_id_;}

  RmBreakTypePtr getBreakType();

  void setStrictTolerance(double tolerance) {
      strict_tolerance_ = tolerance;
      int_tolerance_ = round(tolerance * 274.335215);
  }

  void setIntTolerance(int tolerance) {int_tolerance_ = tolerance;}
  int getIntTolerance(){return int_tolerance_;}

  int getIntNRelaxCStrictTolerance() {return int_n_relax_c_strict_tolerance_;}

  void setNStrictCRelacTolerance(double tolerance) {
    n_strict_c_relax_tolerance_ = tolerance;
  }

  void setNRelaxCStrictTolerance(double tolerance) {
    n_relax_c_strict_tolerance_ = tolerance;
      int_n_relax_c_strict_tolerance_ = round(tolerance * 274.335215);
  }

  void setPeakId(int peak_id) {peak_id_ = peak_id;}

  static bool cmpPosInc(const PrmPeakPtr &a, const PrmPeakPtr &b) {
    return a->getPosition() < b->getPosition();}

  //int getTdIdx() {return Td_Idx;}

  //void setTdIdx(int td_idx) {Td_Idx = td_idx;}

  PrmPeakPtr  getPairPeak(){return pair_peak_ptr_;}

  void setPairPeak(PrmPeakPtr pair_peak_ptr){pair_peak_ptr_ = pair_peak_ptr;}

  //double getPosAfterMove(){return pos_after_move_;}

  //void setPosAferMove(double pos){pos_after_move_ = pos;}

  //int getListIdx(){return list_idx_;}

  //void setListIdx(int idx){list_idx_ = idx;}

  int getIntPosition(){return Int_pos;}
  void setIntPosition(int x){ Int_pos = x;}

  int getIntPosAfterMove(){return Int_pos_after_move_;}

  void setIntPosAfterMove(int pos){Int_pos_after_move_ = pos;}

  //int getPairType(){return pair_type_;}

  //void setPairType(int pair_type){pair_type_ = pair_type;}

  int getMassDist(){return mass_dist_;}

  void setMassDist(int mass_dist){mass_dist_ = mass_dist_;}

 void setPeakInPair(int peaknum){peak_in_pair_ = peaknum;}

  int getPeakInPair(){return peak_in_pair_;}
  double convert_ratio = 274.335215;

  //void setInterPeakVec(int idx, int value){interpeak_vec_[idx] = value;}

  //void setInterPeakVec(std::vector<int>interpeak_vec){interpeak_vec_ = interpeak_vec;}
  //std::vector<int> getInterPeakVec(){return interpeak_vec_;}

    void setSpecPeakVec(std::vector<int>spec_peak_vec){spec_peak_vec_ = spec_peak_vec;}
    std::vector<int> getSpecPeakVec(){return spec_peak_vec_;}

    void setProtPeakVec(std::vector<int>prot_peak_vec){prot_peak_vec_ = prot_peak_vec;}
    std::vector<int> getProtPeakVec(){return prot_peak_vec_;}

    void setNode(int node){node_idx_ = node;}
    int getNode(){return node_idx_;}

 private:
  int spec_id_;
  DeconvPeakPtr base_peak_ptr_;
  BasePeakTypePtr base_type_;
  int peak_id_;
  //int Td_Idx;
  int Int_pos;
  int Int_pos_after_move_;
  int int_tolerance_;
  int mass_dist_;
  double mono_mass_;
  //double pos_after_move_;
  //int list_idx_ = -1;
  double score_;
  double strict_tolerance_;
  double n_strict_c_relax_tolerance_;
  double n_relax_c_strict_tolerance_;
  int int_n_relax_c_strict_tolerance_;
  //int pair_type_;
  int peak_in_pair_ = 3;
  //std::vector<int> interpeak_vec_;
  std::vector<int> spec_peak_vec_;
  int node_idx_;
  std::vector<int> prot_peak_vec_;
  PrmPeakPtr  pair_peak_ptr_;

  SupportPeakPtrVec neighbor_list_;
};

typedef std::vector<PrmPeakPtr> PrmPeakPtrVec;
typedef std::vector<PrmPeakPtrVec> PrmPeakPtrVec2D;


} /* namespace toppic */

#endif /* PRM_PEAK_HPP_ */
