//
// Created by zhiwei zeng on 24/02/2023.
//

#include "Experiments.h"
#include "IO.h"
#include "StudyPerformance.h"
#include "FilterVertices.h"

#include <iostream>
#include <numeric>

string Experiments::datagraphEigenMatrix = "yeast.csv";

string Experiments::experiment1(Graph *data_graph, Graph *query_graph){
    ui** candidates = NULL;
    ui* candidates_count = NULL;
    ui* tso_order = NULL;
    TreeNode* tso_tree = NULL;
    ui* cfl_order = NULL;
    TreeNode* cfl_tree = NULL;
    ui* dpiso_order = NULL;
    TreeNode* dpiso_tree = NULL;
    TreeNode* ceci_tree = NULL;
    ui* ceci_order = NULL;
    std::vector<std::unordered_map<VertexID, std::vector<VertexID >>> TE_Candidates;
    std::vector<std::vector<std::unordered_map<VertexID, std::vector<VertexID>>>> NTE_Candidates;

    bool (*functptr[7])(Graph*, Graph* , ui**& , ui*&, ui*&,TreeNode *&,std::vector<std::unordered_map<VertexID, std::vector<VertexID >>> &,std::vector<std::vector<std::unordered_map<VertexID, std::vector<VertexID>>>> &NTE_Candidates,bool,int);
    functptr[0] = FilterVertices::LDFWrapper;
    functptr[1] = FilterVertices::NLFWrapper;
    functptr[2] = FilterVertices::GQLWrapper;
    functptr[3] = FilterVertices::TSOWrapper;
    functptr[4] = FilterVertices::CFLWrapper;
    functptr[5] = FilterVertices::DPisoWrapper;
    functptr[6] = FilterVertices::CECIFilter;

    int sum=0;
    int top_s = 10;
    if(query_graph->getVerticesCount()==4){
        top_s = 4;
    }
    if(query_graph->getVerticesCount()==8){
        top_s = 8;
    }

    static int results[14];
    int counter = 0;

    for(int i =0; i<7;i++){
        functptr[i](data_graph, query_graph, candidates, candidates_count,ceci_order,ceci_tree,TE_Candidates,NTE_Candidates,true,top_s);
        results[counter] = accumulate(candidates_count, candidates_count+query_graph->getVerticesCount(), sum);
        counter++;
        functptr[i](data_graph, query_graph, candidates, candidates_count,ceci_order,ceci_tree,TE_Candidates,NTE_Candidates,false,top_s);
        results[counter] = accumulate(candidates_count, candidates_count+query_graph->getVerticesCount(), sum);
        counter++;
    }

    string results_string = "";
    for (int i=0; i<14; i++){
        results_string.append(std::to_string(results[i])+",");
    }
    results_string.pop_back();
    results_string.append("\n");

    return results_string;
}


void Experiments::experiment2(string data_graph,string query_graph,string eigen,string top_s){
    //In this experiment I find out the ground truth and the candidate for each vertex.

    // -d ../../test/reallife_dataset/wordnet/data_graph/wordnet.graph -q ../../test/reallife_dataset/wordnet/query_graph/query_dense_12_2.graph -filter GQL -order GQL -engine LFTJ -num MAX -eigen 1 -tops 10
    //TODO Missing CECI
    string filters[6] = {"LDF","NLF","GQL","TSO","CFL","DPiso"};

    int candidate_counts[6];
    int results[6];
    int embeddings[6];


    for(int i = 0; i<6;i++){
        cout<<i<<endl;
        int* candidate_count = new int;
        int* candidate_true = new int;
        int* embedding_cnt = new int;
        StudyPerformance::solveGraphQuery(data_graph, query_graph, filters[i], "GQL", "LFTJ", eigen, top_s,
                        candidate_count, candidate_true,embedding_cnt);
        candidate_counts[i] = *candidate_count;
        results[i]= *candidate_true;
        embeddings[i] = *embedding_cnt;

        delete candidate_count;
        delete candidate_true;

    }

    cout<<"--------------------------"<<endl;
    cout<<"Candidates"<<endl;
    for(int i=0; i<6;i++){
        cout<<candidate_counts[i]<<",";
    }
    cout<<" "<<endl;
    cout<<"Candidates_true"<<endl;
    for(int i=0; i<6;i++){
        cout<<results[i]<<",";
    }
    cout<<" "<<endl;
    cout<<"Embedding count"<<endl;
    for(int i=0; i<6;i++){
        cout<<embeddings[i]<<",";
    }
    cout<<" "<<endl;
    cout<<"--------------------------"<<endl;
}


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
//        string results = to_string(i)+",";
//        string results2 = results.append(experiment(data_graph,query_graph));
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
