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

#include "xercesc/framework/MemBufInputSource.hpp"

#include "common/util/logger.hpp"
#include "common/util/str_util.hpp"
#include "prsm/prsm_reader.hpp"

namespace toppic {

PrsmReader::PrsmReader(const std::string &file_name) {
  input_.open(file_name.c_str(), std::ios::in);
}

PrsmReader::~PrsmReader() {
  if (input_.is_open()) {
    input_.close();
  }
}

std::vector<std::string> PrsmReader::readOnePrsmLines() {
  std::string line;
  std::vector<std::string> line_list;
  while (std::getline(input_, line)) {
    str_util::trim(line);
    // LOG_DEBUG("line " << line);
    if (line ==  "<prsm>") {
      line_list.push_back(line);
    } else if (line == "</prsm>") {
      if (line_list.size() != 0) {
        line_list.push_back(line);
      }
      return line_list;
    } else if (line == "") {
      continue;
    } else {
      if (line_list.size() > 0) {
        line_list.push_back(line);
      }
    }
  }
  return line_list;
}

PrsmStrPtr PrsmReader::readOnePrsmStr() {
  std::vector<std::string> prsm_str_vec = readOnePrsmLines();
  if (prsm_str_vec.size() == 0) {
    return PrsmStrPtr(nullptr);
  }
  return std::make_shared<PrsmStr>(prsm_str_vec);
}

PrsmPtr PrsmReader::readOnePrsm(FastaIndexReaderPtr reader_ptr,
                                const ModPtrVec fix_mod_list) {
  std::vector<std::string> prsm_str_vec = readOnePrsmLines();
  if (prsm_str_vec.size() == 0) {
    return PrsmPtr(nullptr);
  }
  std::string prsm_str = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
  for (size_t i = 0; i < prsm_str_vec.size(); i++) {
    prsm_str += prsm_str_vec[i];
  }
  // LOG_DEBUG("prsm str " << prsm_str);
  xercesc::MemBufInputSource prsm_buf(
      (const XMLByte*)prsm_str.c_str(), prsm_str.size(), "prsm_str (in memory)");

  XmlDOMParser* parser = XmlDOMParserFactory::getXmlDOMParserInstance();
  PrsmPtr ptr;
  if (parser) {
    XmlDOMDocument doc(parser, prsm_buf);
    XmlDOMElement* root = doc.getDocumentElement();
    ptr = std::make_shared<Prsm>(root, reader_ptr, fix_mod_list);
  }
  return ptr;
}

void PrsmReader::close() {
  if (input_.is_open()) {
    input_.close();
  }
}

}  // namespace toppic
