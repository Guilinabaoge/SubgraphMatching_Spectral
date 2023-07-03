
#include "spectra.h"
#include "../../graph/graph.h"
#include <unordered_map>
#include "../eigenHelper.h"
#include "../GrM.h"
#include "../IO.h"
#include "../Experiments.h"



// using namespace cs;
using namespace std;
using namespace Eigen;
// using namespace Spectra;
using namespace std::chrono;

inline bool SecHopEigenPNSSJEP(vector<pair<VertexID, VertexID>> &q_curr, unordered_map<ui, ui> &ID, unordered_set<ui> &SID, map<ui, int> &EvalNeigb2,
                               int *IDDLC, vector<vector<CSV>> &FCS, vector<T> &tripletList, Graph *query_graph, ui Omax, int qID)
{
    pair<VertexID, VertexID> temp1;
    VertexID tempxx = 0;
    vector<VertexID> temp2;
    VertexID vx1 = 0;
    VertexID vx2 = 0;
    VertexID DN = 0;
    unordered_set<ui> EdgeF;
    int kk = 0;
    ui labela = 0;
    VertexID vtemp = 0;
    while (kk < q_curr.size())
    {
        temp1 = q_curr[kk];
        kk++;

        // QN = ;
        if (temp1.first == 1000000)
            continue;
        // SID.insert(temp1.first);
        tempxx = findIndBS(FCS, temp1.second, temp1.first);
        vx1 = ID[FCS[temp1.first][tempxx].ID];
        for (int i = 0; i < FCS[temp1.first][tempxx].edges.size(); i++)
        {
            if (FCS[temp1.first][tempxx].edges[i].first == 1000000 || FCS[temp1.first][tempxx].edges[i].first == qID)
                continue;
            SID.insert(FCS[temp1.first][tempxx].edges[i].first);
            vtemp = FCS[temp1.first][tempxx].edges[i].second;
            auto [it1, success] = ID.try_emplace(vtemp, IDDLC[0]);

            if (success)
            {
                vx2 = IDDLC[0];
                IDDLC[0]++;
                if (IDDLC[1] > 0)
                {
                    labela = query_graph->getVertexLabel(FCS[temp1.first][tempxx].edges[i].first);
                    EvalNeigb2[labela]--;
                    if (EvalNeigb2[labela] == 0)
                        IDDLC[1]--;
                }
                if (IDDLC[0] > Omax)
                {
                    tripletList.clear();
                    return true;
                }
            }
            else
            {
                vx2 = ID[vtemp];
            }
            tripletList.emplace_back(T(vx2, vx1, -1));
            tripletList.emplace_back(T(vx1, vx2, -1));
        }
    }
    q_curr.clear();
    return (true);
}

void ExtractUI2h(vector<ui> &Deg, vector<map<ui, int>> &QueryNlabel2, Graph *query_graph, int qsiz)
{
    const VertexID *u_nbrs;
    ui u_nbrs_count;
    const VertexID *u_nbrsN;
    ui u_nbrs_countN;
    bool ad = true;
    int countkk = 0;
    set<ui> QueryVec;
    map<ui, int> QueryVec1;
    ui labela = 0;
    for (int i = 0; i < qsiz; i++)
    {
        QueryVec.insert(i);
        u_nbrs = query_graph->getVertexNeighbors(i, u_nbrs_count);

        for (int j = 0; j < u_nbrs_count; j++)
        {
            if (QueryVec.insert(u_nbrs[j]).second)
            {
                labela = query_graph->getVertexLabel(u_nbrs[j]);
                if (QueryVec1.count(labela) == 0)
                {
                    // Key does not exist, add it with a value of 1
                    QueryVec1[labela] = 1;
                }
                else
                {
                    // Key exists, increment its value
                    QueryVec1[labela]++;
                }
            }

            u_nbrsN = query_graph->getVertexNeighbors(u_nbrs[j], u_nbrs_countN);
            for (int k = 0; k < u_nbrs_countN; k++)
            {
                labela = query_graph->getVertexLabel(u_nbrsN[k]);
                if (QueryVec.insert(u_nbrsN[k]).second)
                {
                    if (u_nbrsN[k] == i)
                        continue;
                    if (QueryVec1.count(labela) == 0)
                    {
                        // Key does not exist, add it with a value of 1
                        QueryVec1[labela] = 1;
                    }
                    else
                    {
                        // Key exists, increment its value
                        QueryVec1[labela]++;
                    }
                }
            }
        }
        Deg.emplace_back(QueryVec.size());
        QueryVec.clear();
        QueryNlabel2.emplace_back(QueryVec1);
        QueryVec1.clear();
    }
}



