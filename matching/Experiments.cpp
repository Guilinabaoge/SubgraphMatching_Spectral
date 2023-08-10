//
// Created by zhiwei zeng on 24/02/2023.
//

#include "Experiments.h"
#include "IO.h"
#include "StudyPerformance.h"
#include "FilterVertices.h"

#include <iostream>
#include <numeric>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>

//string Experiments::datagraphEigenMatrix;
Eigen::MatrixXd Experiments::datagraphEigenMatrix;


//This experiment compare the overall performance of each algorithm with and without eigen value enhanced filter.
matching_algo_outputs Experiments::experiment3(const string data_graph_path,const string query_graph_path
                                               ,const string filter,const string eigen,ui* order_pointer,const string order) {

//    string filters[6] = {"LDF","NLF","GQL","TSO","CFL","DPiso"};

    matching_algo_inputs inputs;
    inputs.dgraph_path = data_graph_path;
    inputs.qgraph_path = query_graph_path;
    inputs.filter = filter;
    inputs.order = order;
    inputs.engine = "LFTJ";
    inputs.eigen = eigen;
    inputs.order_pointer = order_pointer;


    matching_algo_outputs outputs = StudyPerformance::solveGraphQuery(inputs);

    if(stoi(eigen)){
        cout<<"Matches: "<<outputs.enumOutput.embedding_cnt<<filter<<"call_count "<<outputs.call_count <<" With eigen filter total time "<<outputs.total_time<<" enumeration time: "
        <<outputs.enumeration_time<<" preprocessing time: "<<outputs.preprocessing_time<<" candidate_sum: "<<outputs.candidate_count_sum<<" matching order: ";

        cout<<" "<<endl;
    } else{
        cout<<"Matches: "<<outputs.enumOutput.embedding_cnt<<filter<<"call_count "<<outputs.call_count<<" No eigen filter total time "<<outputs.total_time<<" enumeration time: "
        <<outputs.enumeration_time<<" preprocessing time: "<<outputs.preprocessing_time<<" candidate_sum: "<<outputs.candidate_count_sum<<" matching order: ";

        cout<<" "<<endl;
    }

    return outputs;

}


