//
// Created by zhiwei zeng on 25/02/2023.
//
#include <chrono>
#include <future>
#include <thread>
#include <fstream>
#include <iostream>
#include <string>

#include "graph/graph.h"
#include "FilterVertices.h"
#include "IO.h"
#include "eigenHelper.h"
#include "Experiments.h"
#include "StudyPerformance.h"

void generate_datagraph_eigenvector(string data_graph_path,string csvfilename){
    Graph* data_graph = new Graph(true);
    data_graph->loadGraphFromFile(data_graph_path);

    MatrixXd datagraph_eigenvalue(data_graph->getVerticesCount(), 35);
    cout<<"Start compute eigen value"<<endl;
    MTcalc12(data_graph,data_graph->getGraphMaxDegree(),datagraph_eigenvalue,true,35);
    saveData(csvfilename, datagraph_eigenvalue);

}


int main(int argc, char** argv) {
    string data_graph_path = "../../test/reallife_dataset/yeast/data_graph/yeast.graph";
    string query_graph_path = "../../test/reallife_dataset/yeast/query_graph/query_dense_8_1.graph";

    cout<<"with EF"<<endl;
    Experiments::experiment2(data_graph_path,query_graph_path,"1","8");
    cout<<"no EF"<<endl;
    Experiments::experiment2(data_graph_path,query_graph_path,"0","8");
}