inline bool OneHopEigenP(CSV &cvertex, unordered_map<ui, ui> &ID, unordered_set<ui> &SID, int *IDDLC,
                         vector<T> &tripletList, map<ui, int> &EvalNeigb2, Graph *query_graph, vector<pair<VertexID, VertexID>> &q_curr, ui Omax)
{

    ID.insert({cvertex.ID, 0});
    IDDLC[0] = 1;
    // IDDLC[1] = 1;
    IDDLC[2] = 1;
    VertexID vx1 = 0;
    VertexID vx2 = 0;
    // ui count = 1;
    ui count2 = 0;
    ui stop = EvalNeigb2.size();
    ui labela = 0;
    for (int k = 0; k < cvertex.edges.size(); k++)
    {
        if (cvertex.edges[k].first == 1000000)
            continue;
        SID.insert(cvertex.edges[k].first);
        auto result = ID.insert({cvertex.edges[k].second, IDDLC[0]});

        if (result.second)
        {
            tripletList.emplace_back(T(0, IDDLC[0], -1));
            tripletList.emplace_back(T(IDDLC[0], 0, -1));

            IDDLC[0]++;
            if (IDDLC[1] > 0)
            {
                labela = query_graph->getVertexLabel(cvertex.edges[k].first);
                EvalNeigb2[labela]--;
                if (EvalNeigb2[labela] == 0)
                    IDDLC[1]--;
            }
            if (IDDLC[0] > Omax)
            {
                // IDDLC[0]=count;
                return true;
            }
        }
        q_curr.emplace_back(cvertex.edges[k]);
        // SID.insert(cvertex.edges[k].first);
    }
    return true;
    // IDDLC[0] = count;
    // return (count2 == stop);
}
                                    

inline bool OneHopEigen(CSV &cvertex, unordered_map<ui, ui> &ID, unordered_set<ui> &SID, int *IDDLC,
                        vector<T> &tripletList, map<ui, int> &EvalNeigb, Graph *query_graph, vector<pair<VertexID, VertexID>> &q_curr, ui Omax)
{

    ID.insert({cvertex.ID, 0});
    IDDLC[0] = 1;
    IDDLC[2] = 1;
    VertexID vx1 = 0;
    VertexID vx2 = 0;
    // ui count = 1;
    ui count2 = 0;
    ui stop = EvalNeigb.size();
    ui labela = 0;
    for (int k = 0; k < cvertex.edges.size(); k++)
    {
        if (cvertex.edges[k].first == 1000000)
            continue;

        auto result = ID.insert({cvertex.edges[k].second, IDDLC[0]});

        if (result.second)
        {
            tripletList.emplace_back(T(0, IDDLC[0], -1));
            tripletList.emplace_back(T(IDDLC[0], 0, -1));
            IDDLC[0]++;
            if (IDDLC[0] > Omax)
            {
                return true;
            }
        }
        q_curr.emplace_back(cvertex.edges[k]);
    }
    return true;

}

inline bool SecHopEigenPNSSJE(vector<pair<VertexID, VertexID>> &q_curr, unordered_map<ui, ui> &ID, unordered_set<ui> &SID,
                              int *IDDLC, vector<vector<CSV>> &FCS, vector<T> &tripletList, Graph *query_graph, ui Omax, int qID)
{
    pair<VertexID, VertexID> temp1;
    VertexID tempxx = 0;
    vector<VertexID> temp2;
    VertexID vx1 = 0;
    VertexID vx2 = 0;
    VertexID DN = 0;
    unordered_set<ui> EdgeF;
    int kk = 0;
    while (kk < q_curr.size())
    {
        temp1 = q_curr[kk];
        kk++;
        if (temp1.first == 1000000)
            continue;

        tempxx = findIndBS(FCS, temp1.second, temp1.first);
        vx1 = ID[FCS[temp1.first][tempxx].ID];
        for (int i = 0; i < FCS[temp1.first][tempxx].edges.size(); i++)
        {
            if (FCS[temp1.first][tempxx].edges[i].first == 1000000 || FCS[temp1.first][tempxx].edges[i].first == qID)
                continue;
            VertexID vtemp = FCS[temp1.first][tempxx].edges[i].second;
            auto [it1, success] = ID.try_emplace(vtemp, IDDLC[0]);

            if (success)
            {
                vx2 = IDDLC[0];
                IDDLC[0]++;
                if (IDDLC[0] > Omax)
                {
                    tripletList.clear();
                    return true;
                }
            }
            else
            {
                vx2 = ID[vtemp];
            }
            tripletList.emplace_back(T(vx2, vx1, -1));
            tripletList.emplace_back(T(vx1, vx2, -1));
        }
    }
    q_curr.clear();
    return (true);
}


inline bool OneHopEigenMap(CSV &cvertex, map<ui, int> EvalNeigb, Graph *query_graph, ui RS, unordered_set<ui> IDN1)
{
    // unordered_set<ui> IDN1;
    // IDN1.reserve(RS);
    // ui stop = EvalNeigb.size();
    // ui count = 1;
    ui count2 = 0;
    ui labela = 0;
    ui k;
    for (k = 0; k < cvertex.edges.size(); k++)
    {
        if (cvertex.edges[k].first == 1000000)
            continue;
        labela = query_graph->getVertexLabel(cvertex.edges[k].first);
        if (EvalNeigb[labela] < 0)
            continue;
        if (IDN1.insert(cvertex.edges[k].second).second)
        {
            // if (IDN1.count(cvertex.edges[k].second)==0)
            //{IDN1.insert(cvertex.edges[k].second);
            EvalNeigb[labela]--;
            if (EvalNeigb[labela] == 0)
            {
                count2++;
                if (count2 == EvalNeigb.size())
                    return true;
            }
        }
    }
    return false;
}


