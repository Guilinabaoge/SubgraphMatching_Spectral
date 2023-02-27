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
//    string data_graph_path = "../../test/reallife_dataset/wordnet/data_graph/wordnet.graph";
//    string query_graph_path = "../../test/reallife_dataset/wordnet/query_graph/query_dense_16_2.graph";
    Experiments::datagraphEigenMatrix = "yeast.csv";
    queryMeta meta;
    meta.dataset = "yeast";
    meta.query_property = "dense";
    meta.query_size = "4";
    meta.query_number = "1";

    std::ostringstream data;
    data << "../../test/reallife_dataset/"<<meta.dataset<<"/data_graph/"<<meta.dataset<< ".graph";
    std::string data_graph_path = data.str();

    std::ostringstream query;
    query << "../../test/reallife_dataset/"<<meta.dataset<<"/query_graph/query_"<<meta.query_property<<"_" << meta.query_size <<"_" << meta.query_number<< ".graph";
    std::string query_graph_path = query.str();

//    Experiments::experiment3(data_graph_path,query_graph_path,"0");
    Experiments::experiment4(data_graph_path,query_graph_path,"0",meta);

    vector<set<ui>> ground_truth = Experiments::ground_truth_interpreter("ground_truth/yeast/yeast_dense_4_1.txt");


}
