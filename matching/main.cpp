//
// Created by zhiwei zeng on 25/02/2023.
//
#include <chrono>
#include <future>
#include <thread>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#include "matchingcommand.h"
#include "graph/graph.h"
#include "FilterVertices.h"
#include "IO.h"
#include "eigenHelper.h"
#include "Experiments.h"
#include "StudyPerformance.h"

double round_to(double value, double precision = 1.0)
{
    return std::round(value / precision) * precision;
}

pair<matching_algo_outputs,matching_algo_outputs> fakeMatchingWrapper(queryMeta meta,string filter){
    matching_algo_outputs original = Experiments::experiment3(meta.data_graph_path,meta.query_path,filter,"0",NULL);
    ui* fake_pointer = new ui[stoi(meta.query_size)];
    for (int i =0; i<stoi(meta.query_size);i++){
        ui order = original.matching_order[i];
        *&fake_pointer[i] = order;
    }
    matching_algo_outputs enhanced = Experiments::experiment3(meta.data_graph_path,meta.query_path,filter,"1",fake_pointer);
    delete[] fake_pointer;
    return pair(original,enhanced);
}

pair<matching_algo_outputs,matching_algo_outputs> MatchingWrapper(string datagraph,string querygraph,string filter){
    matching_algo_outputs original = Experiments::experiment3(datagraph,querygraph,filter,"0",NULL);
    matching_algo_outputs enhanced = Experiments::experiment3(datagraph,querygraph,filter,"1",NULL);
    return pair(original,enhanced);
}


void generate_datagraph_eigenvector(string data_graph_path,string csvfilename,int size){
    Graph* data_graph = new Graph(true);
    data_graph->loadGraphFromFile(data_graph_path);

    MatrixXd datagraph_eigenvalue(data_graph->getVerticesCount(), size);
    cout<<"Start compute eigen value"<<endl;
    MTcalc12(data_graph,data_graph->getGraphMaxDegree(),datagraph_eigenvalue,true,size);
    saveData(csvfilename, datagraph_eigenvalue);
}



int main(int argc, char** argv) {
   // generate_datagraph_eigenvector("../../test/reallife_dataset/hprd/data_graph/hprd.graph","hprd.csv",35);

    map<std::string, int> dictionary;
    dictionary["LDF"] = 0;
    dictionary["NLF"] = 1;
    dictionary["GQL"] = 2;
    dictionary["TSO"] = 3;
    dictionary["CFL"] = 4;
    dictionary["DPiso"] = 5;

    MatchingCommand command(argc,argv);
    string dataset_name = command.getDatasetName();
    string query_size = command.getQuerySize();
    string query_number = command.getQueryNumber();
    string query_property = command.getQueryProperty();
    string filter = command.getFilterType();
    string isEigencheck = command.getEigenOrNot();
    string wildcard_percentage = command.getWildcardPercentage();

//    Experiments::datagraphEigenMatrix = "../../eigenmatrices/"+dataset_name+".csv";
    Experiments::datagraphEigenMatrix = openData("../../eigenmatrices/"+dataset_name+".csv");
    string datagraph = "../../test/reallife_dataset/"+dataset_name+"/data_graph/"+dataset_name+".graph";
    string querygraph = "../../test/wildcard/wildcard_queries/"+dataset_name+"/"+wildcard_percentage+"/query_"+query_property+"_"+query_size+"_"+query_number+".graph";


    matching_algo_outputs result = Experiments::experiment3(datagraph,querygraph,filter,isEigencheck,NULL);

    std::ostringstream oss;
    oss <<query_property<<"_"<<query_size<<"_"<<query_number<<","<<dictionary[filter]<<","<<isEigencheck;
    oss<<","<<result.call_count;
    oss<<","<<result.total_time;
    oss<<","<<result.candidate_count_sum;
    oss<<","<<result.enumOutput.embedding_cnt;
    oss<<","<<result.matching_order_string;
    oss<<","<<result.preprocessing_time;
    oss<<","<<result.enumeration_time;

    std::string var = oss.str();

    cout<<var<<endl;

    string file_path = "";
    file_path = "../../performance_experiment/"+wildcard_percentage+"/"+dataset_name+"/"+dataset_name+"_"+query_property+query_size+".csv";
    cout<<file_path<<endl;
    std::ofstream myfile;
    myfile.open (file_path,std::ios_base::app);
    myfile<<var<<"\n";
    myfile.close();

    return 0;

}









