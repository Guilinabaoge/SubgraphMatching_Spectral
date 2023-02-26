//
// Created by zhiwei zeng on 24/02/2023.
//

#include "graph/graph.h"
#include "EvaluateQuery.h"

#ifndef SUBGRAPHMATCHING_STUDYPERFORMANCE_CPP_H
#define SUBGRAPHMATCHING_STUDYPERFORMANCE_CPP_H

struct matching_algo_inputs {
    string dgraph_path;
    string qgraph_path;
    string filter;
    string order;
    string engine;
    string eigen;
};

struct matching_algo_outputs{
    enumResult enumOutput;
    ui query_size;
    vector<set<ui>> candidate;
    ui candidate_count_sum;
    double total_time;
};

class StudyPerformance{
public:
    static size_t enumerate(Graph* data_graph, Graph* query_graph, Edges*** edge_matrix, ui** candidates, ui* candidates_count,
                            ui* matching_order, size_t output_limit);
    static void spectrum_analysis(Graph* data_graph, Graph* query_graph, Edges*** edge_matrix, ui** candidates, ui* candidates_count,
                                  size_t output_limit, std::vector<std::vector<ui>>& spectrum, size_t time_limit_in_sec);
    static matching_algo_outputs solveGraphQuery(matching_algo_inputs inputs);

};

#endif //SUBGRAPHMATCHING_STUDYPERFORMANCE_CPP_H