void ExtractUI(vector<ui> &Deg, Graph *query_graph, int qsiz)
{
    const VertexID *u_nbrs;
    ui u_nbrs_count;
    const VertexID *u_nbrsN;
    ui u_nbrs_countN;
    bool ad = true;
    int countkk = 0;
    set<ui> QueryVec;
    for (int i = 0; i < qsiz; i++)
    {
        QueryVec.insert(i);
        u_nbrs = query_graph->getVertexNeighbors(i, u_nbrs_count);

        for (int j = 0; j < u_nbrs_count; j++)
        {
            QueryVec.insert(u_nbrs[j]);
            u_nbrsN = query_graph->getVertexNeighbors(u_nbrs[j], u_nbrs_countN);
            for (int k = 0; k < u_nbrs_countN; k++)
                QueryVec.insert(u_nbrsN[k]);
        }
        Deg.emplace_back(QueryVec.size());
        QueryVec.clear();
    }
}

void allocateBufferFCS(vector<vector<CSV>> &FCS, const Graph *query_graph, ui **&candidates,
                                    ui *&candidates_count) {
    ui query_vertex_num = query_graph->getVerticesCount();
    //ui candidates_max_num = data_graph->getGraphMaxLabelFrequency();

    candidates_count = new ui[query_vertex_num];
    memset(candidates_count, 0, sizeof(ui) * query_vertex_num);

    candidates = new ui*[query_vertex_num];

    for (ui i = 0; i < query_vertex_num; ++i) {
        candidates[i] = new ui[FCS[i].size()];
    }
}

void ExtractNImap(vector<map<ui, int>> &QueryNlabel, Graph *query_graph, int qsiz)
{
    const VertexID *u_nbrs;
    bool ad = true;
    int countkk = 0;
    ui u_nbrs_count;
    for (int i = 0; i < qsiz; i++)
    {
        u_nbrs = query_graph->getVertexNeighbors(i, u_nbrs_count);
        map<ui, int> QueryVec;

        for (int j = 0; j < u_nbrs_count; j++)
        {
            ui labela = query_graph->getVertexLabel(u_nbrs[j]);
            if (QueryVec.count(labela) == 0)
            {
                // Key does not exist, add it with a value of 1
                QueryVec[labela] = 1;
            }
            else
            {
                // Key exists, increment its value
                QueryVec[labela]++;
            }
        }
        QueryNlabel.emplace_back(QueryVec);
        QueryVec.clear();
    }
}


bool InitPrunTCSR(vector<vector<CSV>> &FCS, int qsiz, Graph *query_graph)
{
    int jj = 0;
    ui VDP;
    VertexID i = 0;
    VertexID rev;
    bool ret = false;
    for (VertexID kk = 0; kk < qsiz; kk++)
    {
        jj = FCS[kk].size();
        VDP = query_graph->getVertexDegree(kk);
        while (jj > 0)
        {
            jj--;
            if (FCS[kk][jj].Ichange == true)
            {
                if (FCS[kk][jj].edges.size() == 0)
                {
                    ret = true;
                    FCS[kk][jj].deleted = true;
                    // FCS[kk].erase(FCS[kk].begin() + jj);
                }

                else if (FCS[kk][jj].edges.size() < VDP)
                {
                    i = 0;

                    while (i < FCS[kk][jj].edges.size())
                    {
                        if (FCS[kk][jj].edges[i].first == 1000000)
                        {
                            i++;
                            continue;
                        }

                        rev = findIndBS(FCS, FCS[kk][jj].edges[i].second, FCS[kk][jj].edges[i].first); // vertex to remove ID?
                        FCS[FCS[kk][jj].edges[i].first][rev].Ichange = true;
                        for (int dd = 0; dd < FCS[FCS[kk][jj].edges[i].first][rev].edges.size(); dd++)
                            if (FCS[FCS[kk][jj].edges[i].first][rev].edges[dd].first == kk && FCS[FCS[kk][jj].edges[i].first][rev].edges[dd].second == FCS[kk][jj].ID)
                            {
                                // FCS[FCS[kk][jj].edges[i].first][rev].edges.erase(FCS[FCS[kk][jj].edges[i].first][rev].edges.begin() + dd);
                                FCS[FCS[kk][jj].edges[i].first][rev].edges[dd].first = 1000000;
                                i++;
                                break;
                            }
                    }
                    // FCS[kk].erase(FCS[kk].begin() + jj);
                    FCS[kk][jj].deleted = true;
                    ret = true;
                }

                FCS[kk][jj].Ichange = false;
            }
        }
    }
    /*for (auto &row : FCS) {
    row.erase(remove_if(row.begin(), row.end(), [](const CSV &csv) {
        return csv.deleted;
    }), row.end());
}

for (auto &row : FCS) {
    for (auto &csv : row) {
        csv.edges.erase(remove_if(csv.edges.begin(), csv.edges.end(), [](const pair<VertexID, VertexID> &edge) {
            return edge.first == 1000000;
        }), csv.edges.end());
    }
}
*/
    for (auto &row : FCS)
    {
        row.erase(remove_if(row.begin(), row.end(), [&](CSV &csv)
                            {
        if (csv.deleted) {
            return true;
        }
        auto newEnd = remove_if(csv.edges.begin(), csv.edges.end(), [](const pair<VertexID, VertexID> &edge) {
            return edge.first == 100000;
        });
        csv.edges.erase(newEnd, csv.edges.end());
        return false; }),
                  row.end());
    }

    return ret;
}

