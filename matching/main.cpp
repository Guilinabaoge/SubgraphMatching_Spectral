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
//    query << "../../test/reallife_dataset/" << meta.dataset << "/query_graph/query_" << meta.query_property << "_"
//          << meta.query_size << "_" << meta.query_number << ".graph";
    query<<"../../test/large_query/test.graph";
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

void generate_datagraph_eigenvector(string data_graph_path,string csvfilename,int size){
    Graph* data_graph = new Graph(true);
    data_graph->loadGraphFromFile(data_graph_path);

    MatrixXd datagraph_eigenvalue(data_graph->getVerticesCount(), size);
    cout<<"Start compute eigen value"<<endl;
    MTcalc12(data_graph,data_graph->getGraphMaxDegree(),datagraph_eigenvalue,true,size);
    saveData(csvfilename, datagraph_eigenvalue);

}

void fixed_order_experiment(int argc, char** argv){

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
}

int wildcard(){
    //    generate_datagraph_eigenvector("../../test/reallife_dataset/hprd/data_graph/hprd.graph","hprd.csv",35);
    Experiments::datagraphEigenMatrix = "youtube.csv";
    string datagraph = "../../test/reallife_dataset/youtube/data_graph/youtube.graph";

    string percentage[5] = {"0","0.25","0.5","0.75","1"};
    for(int i =5; i<21;i++){
        for(auto p: percentage){
            cout<<i<<endl;
            string querygraph = "../../test/wildcard/youtube/dense_32_"+to_string(i)+"_"+ p+".graph";
            pair <matching_algo_outputs,matching_algo_outputs> LDF = MatchingWrapper(datagraph,querygraph,"LDF");
            pair <matching_algo_outputs,matching_algo_outputs> NLF = MatchingWrapper(datagraph,querygraph,"NLF");
            pair <matching_algo_outputs,matching_algo_outputs> GQL = MatchingWrapper(datagraph,querygraph,"GQL");
            pair <matching_algo_outputs,matching_algo_outputs> TSOF = MatchingWrapper(datagraph,querygraph,"TSO");
            pair <matching_algo_outputs,matching_algo_outputs> CFL = MatchingWrapper(datagraph,querygraph,"CFL");
            pair <matching_algo_outputs,matching_algo_outputs> DPiso = MatchingWrapper(datagraph,querygraph,"DPiso");
//    matching_algo_outputs KF = Experiments::experiment3(datagraph,querygraph,"KF","0",NULL);

            vector<pair<matching_algo_outputs,matching_algo_outputs>> evaluations;
            evaluations.push_back(LDF);
            evaluations.push_back(NLF);
            evaluations.push_back(GQL);
            evaluations.push_back(TSOF);
            evaluations.push_back(CFL);
            evaluations.push_back(DPiso);

            std::ostringstream oss;
            oss<<"dense_32_"+to_string(i)+"_"+ p;

            for(auto &eval : evaluations){
                oss<<","<<eval.first.candidate_count_sum<<","<<eval.second.candidate_count_sum;
            }

            string file_path = "";
            file_path = "pruning_power_experiment/wildcard/youtube.csv";

            std::string var = oss.str();

            std::ofstream myfile;
            myfile.open (file_path,std::ios_base::app);
            myfile<<var<<"\n";
            myfile.close();
        }
    }


    return 0;
}

int main(int argc, char** argv) {
//    generate_datagraph_eigenvector("../../test/reallife_dataset/hprd/data_graph/hprd.graph","hprd.csv",35);

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
    Experiments::datagraphEigenMatrix = "wordnet.csv";
    string datagraph = "../../test/reallife_dataset/wordnet/data_graph/wordnet.graph";
    string querygraph = "../../test/reallife_dataset/wordnet/query_graph/query_"+query_property+"_"+query_size+"_"+query_number+".graph";

    pair <matching_algo_outputs,matching_algo_outputs> result = MatchingWrapper(datagraph,querygraph,filter);


    std::ostringstream oss;
    oss <<query_property<<"_"<<query_size<<"_"<<query_number<<","<<dictionary[filter];
    oss<<","<<result.first.call_count<<","<<result.second.call_count;
    oss<<","<<result.first.total_time<<","<<result.second.total_time;
    oss<<","<<result.first.candidate_count_sum<<","<<result.second.candidate_count_sum;
    oss<<","<<result.first.enumOutput.embedding_cnt<<","<<result.second.enumOutput.embedding_cnt;
    oss<<","<<result.first.matching_order_string<<","<<result.second.matching_order_string;
    oss<<","<<result.first.preprocessing_time<<","<<result.second.preprocessing_time;
    oss<<","<<result.first.enumeration_time<<","<<result.second.enumeration_time;



    std::string var = oss.str();

    cout<<var<<endl;

    string file_path = "";
//    file_path = "performance_experiment/test.csv";
    file_path = "performance_experiment/"+dataset_name+"_"+query_property+query_size+".csv";

//    file_path = "performance_experiment/mydataset_lesslabels/"+dataset_name+"/25-0/"+query_property+query_size+".csv";

    std::ofstream myfile;
    myfile.open (file_path,std::ios_base::app);
    myfile<<var<<"\n";
    myfile.close();

    return 0;

}









