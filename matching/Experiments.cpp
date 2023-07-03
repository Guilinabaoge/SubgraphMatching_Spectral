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
matching_algo_outputs Experiments::experiment3(const string data_graph_path,const string query_graph_path,const string filter,const string eigen,ui* order_pointer) {

//    string filters[6] = {"LDF","NLF","GQL","TSO","CFL","DPiso"};

    matching_algo_inputs inputs;
    inputs.dgraph_path = data_graph_path;
    inputs.qgraph_path = query_graph_path;
    inputs.filter = filter;
    inputs.order = "GQL";
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





// Compute the ground truth for each query.
//void Experiments::experiment4(const string data_graph_path, const string query_graph_path, const string eigen) {
//}


//int main(int argc, char** argv){
//    //    MatrixXd datagraph_eigenvalue(data_graph->getVerticesCount(), 35);
//    //    MTcalc12(data_graph,data_graph->getGraphMaxDegree(),datagraph_eigenvalue,true,35);
//    //    saveData("wordnet.csv", datagraph_eigenvalue);
//
//    //  -d ../../test/reallife_dataset/wordnet/data_graph/wordnet.graph -q ../../test/reallife_dataset/wordnet/query_graph/query_dense_8_1.graph -filter GQL -order GQL -engine LFTJ -num MAX -eigen 1 -tops 4
//    int query_number[200];
//    for (int i=0;i<200;i++){
//        query_number[i] = i+1;
//    }
//    //Yeast
//    int query_vertices_dense[5]={4,8,16,24,32};
//    int query_vertices_sparse[4]={8,16,24,32};
//
//    //Wordnet
//    int query_vertices_dense_v2[5]={4,8,12,16,20};
//    int query_vertices_sparse_v2[4]={8,12,16,20};
//
//
//    string querypath[1800];
//    int counter = 0;
//
//    for(int i=0; i<5;i++){
//        for (int j=0;j<200;j++){
//            std::ostringstream oss;
//            oss << "../../test/reallife_dataset/wordnet/query_graph/query_dense_" << query_vertices_dense_v2[i] <<"_" << query_number[j]<< ".graph";
//            std::string var = oss.str();
//            querypath[counter] = var;
//            counter++;
//        }
//    }
//
//    for(int i=0; i<4;i++){
//        for (int j=0;j<200;j++){
//            std::ostringstream oss;
//            oss << "../../test/reallife_dataset/wordnet/query_graph/query_dense_" << query_vertices_sparse_v2[i] <<"_" << query_number[j]<< ".graph";
//            std::string var = oss.str();
//            querypath[counter] = var;
//            counter++;
//        }
//    }
//
//    std::string input_data_graph_file = "../../test/reallife_dataset/wordnet/data_graph/wordnet.graph";
//    Graph* data_graph = new Graph(true);
//    data_graph->loadGraphFromFile(input_data_graph_file);
//
//    std::ofstream myfile;
//    myfile.open ("wordnetexperiment.csv",std::ios_base::app);
//    myfile << "Query_NUmber,LDF+EF,LDF,NLF+EF,NLF,GQL+EF,GQL,TSOF+EF,TSOF,CFL+EF,CFL,DPiso+EF,Dpiso,CECIF+EF,CECIF\n";
//    myfile.close();
//
//    for (int i=771; i<1800;i++){
//        Graph* query_graph = new Graph(true);
//        query_graph->loadGraphFromFile(querypath[i]);
//        query_graph->buildCoreTable();
//        string candidate_true = to_string(i)+",";
//        string results2 = candidate_true.append(experiment(data_graph,query_graph));
//        myfile.open ("wordnetexperiment.csv",std::ios_base::app);
//        myfile<<results2;
//        myfile.close();
//        cout << i << endl;
//    }
//
//
//
//
//
//}