void EdgesCSBasicSet(vector<vector<CSV>> &FCS, int qsiz, int dsiz, Graph *data_graph, Graph *query_graph)
{
    ui u_nbrs_count = 0;
    ui u_nbrs_countD = 0;
    int sizA = 0;
    int sizC;
    VertexID VID = 0;
    VertexID de = 0;
    VertexID cne = 0;
    VertexID labela = 0;
    unordered_map<ui, ui> s[qsiz];
    for (int i = 0; i < qsiz; i++)
    {
        s[i].reserve(FCS[i].size());
        for (int j = 0; j < FCS[i].size(); j++)
            s[i].insert({FCS[i][j].ID, j});
    }

    for (VertexID a = 0; a < qsiz; a++)
    { // for every node of the query
        const VertexID *u_nbrs = query_graph->getVertexNeighbors(a, u_nbrs_count);
        // VertexID* u_nbrs = query_graph->getVertexNeighbors(a, u_nbrs_count);
        sizA = FCS[a].size();
        for (VertexID c = 0; c < u_nbrs_count; c++)
        {
            if (u_nbrs[c] < a)
                continue;
            cne = u_nbrs[c];
            labela = query_graph->getVertexLabel(cne);
            sizC = FCS[cne].size();
            for (VertexID b = 0; b < sizA; b++)
            { // for every candidate vertex of every node of the query
                // candidate vertex for a query a is FCS[a][b].ID
                VID = FCS[a][b].ID;
                // const VertexID* u_nbrsD = data_graph->getVertexNeighbors(FCS[a][b].ID, u_nbrs_countD); //real neigboors of the candidate vertex
                const VertexID *u_nbrsD = data_graph->getNeighborsByLabel(FCS[a][b].ID, labela, u_nbrs_countD); // real neigboors of the candidate vertex
                for (VertexID e = 0; e < u_nbrs_countD; e++)
                { // for every neigboor of the candidate vertex of the real graph
                    // if(u_nbrsD[e]== FCS[cne][d].ID){
                    auto got = s[cne].find(u_nbrsD[e]);
                    if (got != s[cne].end())
                    {
                        FCS[a][b].edges.emplace_back(make_pair(cne, FCS[cne][got->second].ID));
                        FCS[cne][got->second].edges.emplace_back(make_pair(a, VID));
                    }
                }
            }
        }
    }
    for (int i = 0; i < qsiz; i++)
    {
        s[i].clear();
    }
}


void Vertices(vector<vector<CSV>> &FCS, int qsiz, int dsiz, Graph *data_graph, Graph *query_graph, float **&eigenVq1, vector<map<ui, int>> &QueryNlabel)
{


    float **eigenVD1 = NULL;
    eigenVD1 = new float *[dsiz];

    for (ui i = 0; i < dsiz; ++i)
    {
        eigenVD1[i] = new float[35];
    }

//    openData1(Experiments::datagraphEigenMatrix,eigenVD1);


    VectorXd devalues;
    VectorXd qevalues;
    bool con = true;
    ui com = data_graph->getGraphMaxLabelFrequency();
    ui copies = query_graph->getLabelsCount() + 1;
    ui labelsNum[copies];
    ui kk;
    ui i;
    ui j;
    ui reverseLab[310];
    ui u_nbrs_countD = 0;
    const ui *labelData[copies];
    LabelID label = 0;
    for (i = 0; i < 310; i++)
        reverseLab[i] = 310;
    int pos = 0;
    for (i = 0; i < qsiz; i++)
    {
        label = query_graph->getVertexLabel(i);
        ui vdata_vertex_num = 0;
        if (reverseLab[label] == 310)
        {
            reverseLab[label] = pos;
            labelData[pos] = data_graph->getVerticesByLabel(label, vdata_vertex_num);
            labelsNum[pos] = vdata_vertex_num;
            pos++;
        }
    }
    ui reserveS;
    vector<CSV> CS;
    ui k = 0;
    ui degree = 0;
    ui data_vertex_num;
    int prunES = 5;
    if (qsiz > 10)
        prunES = 10;
    else
        prunES = 5;
    // prunES = 10;
    // prunES=qsiz-1;
    for (i = 0; i < qsiz; i++)
    {
        label = query_graph->getVertexLabel(i);
        degree = query_graph->getVertexDegree(i);
        data_vertex_num = 0;
        reserveS = com * degree;
        for (j = 0; j < labelsNum[reverseLab[label]]; ++j)
        {
            VertexID data_vertex = labelData[reverseLab[label]][j];
            //
            if (data_graph->getVertexDegree(data_vertex) >= degree)
            {

                con = true;
                for (kk = 0; kk < prunES; kk++)
                {
                    if (eigenVD1[data_vertex][kk] < eigenVq1[i][kk])
                        if (eigenVq1[i][kk] - eigenVD1[data_vertex][kk] > 0.00001)
                        {
                            con = false;
                            break;
                        }
                } // con=true;
                if (con)
                {

                    for (auto it = QueryNlabel[i].begin(); it != QueryNlabel[i].end(); ++it)
                    {
                        data_graph->getNeighborsByLabelCount(data_vertex, it->first, u_nbrs_countD);
                        if (u_nbrs_countD < it->second)
                        {
                            con = false;
                            break;
                        }
                    }
                    if (con)
                    {
                        CSV cat(10, data_vertex, reserveS);
                        CS.emplace_back(cat);
                    }
                }
            }
        }
        FCS.emplace_back(CS);
        CS.clear();
    }
}

