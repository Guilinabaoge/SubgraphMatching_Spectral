//
// Created by zhiwei zeng on 24/02/2023.
//


#include <string>
#include <iostream>
#include "graph/graph.h"


#ifndef SUBGRAPHMATCHING_EXPERIMENTS_H
#define SUBGRAPHMATCHING_EXPERIMENTS_H


class Experiments {
public:
    static std::string datagraphEigenMatrix;
    static std::string experiment1(Graph *data_graph, Graph *query_graph);
    static void experiment2(std::string data_graph,std::string query_graph,std::string eigen,std::string top_s);
};


#endif //SUBGRAPHMATCHING_EXPERIMENTS_H
