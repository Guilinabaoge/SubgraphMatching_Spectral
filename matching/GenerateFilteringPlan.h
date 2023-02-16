//
// Created by ssunah on 11/21/18.
//

#ifndef SUBGRAPHMATCHING_GENERATEFILTERINGPLAN_H
#define SUBGRAPHMATCHING_GENERATEFILTERINGPLAN_H


#include "graph/graph.h"
#include "configuration/types.h"

class GenerateFilteringPlan {
public:
    static void generateTSOFilterPlan(Graph *data_graph, Graph *query_graph, TreeNode *&tree,
                                           VertexID *&order,bool isEigenCheck);
    static void generateCFLFilterPlan(Graph *data_graph, Graph *query_graph, TreeNode *&tree,
                                      VertexID *&order, int &level_count, ui *&level_offset);
    static void generateDPisoFilterPlan(Graph *data_graph, Graph *query_graph, TreeNode *&tree,
                                        VertexID *&order);
    static void generateCECIFilterPlan(Graph *data_graph, Graph *query_graph, TreeNode *&tree,
                                       VertexID *&order);
private:
    static VertexID selectTSOFilterStartVertex(Graph *data_graph, Graph *query_graph,bool isEigenCheck);
    static VertexID selectCFLFilterStartVertex(Graph *data_graph, Graph *query_graph);
    static VertexID selectDPisoStartVertex(Graph *data_graph, Graph *query_graph);
    static VertexID selectCECIStartVertex(Graph *data_graph, Graph *query_graph);
};


#endif //SUBGRAPHMATCHING_GENERATEFILTERINGPLAN_H