inline void clearWrong(vector<vector<CSV>> &FCS)
{
    for (auto &row : FCS)
    {
        row.erase(remove_if(row.begin(), row.end(), [&](CSV &csv)
                            {
        if (csv.deleted) {
            return true;
        }
        auto newEnd = remove_if(csv.edges.begin(), csv.edges.end(), [](const pair<VertexID, VertexID> &edge) {
            return edge.first == 1000000;
        });
        csv.edges.erase(newEnd, csv.edges.end());
        return false; }),
                  row.end());
    }
}

inline void removeVertexAndEgjesFK(vector<vector<CSV>> &FCS, int i, int deli)
{
    VertexID vx1;
    // CSV cvertexpair;
    for (int j = 0; j < FCS[i][deli].edges.size(); j++)
    {
        // BSCHange
        if (FCS[i][deli].edges[j].first == 1000000)
            continue;
        vx1 = findIndBS(FCS, FCS[i][deli].edges[j].second, FCS[i][deli].edges[j].first);
        // vx1=findInd(FCS,cvertex.edges[j].second,cvertex.edges[j].first);
        // cvertexpair = FCS[cvertex.edges[j].first][vx1];
        FCS[FCS[i][deli].edges[j].first][vx1].change = true;
        FCS[FCS[i][deli].edges[j].first][vx1].IPchange = true;
        // cvertexpair.change = true;
        // cvertexpair.IPchange = true;
        // FCS[FCS[i][deli].edges[j].first][vx1].change = true;
        FCS[FCS[i][deli].edges[j].first][vx1].Nedge[i]--;
        if (FCS[FCS[i][deli].edges[j].first][vx1].Nedge[i] == 0)
            FCS[FCS[i][deli].edges[j].first][vx1].NedgeC = true;
        for (int k = 0; k < FCS[FCS[i][deli].edges[j].first][vx1].edges.size(); k++)
        {
            if (FCS[FCS[i][deli].edges[j].first][vx1].edges[k].first == 1000000)
                continue;
            if (FCS[FCS[i][deli].edges[j].first][vx1].edges[k].first == i && FCS[FCS[i][deli].edges[j].first][vx1].edges[k].second == FCS[i][deli].ID)
            {
                FCS[FCS[i][deli].edges[j].first][vx1].edges[k].first = 1000000;
                break;
            }
        }
    }

    FCS[i][deli].edges.clear();
    FCS[i][deli].deleted = true;

}

inline void removeVertexAndEgjesFKNP(vector<vector<CSV>> &FCS, int i, int deli)
{
    VertexID vx1;
    ui j;
    ui k;
    // CSV cvertexpair;
    for (j = 0; j < FCS[i][deli].edges.size(); j++)
    {
        // BSCHange
        if (FCS[i][deli].edges[j].first == 1000000)
            continue;
        vx1 = findIndBS(FCS, FCS[i][deli].edges[j].second, FCS[i][deli].edges[j].first);
        // vx1=findInd(FCS,cvertex.edges[j].second,cvertex.edges[j].first);
        // cvertexpair = FCS[cvertex.edges[j].first][vx1];
        // cvertexpair.IPchange = true;
        // cvertexpair.change = true;
        FCS[FCS[i][deli].edges[j].first][vx1].IPchange = true;
        // FCS[FCS[i][deli].edges[j].first][vx1].change = true;

        FCS[FCS[i][deli].edges[j].first][vx1].Nedge[i]--;
        if (FCS[FCS[i][deli].edges[j].first][vx1].Nedge[i] == 0)
            FCS[FCS[i][deli].edges[j].first][vx1].NedgeC = true;
        for (k = 0; k < FCS[FCS[i][deli].edges[j].first][vx1].edges.size(); k++)
        {
            if (FCS[FCS[i][deli].edges[j].first][vx1].edges[k].first == 1000000)
                continue;
            if (FCS[FCS[i][deli].edges[j].first][vx1].edges[k].first == i && FCS[FCS[i][deli].edges[j].first][vx1].edges[k].second == FCS[i][deli].ID)
            {
                FCS[FCS[i][deli].edges[j].first][vx1].edges[k].first = 1000000;
                break;
            }
        }
    }
    // while (!FCS[i][deli].edges.empty())
    //   FCS[i][deli].edges.pop_back();

    // FCS[i][deli].edges.clear();
    FCS[i][deli].deleted = true;
    // FCS.erase( remove (FCS[i].begin(), FCS[i].end(), 1000000), FCS[i].end() );

    // FCS[i].erase(FCS[i].begin()+deli);
}


