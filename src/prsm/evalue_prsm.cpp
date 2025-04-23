//
// Created by kunyili on 3/18/25.
//

#include "evalue_prsm.hpp"
#include "common/util/logger.hpp"

namespace toppic {
    Evalue_Prsm::Evalue_Prsm(DeconvMsPtrVec deconv_ms_ptr_vec, int alignment_score, int var_mods_num, std::string prot_name, double prot_res_mass_sum) :
    deconv_ms_ptr_vec_(deconv_ms_ptr_vec),
    alignment_score_(alignment_score),
    var_mods_num_(var_mods_num),
    prot_name_(prot_name),
    prot_res_mass_sum_(prot_res_mass_sum){}

    double Evalue_Prsm::getEValue() {
        if (expected_value_ptr_ == nullptr) {
            LOG_WARN("Probability pointer is null.");
            return -1;
        } else {
            return expected_value_ptr_->getEValue();
        }
    }



} // toppic