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


#ifndef TOPPIC_SEARCH_GRAPH_ALIGN_GRAPH_ALIGN_HPP_
#define TOPPIC_SEARCH_GRAPH_ALIGN_GRAPH_ALIGN_HPP_

#include "seq/fasta_seq.hpp"
#include "prsm/prsm.hpp"
#include "search/diag/diag_header.hpp"
#include "search/graph/graph.hpp"
#include "search/graph/proteo_graph.hpp"
#include "search/graph/spec_graph.hpp"
#include "search/graphalign/graph_dp_node.hpp"
#include "search/graphalign/graph_result_node.hpp"
#include "search/graphalign/graph_align_mng.hpp"

namespace toppic {

typedef std::vector<std::vector<std::vector<std::vector<std::pair<int, int>>>>> ConsistentPairs;

class GraphAlign {
 public:
  GraphAlign(GraphAlignMngPtr mng_ptr, 
             ProteoGraphPtr proteo_graph_ptr,
             SpecGraphPtr spec_graph_ptr,
             FastaSeqPtr fasta_seq_ptr);

  void process();

  PrsmPtr geneResult(int s, int m);

  PrsmPtr geneResult(int s);

  int getBestScore(){return best_score_;};
  void setBestScore(int s){best_score_ = s;};

 private:
  GraphAlignMngPtr mng_ptr_;

  ProteoGraphPtr proteo_graph_ptr_;

  MassGraphPtr pg_;

  FastaSeqPtr fasta_seq_ptr_;

  int proteo_ver_num_;

  SpecGraphPtr spec_graph_ptr_;

  MassGraphPtr sg_;

  int spec_ver_num_;

  int n_unknown_shift_;

  DistVec spec_dist_;

  DistVec2D dist_vec_;

  ConsistentPairs cons_pairs_;

  GraphDpNodePtrVec2D table_;

  GraphResultNodePtrVec3D result_nodes_;

  GraphResultNodePtrVec2D nodes_2d_;

  DiagHeaderPtrVec diag_headers_; 

  DiagHeaderPtrVec2D diag_headers_2d_;

  int best_score_;

  void getConsistentPairs();

  void addToConsistentPairs(int m, const std::vector<std::pair<int, int>> & sp_pair_ij,
                            const std::vector<std::pair<int, int>> & pg_pair_ij);

  void initTable();

  GraphDpNodePtr compBestVariableNode(int i, int j, int s, int m, int &best_edge_mod_num);

  GraphDpNodePtr compBestShiftNode(int i, int j, int s, int m);

  void updateBestShiftNode(int i, int j, int s, int m);

  void dp();

  GraphResultNodePtrVec backtrace(int s, int m);

  void backtrace();

  void getNodeDiagonals(int s, int m);

  void geneHeaders();
};

typedef std::shared_ptr<GraphAlign> GraphAlignPtr;

}  // namespace toppic

#endif

