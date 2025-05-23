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

#ifndef TOPPIC_BASE_RM_BREAK_TYPE_HPP_
#define TOPPIC_BASE_RM_BREAK_TYPE_HPP_

#include <memory>
#include <string>

namespace toppic {

// residue mass break type
class RmBreakType;
typedef std::shared_ptr<RmBreakType> RmBreakTypePtr;

class RmBreakType {
 public:
  static const RmBreakTypePtr NONE;

  static const RmBreakTypePtr N_TERM;

  static const RmBreakTypePtr C_TERM;

  static const RmBreakTypePtr BOTH;

  std::string getName() {return name_;}

  explicit RmBreakType(std::string name): name_(name) {}

 private:
  std::string name_;
};

}  // namespace toppic

#endif

