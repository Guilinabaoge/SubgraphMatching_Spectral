//
// Created by zhiwei zeng on 24/02/2023.
//

#include "graph/graph.h"

#ifndef SUBGRAPHMATCHING_STUDYPERFORMANCE_CPP_H
#define SUBGRAPHMATCHING_STUDYPERFORMANCE_CPP_H

class StudyPerformance{
public:
    static size_t enumerate(Graph* data_graph, Graph* query_graph, Edges*** edge_matrix, ui** candidates, ui* candidates_count,
                            ui* matching_order, size_t output_limit);
    static void spectrum_analysis(Graph* data_graph, Graph* query_graph, Edges*** edge_matrix, ui** candidates, ui* candidates_count,
                                  size_t output_limit, std::vector<std::vector<ui>>& spectrum, size_t time_limit_in_sec);
    static int solveGraphQuery(string dgraph_path, string qgraph_path, string filter, string order, string engine,
                        string eigen, string tops, int* candidate_vertices, int* result_vertices,int* embedding_cnt);

};

#endif //SUBGRAPHMATCHING_STUDYPERFORMANCE_CPP_H