int SpectralMatching(int sizd, Graph *data_graph, string input_query_graph_file, bool twohop, ui **&candidates, ui *&candidates_count )
{
    // auto start2 = high_resolution_clock::now();

    Graph *query_graph = new Graph(true);
    query_graph->loadGraphFromFile(input_query_graph_file);
    int sizq = query_graph->getVerticesCount();
    // int sizq=query_graph->getVerticesCount();
    int Eprun = 5;
    if (sizd >= 10)
        Eprun = 10;
    else
        Eprun = 5;
    // Eprun=sizd-1;
    MatrixXd eigenVq1(sizq, Eprun);

    MTcalc12(query_graph, query_graph->getGraphMaxDegree(), eigenVq1, true, Eprun);
    float **eigenQ = NULL;
    eigenQ = new float *[sizq];

    for (ui i = 0; i < sizq; ++i)
    {
        eigenQ[i] = new float[Eprun];
        for (ui j = 0; j < Eprun; j++)
        {
            eigenQ[i][j] = eigenVq1(i, j);
        }
    }
    return CSInit(data_graph, query_graph, eigenQ, twohop,candidates,candidates_count);
}


inline VertexID findIndBS(vector<vector<CSV>> &FCS, VertexID IDC, VertexID IDQ)
{
    int lo = 0, hi = FCS[IDQ].size() - 1;
    int mid;
    // This below check covers all cases , so need to check
    // for mid=lo-(hi-lo)/2
    while (hi - lo > 1)
    {
        int mid = (hi + lo) / 2;
        if (FCS[IDQ][mid].ID < IDC)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid;
        }
    }
    if (FCS[IDQ][lo].ID == IDC)
    {
        return lo;
    }
    else if (FCS[IDQ][hi].ID == IDC)
    {
        return hi;
    }
    cout << "error Prob" << endl;
    return -10000;
}

void fillEN(vector<vector<CSV>> &FCS, int qsiz, Graph *query_graph)
{
    for (int i = 0; i < qsiz; i++)
    {
        ui de = query_graph->getVertexDegree(i);

        for (int j = 0; j < FCS[i].size(); j++)
        {
            for (int kk = 0; kk < FCS[i][j].edges.size(); kk++)
            {
                FCS[i][j].Nedge[FCS[i][j].edges[kk].first]++;
                // FCS[i][j].NedgeID[FCS[i][j].edges[kk].second]++;
                // if(FCS[i][j].NedgeID[FCS[i][j].edges[kk].second]==1)
                // FCS[i][j].NedgeL[query_graph->getVertexLabel(FCS[i][j].edges[kk].first)]++;
            }

            if (de > FCS[i][j].Nedge.size())
                FCS[i][j].NedgeC = true;
            if (de < FCS[i][j].Nedge.size())
                cout << "Unstobable today" << endl;
        }
    }
}
int CSInit(Graph *data_graph, Graph *query_graph, float **&eigenVq1, bool twohop,ui **&candidates, ui *&candidates_count)
{
    int qsiz = query_graph->getVerticesCount();
    int dsiz = data_graph->getVerticesCount();
    vector<vector<CSV>> FCS;

    FCS.reserve(qsiz);
    vector<ui> DegreeK;
    vector<vector<pair<ui, int>>> QueryNlabel;
    // vector<vector<pair<ui, int>>> QueryNlabel2;
    // vector<int> Qnew(qsiz);
    vector<map<ui, int>> NLabel;
    vector<map<ui, int>> NLabel2;
    // ExtractNI(QueryNlabel, query_graph, qsiz);
    ExtractNImap(NLabel, query_graph, qsiz);
    // NLabel2=NLabel;
    // ExtractUI(DegreeK, query_graph, qsiz);
    ExtractUI2h(DegreeK, NLabel2, query_graph, qsiz);
    // auto VCSN1 = high_resolution_clock::now();
    // auto VCS1 = high_resolution_clock::now();
    Vertices(FCS, qsiz, dsiz, data_graph, query_graph, eigenVq1, NLabel); // parallel-Qsiz?
                                                                                    // VerticesCSN(FCS, qsiz, dsiz, data_graph, query_graph, eigenVD1, eigenVq1, QueryNlabel); // parallel-Qsiz?
    // auto VCS2 = high_resolution_clock::now();
    // auto VCSdur = duration_cast<milliseconds>(VCS2 - VCS1);
    // auto ES1 = high_resolution_clock::now();
    // EdgesCSBasic(FCS,qsiz,dsiz,data_graph,query_graph);
    EdgesCSBasicSet(FCS, qsiz, dsiz, data_graph, query_graph);
    // auto ES2 = high_resolution_clock::now();
    // auto ESdur = duration_cast<milliseconds>(ES2 - ES1);
    // EdgesCheck(FCS, qsiz);
    // cout<<"EdgesCS"<<ESdur.count() <<endl;
    //
    auto IP1 = high_resolution_clock::now();

    while (InitPrunTCSR(FCS, qsiz, query_graph))
        ;
    clearWrong(FCS);
    // EdgesCheck(FCS, qsiz);
    //  printCSSize(FCS,qsiz);
    fillEN(FCS, qsiz, query_graph);
    // auto IP2 = high_resolution_clock::now();
    // auto IPdur = duration_cast<milliseconds>(IP2 - IP1);
    // auto RE1 = high_resolution_clock::now();
    int GDegree = query_graph->getGraphMaxDegree();
    int maxDK = 0;
    int stop = 0;
    while (ReverseRefinementNOTES(NLabel,
                                  FCS, qsiz, query_graph, GDegree))
        clearWrong(FCS);
    ;
    stop++;
    // CSSizeReal(FCS, qsiz);

    if (RefinementEigen(NLabel, NLabel2, FCS, qsiz, query_graph, eigenVq1, DegreeK, twohop))
    {
        //      if(RefinementEigenTest( NLabel, FCS, qsiz, query_graph, eigenVq1,DegreeK)){
        clearWrong(FCS);
        while (ReverseRefinementNOTES(NLabel, FCS, qsiz, query_graph, GDegree))
            clearWrong(FCS);
    }


    allocateBufferFCS(FCS, query_graph, candidates, candidates_count);
    for (int i = 0; i < qsiz; i++)
    {
        for (int j = 0; j < FCS[i].size(); j++)
            candidates[i][j] = FCS[i][j].ID;

        candidates_count[i] = FCS[i].size();
    }

    int totalCand = 0;
    for (int i = 0; i < query_graph->getVerticesCount(); i++)
    {

        // cout<<"C(i) "<<candidates_count[i]<<",";
        totalCand = candidates_count[i] + totalCand;
    }
    return totalCand;
}

