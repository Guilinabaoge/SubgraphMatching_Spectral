#include <iostream>
#include <vector>
#include <Eigen/Core>
#include "../GrM.h"
using namespace std;
using  namespace Eigen;
 class CSV{
    public:  

VertexID ID;
vector<pair<VertexID,VertexID>> edges;
map<ui,int> Nedge;

bool NedgeC=false;
bool change=true;
bool Ichange=true;
bool IPchange=true;
bool deleted=false;
public:  
CSV(int eigens,VertexID IDV){
ID=IDV;
}
CSV(int eigens,VertexID IDV,ui maxDeg){

//eigenvalues.resize(eigens);
ID=IDV;
//edges.reserve(maxDeg);
deleted=false;
change=true;
Ichange=true;
}
CSV(int eigens,ui IDV,ui maxDeg,ui MaxQDeg){
ID=IDV;

}
CSV(ui IDV,ui totalD){
ID=IDV;

}
CSV(){
    ID=0;
}
};

