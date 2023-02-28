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

    //generate_datagraph_eigenvector("../../test/reallife_dataset/dblp/data_graph/dblp.graph","dplp.csv");

    //yeast,hprd,uspatents,youtube,dblp,eu2005,
    for(int i=1;i<11;i++){
        vector<string> query_size_dense_1 = {"4","8","16","24","32"};
        vector<string> query_size_sparse_1 = {"8","16","24","32"};

        //human,wordnet
        vector<string> query_size_dense_2 = {"4","8","12","16","20"};
        vector<string> query_size_sparse_2 = {"8","12","16","20"};


        Experiments::datagraphEigenMatrix = "wordnet.csv";
        queryMeta meta;
        meta.dataset = "wordnet";
        meta.query_property = "dense";
        meta.query_size = query_size_dense_2[0];
        meta.query_number = to_string(i);

        std::ostringstream data;
        data << "../../test/reallife_dataset/" << meta.dataset << "/data_graph/" << meta.dataset << ".graph";
        meta.data_graph_path = data.str();

        std::ostringstream query;
        query << "../../test/reallife_dataset/" << meta.dataset << "/query_graph/query_" << meta.query_property << "_"
              << meta.query_size << "_" << meta.query_number << ".graph";
        meta.query_path = query.str();


        //Query_Name,LDF,LDF+EF,NLF,NLF+EF,GQL,GQL+EF,TSOF,TSOF+EF,CFL,CFL+EF,DPiso,Dpiso+EF,KF

        pair<string,string> LDF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"LDF","0");
        pair<string,string> LDF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"LDF","1");
        pair<string,string> NLF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"NLF","0");
        pair<string,string> NLF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"NLF","1");
        pair<string,string> GQL = Experiments::experiment3(meta.data_graph_path,meta.query_path,"GQL","0");
        pair<string,string> GQL_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"GQL","1");
        pair<string,string> TSOF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"TSO","0");
        pair<string,string> TSOF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"TSO","1");
        pair<string,string> CFL = Experiments::experiment3(meta.data_graph_path,meta.query_path,"CFL","0");
        pair<string,string> CFL_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"CFL","1");
        pair<string,string> DPiso = Experiments::experiment3(meta.data_graph_path,meta.query_path,"DPiso","0");
        pair<string,string> DPiso_EF =Experiments::experiment3(meta.data_graph_path,meta.query_path,"DPiso","1");
        pair<string,string> KF =Experiments::experiment3(meta.data_graph_path,meta.query_path,"KF","0");

        std::ostringstream oss;
        oss <<meta.query_property<<"_"<<meta.query_size<<"_"<<meta.query_number<<"," <<LDF.first<<"," <<LDF_EF.first<<","
            <<NLF.first<<"," <<NLF_EF.first<<","<<GQL.first<<"," <<GQL_EF.first<<"," <<TSOF.first<<"," <<TSOF_EF.first<<"," <<CFL.first<<"," <<CFL_EF.first<<","
            <<DPiso.first<<"," <<DPiso_EF.first<<"," <<KF.first<<","<<LDF.second<<","<<LDF_EF.second<<","<<NLF.second<<","<<NLF_EF.second<<","<<GQL.second<<","<<GQL_EF.second<<","<<TSOF.second<<","<<TSOF_EF.second
            <<","<<CFL.second<<","<<CFL_EF.second<<","<<DPiso.second<<","<<DPiso_EF.second<<","<<
            KF.second;
        std::string var = oss.str();

        cout<<var<<endl;

        std::ofstream myfile;
        myfile.open ("performance_experiment/wordnet.csv",std::ios_base::app);
        myfile<<var<<"\n";
        myfile.close();
    }





    //for(int i = 1; i<201;i++ ) {
    //    Experiments::datagraphEigenMatrix = "yeast.csv";
    //    queryMeta meta;
    //    meta.dataset = "yeast";
    //    meta.query_property = "sparse";
    //    meta.query_size = query_size_sparse_1[3];
    //    meta.query_number = to_string(i);
    //
    //    std::ostringstream data;
    //    data << "../../test/reallife_dataset/" << meta.dataset << "/data_graph/" << meta.dataset << ".graph";
    //    meta.data_graph_path = data.str();
    //
    //    std::ostringstream query;
    //    query << "../../test/reallife_dataset/" << meta.dataset << "/query_graph/query_" << meta.query_property << "_"
    //          << meta.query_size << "_" << meta.query_number << ".graph";
    //    meta.query_path = query.str();
    //
    //    Experiments::experiment4("0", meta);
    //    cout<<i<<endl;
    //}


    }
