//
// Created by zhiwei zeng on 24/02/2023.
//


#include <string>
#include <iostream>
#include "graph/graph.h"
#include "EvaluateQuery.h"


#ifndef SUBGRAPHMATCHING_EXPERIMENTS_H
#define SUBGRAPHMATCHING_EXPERIMENTS_H


class Experiments {
public:
    static std::string datagraphEigenMatrix;
    static bool candidate_set_correctnesscheck(vector<set<ui>> candidate,vector<set<ui>> candidate_true,ui query_size);
    static std::string experiment1(Graph *data_graph, Graph *query_graph);
    static void experiment2(std::string data_graph,std::string query_graph,std::string eigen);
    static void experiment3(const std::string data_graph_path,const std::string query_graph_path,const std::string eigen);
    static void experiment4(const std::string data_graph_path,const std::string query_graph_path,const std::string eigen);
};


#endif //SUBGRAPHMATCHING_EXPERIMENTS_H
