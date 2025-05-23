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

#ifndef TOPPIC_TOPFD_ENV_ENV_FILTER_HPP_
#define TOPPIC_TOPFD_ENV_ENV_FILTER_HPP_

#include "ms/env/env_para.hpp"
#include "ms/env/match_env.hpp"

namespace toppic {

namespace env_filter {

void filter(MatchEnvPtr2D &match_envs, const PeakPtrVec &peak_list,
            EnvParaPtr env_para_ptr);

// Filtering envelopes without removing envelopes with different 
// candidate charge states and monoisotopic masses. For example,
// The same envelope with -1/+1 Dalton shift or -1/+1 charge difference
void multipleMassFilter(MatchEnvPtr2D &match_env, EnvParaPtr env_para_ptr);

bool testRealEnvValid(MatchEnvPtr env, EnvParaPtr env_para_ptr);
}

}

#endif
