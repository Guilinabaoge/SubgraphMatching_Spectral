#include "cs.h"
#include <chrono>
#include <thread>
#include <Eigen/StdVector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <utility>
#include <cmath>
#include <Eigen/SparseCore>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <list>
typedef Eigen::Triplet<double> T;

inline bool OneHopEigen(CSV &cvertex, unordered_map<ui, ui> &ID, unordered_set<ui> &SID, int *IDDLC,
                        vector<T> &tripletList, map<ui, int> &EvalNeigb, Graph *query_graph, vector<pair<VertexID, VertexID>> &q_curr, ui Omax);
inline bool SecHopEigenPNSSJE(vector<pair<VertexID, VertexID>> &q_curr, unordered_map<ui, ui> &ID, unordered_set<ui> &SID,
                              ui *IDDLC, vector<vector<CSV>> &FCS, vector<T> &tripletList, Graph *query_graph, ui Omax, int qID);
inline bool OneHopEigen(CSV &cvertex, unordered_map<ui, ui> &ID, unordered_set<ui> &SID, ui *IDDLC,
                        vector<T> &tripletList, map<ui, int> EvalNeigb, Graph *query_graph, vector<pair<VertexID, VertexID>> &q_curr, ui Omax);
bool RefinementEigen(vector<map<ui, int>> NLabel, vector<map<ui, int>> NLabel2, vector<vector<CSV>> &FCS,
                     int qsiz, Graph *query_graph, float **&eigenVq1, vector<ui> DM, bool twohop);
int CSSizeReal(vector<vector<CSV>> &FCS, int qsiz);
bool ReverseRefinementNOTES(vector<map<ui, int>> NLabel, vector<vector<CSV>> &FCS, int qsiz, Graph *query_graph, ui GDegree);
int CSInit(Graph *data_graph, Graph *query_graph, float **&eigenVq1,bool twohop, ui **&candidates, ui *&candidates_count);
inline VertexID findIndBS(vector<vector<CSV>> &FCS, VertexID IDC, VertexID IDQ);
inline void removeVertexAndEgjesFK(vector<vector<CSV>> &FCS, int i, int deli);
void clearWrong(vector<vector<CSV>> &FCS);
void allocateBufferFCS(vector<vector<CSV>> &FCS, const Graph *query_graph, ui **&candidates,
                                    ui *&candidates_count);
void ExtractUI(vector<ui> &Deg, Graph *query_graph, int qsiz);
void ExtractNImap(vector<map<ui, int>> &QueryNlabel, Graph *query_graph, int qsiz);
bool InitPrunTCSR(vector<vector<CSV>> &FCS, int qsiz, Graph *query_graph);
void EdgesCSBasicSet(vector<vector<CSV>> &FCS, int qsiz, int dsiz, Graph *data_graph, Graph *query_graph);
void Vertices(vector<vector<CSV>> &FCS, int qsiz, int dsiz, Graph *data_graph, Graph *query_graph, float **&eigenVD1, float **&eigenVq1, vector<map<ui, int>> &QueryNlabel);
inline void removeVertexAndEgjesFKNP(vector<vector<CSV>> &FCS, int i, int deli);
int SpectralMatching(int sizd, Graph *data_graph, string input_query_graph_file,bool twohop, ui **&candidates, ui *&candidates_count);
void fillEN(vector<vector<CSV>> &FCS, int qsiz, Graph *query_graph);
inline bool OneHopEigen(CSV &cvertex, unordered_map<ui, ui> &ID, unordered_set<ui> &SID, int *IDDLC,
                 vector<T> &tripletList, map<ui, int> &EvalNeigb, Graph *query_graph, vector<pair<VertexID, VertexID>> &q_curr, ui Omax);
static inline bool OneHopEigenP(CSV &cvertex, unordered_map<ui, ui> &ID, unordered_set<ui> &SID, int *IDDLC,
                         vector<T> &tripletList, map<ui, int> &EvalNeigb2, Graph *query_graph, vector<pair<VertexID, VertexID>> &q_curr, ui Omax);
