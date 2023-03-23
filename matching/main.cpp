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


void exact_eval(string dataset,string querysize,string querynumber,string property){
    Experiments::datagraphEigenMatrix = dataset+".csv";
    queryMeta meta;
    meta.dataset = dataset;
    meta.query_property = property;
    meta.query_size = querysize;
    meta.query_number = querynumber;

    std::ostringstream data;
    data << "../../test/reallife_dataset/" << meta.dataset << "/data_graph/" << meta.dataset << ".graph";
    meta.data_graph_path = data.str();

    std::ostringstream query;
    query << "../../test/reallife_dataset/" << meta.dataset << "/query_graph/query_" << meta.query_property << "_"
          << meta.query_size << "_" << meta.query_number << ".graph";
    meta.query_path = query.str();

    pair <matching_algo_outputs,matching_algo_outputs> LDF = fakeMatchingWrapper(meta,"LDF");
    pair <matching_algo_outputs,matching_algo_outputs> NLF = fakeMatchingWrapper(meta,"NLF");
    pair <matching_algo_outputs,matching_algo_outputs> GQL = fakeMatchingWrapper(meta,"GQL");
    pair <matching_algo_outputs,matching_algo_outputs> TSOF = fakeMatchingWrapper(meta,"TSO");
    pair <matching_algo_outputs,matching_algo_outputs> CFL = fakeMatchingWrapper(meta,"CFL");
    pair <matching_algo_outputs,matching_algo_outputs> DPiso = fakeMatchingWrapper(meta,"DPiso");
    matching_algo_outputs KF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"KF","0",NULL);

    vector<pair<matching_algo_outputs,matching_algo_outputs>> evaluations;
    evaluations.push_back(LDF);
    evaluations.push_back(NLF);
    evaluations.push_back(GQL);
    evaluations.push_back(TSOF);
    evaluations.push_back(CFL);
    evaluations.push_back(DPiso);


    std::ostringstream oss;
    oss <<meta.query_property<<"_"<<meta.query_size<<"_"<<meta.query_number;

    for(auto &eval : evaluations){
        oss<<","<<eval.first.call_count<<","<<eval.second.call_count;
    }
    oss<<","<<KF.call_count
    <<","<<LDF.first.enumOutput.embedding_cnt;

    for(auto &eval : evaluations){
        oss<<","<<eval.first.total_time<<","<<eval.second.total_time;
    }
    oss<<","<<KF.total_time<<","<<LDF.first.enumOutput.embedding_cnt;

    for(auto &eval : evaluations){
        oss<<","<<eval.first.candidate_count_sum<<","<<eval.second.candidate_count_sum;
    }
    oss<<","<<KF.candidate_count_sum;
    for(auto &eval : evaluations){
        oss<<","<<eval.first.matching_order_string<<","<<eval.second.matching_order_string;
    }
    oss<<","<<KF.matching_order_string;
    for(auto &eval : evaluations){
        oss<<","<<eval.first.preprocessing_time<<","<<eval.second.preprocessing_time;
    }
    oss<<","<<KF.preprocessing_time;
    for(auto &eval : evaluations){
        oss<<","<<eval.first.enumeration_time<<","<<eval.second.enumeration_time;
    }
    oss<<","<<KF.enumeration_time;