int CSSizeReal(vector<vector<CSV>> &FCS, int qsiz)
{
    int count = 0;
    int Tcount = 0;
    for (int kk = 0; kk < qsiz; kk++)
    {
        for (int zz = 0; zz < FCS[kk].size(); zz++)
            // if(FCS[kk][zz].ID!=1000000)
            if (FCS[kk][zz].deleted == false)
            {
                count++;
                 //cout<<", "<<FCS[kk][zz].ID;
            } // cout<<" "<<endl;

        Tcount = count + Tcount;
        // cout<<"i= "<<kk<<" size "<<count<<endl;
        count = 0;
    }
    return Tcount;
}


bool ReverseRefinementNOTES(vector<map<ui, int>> NLabel, vector<vector<CSV>> &FCS, int qsiz, Graph *query_graph, ui GDegree)
{
    unordered_set<ui> SID;
    // unordered_set<ui> SIDD;

    SID.reserve(GDegree / 4);
    // SIDD.reserve(qsiz);

    bool returnhere = false;
    ui i;
    ui j;
    // CSV cvertex;

    for (i = 0; i < qsiz; i++)
    {

        for (j = 0; j < FCS[i].size(); j++)
        {
            // cvertex = FCS[i][j];
            if (FCS[i][j].IPchange == false || FCS[i][j].deleted == true)
                continue;

            else if (FCS[i][j].NedgeC)
            {
                removeVertexAndEgjesFKNP(FCS, i, j);
                returnhere = true;
            }
            else if (!OneHopEigenMap(FCS[i][j], NLabel[i], query_graph, GDegree, SID))
            {
                removeVertexAndEgjesFKNP(FCS, i, j);
                returnhere = true;
            }
            else
            {
                FCS[i][j].IPchange = false;
            }
        }
    }

    return returnhere;
}

