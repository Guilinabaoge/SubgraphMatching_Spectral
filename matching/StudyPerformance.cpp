//
// Created by ssunah on 12/3/18.
//

#include <functional>
#include <map>
#include <chrono>
#include <future>
#include <thread>
#include <fstream>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>

#include "matchingcommand.h"
#include "graph/graph.h"
#include "GenerateFilteringPlan.h"
#include "FilterVertices.h"
#include "BuildTable.h"
#include "GenerateQueryPlan.h"
#include "EvaluateQuery.h"
#include "IO.h"
#include "eigenHelper.h"
#include "Experiments.h"
#include "StudyPerformance.h"
#include "KF/spectra.h"



#define NANOSECTOSEC(elapsed_time) ((elapsed_time)/(double)1000000000)
#define BYTESTOMB(memory_cost) ((memory_cost)/(double)(1024 * 1024))
//#define PRINT;
//#define ONLYCOUNTS;


matching_algo_outputs StudyPerformance::solveGraphQuery(matching_algo_inputs inputs){
    int argc = 0;
    char** argv;
    MatchingCommand command(argc, argv);
    std::string input_query_graph_file = inputs.qgraph_path;
    std::string input_data_graph_file = inputs.dgraph_path;
    std::string input_filter_type = inputs.filter;
    std::string input_order_type = inputs.order;
    std::string input_engine_type = inputs.engine;
    std::string input_max_embedding_num = "1000";
    std::string input_time_limit = "300";
    std::string input_order_num = command.getOrderNum();
    std::string input_distribution_file_path = command.getDistributionFilePath();
    std::string input_csr_file_path = command.getCSRFilePath();
    std::string input_iseigen = inputs.eigen;
    std::string input_tops;
    matching_algo_outputs outputs;
    /**
     * Output the command line information.
     */
#ifdef PRINT1
    std::cout << "Command Line:" << std::endl;
    std::cout << "\tData Graph CSR: " << input_csr_file_path << std::endl;
    std::cout << "\tData Graph: " << input_data_graph_file << std::endl;
    std::cout << "\tQuery Graph: " << input_query_graph_file << std::endl;
    std::cout << "\tFilter Type: " << input_filter_type << std::endl;
    std::cout << "\tOrder Type: " << input_order_type << std::endl;
    std::cout << "\tEngine Type: " << input_engine_type << std::endl;
    std::cout << "\tOutput Limit: " << input_max_embedding_num << std::endl;
    std::cout << "\tTime Limit (seconds): " << input_time_limit << std::endl;
    std::cout << "\tOrder Num: " << input_order_num << std::endl;
    std::cout << "\tDistribution File Path: " << input_distribution_file_path << std::endl;
    std::cout << "\tWith eigen filter?: " << input_iseigen << std::endl;
    std::cout << "\tTop-s eigen value?: " << input_tops << std::endl;

    std::cout << "--------------------------------------------------------------------" << std::endl;
#endif
    /**
     * Load input graphs.
     */
#ifdef PRINT1
    std::cout << "Load graphs..." << std::endl;
#endif


    auto start = std::chrono::high_resolution_clock::now();

    Graph* query_graph = new Graph(true);
    query_graph->loadGraphFromFile(input_query_graph_file);
    query_graph->buildCoreTable();
    outputs.query_size = query_graph->getVerticesCount();

    Graph* data_graph = new Graph(true);

    if (input_csr_file_path.empty()) {
        if(inputs.filter!="KF"){
            data_graph->loadGraphFromFile(input_data_graph_file);
        }
        else{
            data_graph->loadGraphFromFile(input_data_graph_file);
            data_graph->BuildLabelOffset();
        }

    }
    else {
        std::string degree_file_path = input_csr_file_path + "_deg.bin";
        std::string edge_file_path = input_csr_file_path + "_adj.bin";
        std::string label_file_path = input_csr_file_path + "_label.bin";
        data_graph->loadGraphFromFileCompressed(degree_file_path, edge_file_path, label_file_path);
    }

    auto end = std::chrono::high_resolution_clock::now();

    double load_graphs_time_in_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();


    input_tops = "10";
    if(query_graph->getVerticesCount()==4) input_tops = "4";
    if(query_graph->getVerticesCount()==8) input_tops = "8";


#ifdef PRINT1
    std::cout << "-----" << std::endl;
    std::cout << "Query Graph Meta Information" << std::endl;
    query_graph->printGraphMetaData();
    std::cout << "-----" << std::endl;
    data_graph->printGraphMetaData();

    std::cout << "--------------------------------------------------------------------" << std::endl;
#endif
    /**
     * Start queries.
     */


#ifdef PRINT1
    std::cout << "Start queries..." << std::endl;
    std::cout << "-----" << std::endl;
    std::cout << "Filter candidates..." << std::endl;
#endif

    start = std::chrono::high_resolution_clock::now();

    bool isEigenCheck;
    int top_s = std::stoi( input_tops);
    istringstream(input_iseigen) >> isEigenCheck;



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
    if (input_filter_type == "LDF") {
        FilterVertices::LDFFilter(data_graph, query_graph, candidates, candidates_count,isEigenCheck,top_s);
    } else if(input_filter_type=="KF"){
        SpectralMatching(query_graph->getVerticesCount(), data_graph, input_query_graph_file, true,candidates,candidates_count);
    }
    else if (input_filter_type == "NLF") {
        FilterVertices::NLFFilter(data_graph, query_graph, candidates, candidates_count,isEigenCheck,top_s);
    } else if (input_filter_type == "GQL") {
        FilterVertices::GQLFilter(data_graph, query_graph, candidates, candidates_count,isEigenCheck,top_s);
    } else if (input_filter_type == "TSO") {
        FilterVertices::TSOFilter(data_graph, query_graph, candidates, candidates_count, tso_order, tso_tree,isEigenCheck,top_s);
    } else if (input_filter_type == "CFL") {
        FilterVertices::CFLFilter(data_graph, query_graph, candidates, candidates_count, cfl_order, cfl_tree,isEigenCheck,top_s);
    } else if (input_filter_type == "DPiso") {
        FilterVertices::DPisoFilter(data_graph, query_graph, candidates, candidates_count, dpiso_order, dpiso_tree,isEigenCheck,top_s);
    } else if (input_filter_type == "CECI") {
        FilterVertices::CECIFilter(data_graph, query_graph, candidates, candidates_count, ceci_order, ceci_tree, TE_Candidates, NTE_Candidates,isEigenCheck,top_s);
    }  else {
        std::cout << "The specified filter type '" << input_filter_type << "' is not supported." << std::endl;
        exit(-1);
    }

    // Sort the candidates to support the set intersections
    // TODO figure out why CECI doesn't work, read the paper.
//    if (input_filter_type != "CECI")
//        FilterVertices::sortCandidates(candidates, candidates_count, query_graph->getVerticesCount());
//
//    end = std::chrono::high_resolution_clock::now();
//    double filter_vertices_time_in_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    int sum = 0;
    outputs.candidate_count_sum = accumulate(candidates_count, candidates_count + query_graph->getVerticesCount(), sum);


    /**
     * Release the allocated memories.
     */


    delete[] candidates_count;
    delete[] tso_order;
    delete[] tso_tree;
    delete[] cfl_order;
    delete[] cfl_tree;
    delete[] dpiso_order;
    delete[] dpiso_tree;
    delete[] ceci_order;
    delete[] ceci_tree;



    for (ui i = 0; i < query_graph->getVerticesCount(); ++i) {
        delete[] candidates[i];
    }
    delete[] candidates;

    delete query_graph;
    delete data_graph;

    /**
     * End.
     */

#ifdef PRINT
    std::cout << "--------------------------------------------------------------------" << std::endl;
    printf("Load graphs time (seconds): %.4lf\n", NANOSECTOSEC(load_graphs_time_in_ns));
    printf("Filter vertices time (seconds): %.4lf\n", NANOSECTOSEC(filter_vertices_time_in_ns));
    printf("Build table time (seconds): %.4lf\n", NANOSECTOSEC(build_table_time_in_ns));
    printf("Generate query plan time (seconds): %.4lf\n", NANOSECTOSEC(generate_query_plan_time_in_ns));
    printf("Enumerate time (seconds): %.4lf\n", NANOSECTOSEC(enumeration_time_in_ns));
    printf("Preprocessing time (seconds): %.4lf\n", NANOSECTOSEC(preprocessing_time_in_ns));
    printf("Total time (seconds): %.4lf\n", NANOSECTOSEC(total_time_in_ns));
    printf("Memory cost (MB): %.4lf\n", BYTESTOMB(memory_cost_in_bytes));
    printf("#Embeddings: %zu\n", embedding_count);
    printf("Call Count: %zu\n", call_count);
    printf("Candidate count sum %zu\n",outputs.candidate_count_sum);
    printf("Candidate true sum so far %zu\n",outputs.enumOutput.candidate_true_count_sum);
    printf("Per Call Count Time (nanoseconds): %.4lf\n", enumeration_time_in_ns / (call_count == 0 ? 1 : call_count));
    std::cout << "End." << std::endl;
#endif

    return outputs;
}