//    oss <<meta.query_property<<"_"<<meta.query_size<<"_"<<meta.query_number<<"," <<LDF.first.total_time<<"," <<LDF.second.total_time
//        <<","<<NLF.first.total_time<<"," <<NLF.second.total_time<<","<<GQL.first.total_time<<"," <<GQL.second.total_time<<"," <<TSOF.first.total_time
//        <<"," <<TSOF.second.total_time<<"," <<CFL.first.total_time
//        <<"," <<CFL.second.total_time<<","<<DPiso.first.total_time
//        <<"," <<DPiso.second.total_time<<"," <<KF.total_time
//        <<","<<LDF.first.enumOutput.embedding_cnt
//        <<","<<LDF.first.candidate_count_sum
//        <<","<<LDF.second.candidate_count_sum
//        <<","<<NLF.first.candidate_count_sum
//        <<","<<NLF.second.candidate_count_sum
//        <<","<<GQL.first.candidate_count_sum
//        <<","<<GQL.second.candidate_count_sum
//        <<","<<TSOF.first.candidate_count_sum
//        <<","<<TSOF.second.candidate_count_sum
//        <<","<<CFL.first.candidate_count_sum
//        <<","<<CFL.second.candidate_count_sum
//        <<","<<DPiso.first.candidate_count_sum
//        <<","<<DPiso.second.candidate_count_sum
//        <<","<<KF.matching_order_string
//        <<","<<LDF.first.matching_order_string
//        <<","<<LDF.second.matching_order_string
//        <<","<<NLF.first.matching_order_string
//        <<","<<NLF.second.matching_order_string
//        <<","<<GQL.first.matching_order_string
//        <<","<<GQL.second.matching_order_string
//        <<","<<TSOF.first.matching_order_string
//        <<","<<TSOF.second.matching_order_string
//        <<","<<CFL.first.matching_order_string
//        <<","<<CFL.second.matching_order_string
//        <<","<<DPiso.first.matching_order_string
//        <<","<<DPiso.second.matching_order_string
//        <<","<<KF.matching_order_string;

    std::string var = oss.str();

    cout<<var<<endl;

    string file_path = "";
    if(property=="sparse"){
        file_path = "performance_experiment/"+dataset+"_"+"s"+querysize+".csv";
    }
    if(property=="dense"){
        file_path = "performance_experiment/"+dataset+"_"+"d"+querysize+".csv";
    }

    std::ofstream myfile;
    myfile.open (file_path,std::ios_base::app);
    myfile<<var<<"\n";
    myfile.close();

}

void generate_datagraph_eigenvector(string data_graph_path,string csvfilename){
    Graph* data_graph = new Graph(true);
    data_graph->loadGraphFromFile(data_graph_path);

    MatrixXd datagraph_eigenvalue(data_graph->getVerticesCount(), 35);
    cout<<"Start compute eigen value"<<endl;
    MTcalc12(data_graph,data_graph->getGraphMaxDegree(),datagraph_eigenvalue,true,35);
    saveData(csvfilename, datagraph_eigenvalue);

}


int main(int argc, char** argv) {
    //    generate_datagraph_eigenvector("../../test/reallife_dataset/eu2005/data_graph/eu2005.graph","eu2005.csv");

    //yeast,hprd,uspatents,youtube,dblp,eu2005,
    vector<string> query_size_dense_1 = {"4","8","16","24","32"};
    vector<string> query_size_sparse_1 = {"8","16","24","32"};

    //human,wordnet
    vector<string> query_size_dense_2 = {"4","8","12","16","20"};
    vector<string> query_size_sparse_2 = {"8","12","16","20"};

    MatchingCommand command(argc,argv);
    string dataset_name = command.getDatasetName();
    string query_size = command.getQuerySize();
    string query_number = command.getQueryNumber();
    string query_property = command.getQueryProperty();

    cout<<dataset_name<<endl;
    cout<<query_size<<endl;
    cout<<query_number<<endl;
    cout<<query_property<<endl;

    exact_eval(dataset_name,query_size,query_number,query_property);

    return 0;

}


