bool RefinementEigen(vector<map<ui, int>> NLabel, vector<map<ui, int>> NLabel2, vector<vector<CSV>> &FCS,
                     int qsiz, Graph *query_graph, float **&eigenVq1, vector<ui> DM, bool twohop)
{
    vector<T> tripletList;
    //
    std::map<int, int> count_uniques;
    std::set<std::pair<int, int>> seen;
    std::vector<Triplet<double>> unique_triplets;
    unordered_map<ui, ui> SID;
    unordered_set<ui> SIDD;
    SIDD.reserve(qsiz);

    int IDDLC[3] = {0, 0, 0};
    bool returnhere = false;
    VertexID vertex = 0;
    vector<VertexID> temp2;
    vector<pair<VertexID, VertexID>> q_curr;
    int Eprun = 5;
    if (qsiz >= 10)
        Eprun = 10;
    else
        Eprun = 5;
    // VectorXd qevalues;
    // VertexID tempxx = 0;
    // int metrwiters = 0;
    VertexID vertexDegree = 0;
    VertexID vertexlabel = 0;
    ui SIDDSize = 0;
    // int prunC = 0;
    // bool testcor;
    bool continueE = false;
    bool con = true;
    ui oMax = qsiz * 1.5;
    SID.reserve(oMax + 10);
    // SID.reserve(oMax+5);
    // SIDD.reserve(qsiz);
    tripletList.reserve(oMax * 2);
    VectorXd evalues(Eprun);
    ui i;
    ui j;
    map<ui, int> NLabelT;
    for (i = 0; i < qsiz; i++)
    {

        // vertexDegree = query_graph->getVertexDegree(i);
        // vertexlabel = query_graph->getVertexLabel(i);

        for (j = 0; j < FCS[i].size(); j++)
        {
            // cvertex = FCS[i][j];
            // if (FCS[i][j].deleted == true || FCS[i][j].change == false)
            if (FCS[i][j].deleted == true)
                continue;

            // continueE = false;
            if (!FCS[i][j].NedgeC)
            {
                tripletList.clear();
                q_curr.clear();
                SID.clear();

                IDDLC[0] = 0;
                if (twohop)
                {
                    SIDD.clear();
                    SIDD.insert(i);
                    NLabelT = NLabel2[i];
                    IDDLC[1] = NLabel2[i].size();
                    // continueE = OneHopEigenP(FCS[i][j], SID, SIDD, IDDLC, tripletList, NLabelT,
                    //                          query_graph, q_curr, oMax);
                    OneHopEigenP(FCS[i][j], SID, SIDD, IDDLC, tripletList, NLabelT,
                                 query_graph, q_curr, oMax);
                }

                else
                    //  continueE = OneHopEigen(FCS[i][j], SID, SIDD, IDDLC, tripletList, NLabel[i],
                    //                          query_graph, q_curr, oMax);
                    OneHopEigen(FCS[i][j], SID, SIDD, IDDLC, tripletList, NLabel[i],
                                query_graph, q_curr, oMax);

                // continueE = true;
                if (IDDLC[0] <= oMax)
                {
                    if (twohop)
                    {
                        SecHopEigenPNSSJEP(q_curr, SID, SIDD, NLabelT, IDDLC, FCS, tripletList, query_graph, oMax, i);
                        SIDDSize = SIDD.size();
                    }

                    else
                    {
                        SecHopEigenPNSSJE(q_curr, SID, SIDD, IDDLC, FCS, tripletList, query_graph, oMax, i);
                        IDDLC[1] = 0;
                        SIDDSize = IDDLC[0];
                    }
                    //
                }
                if (IDDLC[0] <= oMax)
                    if (IDDLC[0] < DM[i] || IDDLC[1] > 0 || SIDDSize < DM[i])
                    {
                        removeVertexAndEgjesFK(FCS, i, j);
                        returnhere = true;
                        IDDLC[0] = oMax + 1;
                    }

                if (IDDLC[0] <= oMax)
                {
                    count_uniques.clear();
                    seen.clear();
                    unique_triplets.clear();
                    for (auto t : tripletList)
                    {
                        if (seen.count({t.row(), t.col()}) == 0)
                        {
                            unique_triplets.push_back(Triplet<double>(t.row(), t.col(), -1));
                            seen.insert({t.row(), t.col()});
                            count_uniques[t.row()]++;
                        }
                    }
                    for (auto it : count_uniques)
                    {
                        unique_triplets.push_back(Triplet<double>(it.first, it.first, it.second));
                    }
                    tripletList = unique_triplets;
                    if (tripletList.size() == IDDLC[0] * IDDLC[0])
                    {
                        evalues.resize(Eprun);

                        for (ui ss = 0; ss < Eprun; ss++)
                        {
                            if (ss < IDDLC[0])
                                evalues(ss) = IDDLC[0] - 1;
                            else if (ss == IDDLC[0])
                                evalues(ss) = 0;
                            else
                                evalues(ss) = -1;
                        }
                    }
                    else
                    {
                        SparseMatrix<double> M(IDDLC[0], IDDLC[0]);
                        M.setFromTriplets(tripletList.begin(), tripletList.end(), [](double a, double b)
                                          { return b; });
                        // checkM(M);

                        calcEigens1(M, Eprun, evalues, IDDLC[0]);
                    }
                    con = true;
                    for (int dd = 0; dd < Eprun; dd++)
                    {
                        if (eigenVq1[i][dd] <= 0)
                            break;
                        if (evalues[dd] < eigenVq1[i][dd])
                            if (eigenVq1[i][dd] - evalues[dd] > 0.01)
                            {
                                con = false;
                                // break;
                            }
                    }
                    if (!con)
                    {
                        // prunC++;
                        removeVertexAndEgjesFK(FCS, i, j);
                        returnhere = true;
                    }

                    //}
                    // else
                    // {
                    //     FCS[i][j].change = false;
                    //  }
                }
                // else
                // {
                //      FCS[i][j].change = false;
                //  }
            }
            else
            {
                removeVertexAndEgjesFK(FCS, i, j);
                returnhere = true;
            }

            // if (continueE)
            // {

            // }

            /**/
        }
    }
    return returnhere;
}




