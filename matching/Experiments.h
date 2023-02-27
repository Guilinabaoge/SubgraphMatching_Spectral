//
// Created by zhiwei zeng on 24/02/2023.
//


#include <string>
#include <iostream>
#include "graph/graph.h"
#include "EvaluateQuery.h"


#ifndef SUBGRAPHMATCHING_EXPERIMENTS_H
#define SUBGRAPHMATCHING_EXPERIMENTS_H

struct queryMeta{
    string dataset;
    string query_property;
    string query_size;
    string query_number;
    string query_path;
    string data_graph_path;
};

class Experiments {
public:
    static std::string datagraphEigenMatrix;
    static vector<set<ui>> ground_truth_interpreter(string path);
    static bool candidate_set_correctness_check(vector<set<ui>> candidate, vector<set<ui>> candidate_true, ui query_size);
    static std::string experiment1(Graph *data_graph, Graph *query_graph);
    static void experiment2(std::string data_graph,std::string query_graph,std::string eigen);
    static void experiment3(const std::string data_graph_path,const std::string query_graph_path,const string filter,const std::string eigen);
    static void experiment4(const std::string eigen,queryMeta meta);
};


#endif //SUBGRAPHMATCHING_EXPERIMENTS_H