//for(int i=2; i<5;i++){
//    for(int j=10;j<11;j++){
//        //yeast,hprd,uspatents,youtube,dblp,eu2005,
//        vector<string> query_size_dense_1 = {"4","8","16","24","32"};
//        vector<string> query_size_sparse_1 = {"8","16","24","32"};
//
//        //human,wordnet
//        vector<string> query_size_dense_2 = {"4","8","12","16","20"};
//        vector<string> query_size_sparse_2 = {"8","12","16","20"};
//
//
//        Experiments::datagraphEigenMatrix = "youtube.csv";
//        queryMeta meta;
//        meta.dataset = "youtube";
//        meta.query_property = "dense";
//        meta.query_size = query_size_dense_1[i];
//        meta.query_number = to_string(j);
//
//        std::ostringstream data;
//        data << "../../test/reallife_dataset/" << meta.dataset << "/data_graph/" << meta.dataset << ".graph";
//        meta.data_graph_path = data.str();
//
//        std::ostringstream query;
//        query << "../../test/reallife_dataset/" << meta.dataset << "/query_graph/query_" << meta.query_property << "_"
//        << meta.query_size << "_" << meta.query_number << ".graph";
//        meta.query_path = query.str();
//
//
//        //Query_Name,LDF,LDF+EF,NLF,NLF+EF,GQL,GQL+EF,TSOF,TSOF+EF,CFL,CFL+EF,DPiso,DPiso+EF,KF
//
//        matching_algo_outputs LDF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"LDF","0");
//        matching_algo_outputs LDF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"LDF","1");
//        matching_algo_outputs NLF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"NLF","0");
//        matching_algo_outputs NLF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"NLF","1");
//        matching_algo_outputs GQL = Experiments::experiment3(meta.data_graph_path,meta.query_path,"GQL","0");
//        matching_algo_outputs GQL_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"GQL","1");
//        matching_algo_outputs TSOF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"TSO","0");
//        matching_algo_outputs TSOF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"TSO","1");
//        matching_algo_outputs CFL = Experiments::experiment3(meta.data_graph_path,meta.query_path,"CFL","0");
//        matching_algo_outputs CFL_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"CFL","1");
//        matching_algo_outputs DPiso = Experiments::experiment3(meta.data_graph_path,meta.query_path,"DPiso","0");
//        matching_algo_outputs DPiso_EF =Experiments::experiment3(meta.data_graph_path,meta.query_path,"DPiso","1");
//        matching_algo_outputs KF =Experiments::experiment3(meta.data_graph_path,meta.query_path,"KF","0");
//
//        std::ostringstream oss;
//        oss <<meta.query_property<<"_"<<meta.query_size<<"_"<<meta.query_number<<"," <<LDF.total_time<<"," <<LDF_EF.total_time
//        <<","<<NLF.total_time<<"," <<NLF_EF.total_time<<","<<GQL.total_time<<"," <<GQL_EF.total_time<<"," <<TSOF.total_time<<"," <<TSOF_EF.total_time<<"," <<CFL.total_time
//        <<"," <<CFL_EF.total_time<<","<<DPiso.total_time<<"," <<DPiso_EF.total_time<<"," <<KF.total_time<<","<<LDF.enumOutput.embedding_cnt<<","<<LDF_EF.enumOutput.embedding_cnt
//        <<","<<NLF.enumOutput.embedding_cnt<<","<<NLF_EF.enumOutput.embedding_cnt<<","<<GQL.enumOutput.embedding_cnt
//        <<","<<GQL_EF.enumOutput.embedding_cnt<<","<<TSOF.enumOutput.embedding_cnt<<","<<TSOF_EF.enumOutput.embedding_cnt<<","<<CFL.enumOutput.embedding_cnt
//        <<","<<CFL_EF.enumOutput.embedding_cnt<<","<<DPiso.enumOutput.embedding_cnt<<","<<DPiso_EF.enumOutput.embedding_cnt<<","<<KF.enumOutput.embedding_cnt
//        <<","<<LDF.candidate_count_sum<<","<<LDF.enumOutput.candidate_true_count_sum<<","<<LDF_EF.candidate_count_sum<<","<<LDF_EF.enumOutput.candidate_true_count_sum
//        <<","<<NLF.candidate_count_sum<<","<<NLF.enumOutput.candidate_true_count_sum<<","<<NLF_EF.candidate_count_sum<<","<<NLF_EF.enumOutput.candidate_true_count_sum
//        <<","<<GQL.candidate_count_sum<<","<<GQL.enumOutput.candidate_true_count_sum<<","<<GQL_EF.candidate_count_sum<<","<<GQL_EF.enumOutput.candidate_true_count_sum
//        <<","<<TSOF.candidate_count_sum<<","<<TSOF.enumOutput.candidate_true_count_sum<<","<<TSOF_EF.candidate_count_sum<<","<<TSOF_EF.enumOutput.candidate_true_count_sum
//        <<","<<CFL.candidate_count_sum<<","<<CFL.enumOutput.candidate_true_count_sum<<","<<CFL_EF.candidate_count_sum<<","<<CFL_EF.enumOutput.candidate_true_count_sum
//        <<","<<DPiso.candidate_count_sum<<","<<DPiso.enumOutput.candidate_true_count_sum<<","<<DPiso_EF.candidate_count_sum<<","<<DPiso_EF.enumOutput.candidate_true_count_sum
//        <<","<<KF.candidate_count_sum<<","<<KF.enumOutput.candidate_true_count_sum;
//
//        std::string var = oss.str();
//
//        cout<<var<<endl;
//
//        std::ofstream myfile;
//        myfile.open ("performance_experiment/youtube.csv",std::ios_base::app);
//        myfile<<var<<"\n";
//        myfile.close();
//    }
//}

