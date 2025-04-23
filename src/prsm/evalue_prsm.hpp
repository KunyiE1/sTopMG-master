//
// Created by kunyili on 3/18/25.
//

#ifndef TOPPIC_EVALUE_PRSM_HPP
#define TOPPIC_EVALUE_PRSM_HPP



#include <string>
#include <vector>

#include "seq/proteoform.hpp"
#include "ms/spec/deconv_ms.hpp"
#include "ms/spec/extend_ms.hpp"
#include "para/sp_para.hpp"
#include "prsm/expected_value.hpp"
#include "ms/factory/prm_ms_util.hpp"

namespace toppic {
    class Evalue_Prsm;
    typedef std::shared_ptr<Evalue_Prsm> Evalue_PrsmPtr;

    class Evalue_Prsm {
    public:
        Evalue_Prsm(DeconvMsPtrVec deconv_ms_ptr_vec, int alignment_score, int var_mods_num, std::string prot_name, double prot_res_mass_sum);


        void setExpectedValuePtr(ExpectedValuePtr ev_ptr) {expected_value_ptr_ = ev_ptr;}


        double getEValue();


        double prot_res_mass_sum_;
        DeconvMsPtrVec deconv_ms_ptr_vec_;
        std::string prot_name_;
        int alignment_score_;
        int var_mods_num_;
        ProteoformTypePtr type_ptr;
        ExpectedValuePtr expected_value_ptr_;
//        double E_value_ = -2;
    };

} // toppic

#endif //TOPPIC_EVALUE_PRSM_HPP