//
//for(int i=4; i<5;i++){
//for(int j=1;j<36;j++){
////yeast,hprd,uspatents,youtube,dblp,eu2005,
//vector<string> query_size_dense_1 = {"4","8","16","24","32"};
//vector<string> query_size_sparse_1 = {"8","16","24","32"};
//
////human,wordnet
//vector<string> query_size_dense_2 = {"4","8","12","16","20"};
//vector<string> query_size_sparse_2 = {"8","12","16","20"};
//
//
//Experiments::datagraphEigenMatrix = "wordnet.csv";
//queryMeta meta;
//meta.dataset = "wordnet";
//meta.query_property = "dense";
//meta.query_size = query_size_dense_2[i];
//meta.query_number = to_string(j);
//
//std::ostringstream data;
//data << "../../test/reallife_dataset/" << meta.dataset << "/data_graph/" << meta.dataset << ".graph";
//meta.data_graph_path = data.str();
//
//std::ostringstream query;
//query << "../../test/reallife_dataset/" << meta.dataset << "/query_graph/query_" << meta.query_property << "_"
//<< meta.query_size << "_" << meta.query_number << ".graph";
//meta.query_path = query.str();
//
//
////Query_Name,LDF,LDF+EF,NLF,NLF+EF,GQL,GQL+EF,TSOF,TSOF+EF,CFL,CFL+EF,DPiso,DPiso+EF,KF
//
//matching_algo_outputs LDF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"LDF","0");
//matching_algo_outputs LDF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"LDF","1");
//matching_algo_outputs NLF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"NLF","0");
//matching_algo_outputs NLF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"NLF","1");
//matching_algo_outputs GQL = Experiments::experiment3(meta.data_graph_path,meta.query_path,"GQL","0");
//matching_algo_outputs GQL_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"GQL","1");
//matching_algo_outputs TSOF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"TSO","0");
//matching_algo_outputs TSOF_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"TSO","1");
//matching_algo_outputs CFL = Experiments::experiment3(meta.data_graph_path,meta.query_path,"CFL","0");
//matching_algo_outputs CFL_EF = Experiments::experiment3(meta.data_graph_path,meta.query_path,"CFL","1");
//matching_algo_outputs DPiso = Experiments::experiment3(meta.data_graph_path,meta.query_path,"DPiso","0");
//matching_algo_outputs DPiso_EF =Experiments::experiment3(meta.data_graph_path,meta.query_path,"DPiso","1");
//matching_algo_outputs KF =Experiments::experiment3(meta.data_graph_path,meta.query_path,"KF","0");
//
//std::ostringstream oss;
//oss <<meta.query_property<<"_"<<meta.query_size<<"_"<<meta.query_number
//<<","<<LDF.candidate_count_sum<<","<<LDF_EF.candidate_count_sum
//<<","<<NLF.candidate_count_sum<<","<<NLF_EF.candidate_count_sum
//<<","<<GQL.candidate_count_sum<<","<<GQL_EF.candidate_count_sum
//<<","<<TSOF.candidate_count_sum<<","<<TSOF_EF.candidate_count_sum
//<<","<<CFL.candidate_count_sum<<","<<CFL_EF.candidate_count_sum
//<<","<<DPiso.candidate_count_sum<<","<<DPiso_EF.candidate_count_sum;
//
//std::string var = oss.str();
//
//cout<<var<<endl;
//
//std::ofstream myfile;
//myfile.open ("pruning_power_experiment/wordnet.csv",std::ios_base::app);
//myfile<<var<<"\n";
//myfile.close();
//}
//}