//
//  hashLinks.h
//  OntoMatch
//
//  Created by D&F on 8/12/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

#ifndef __OntoMatch__hashLinks__
#define __OntoMatch__hashLinks__
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#define M 100

using namespace std;

const int MaxNodeNum = 15;

typedef struct  vertexNode
{
    string label;       //each vertex has a unique label to describe its propertities
    int Index;      //mark the vertex's index
    vector<int> Adj;      //vertex's adjacency vector
    vector<int> Ctext;    //vertex's context vector
} *vPtr, vNode;

/* edge structure */
typedef struct SEdge
{
    int startNode;
    int endNode;
}Edge;


/*SchemaGraph structure */

typedef struct SchemaGraph
{
    //int MaxNum = MaxNodeNum;
    string name;
    int vertexNum;
    int edgeNum;
    vector<vertexNode> vecNode;
    
}*SG, SchemaGraph;


typedef  struct _Hash_Table
{
    double hash_value[M][M]= {0};
    
}hashlinks;


/*function declaration*/
int find_common(vPtr v1, vPtr v2);  //find common context of two nodes v1 and v2

double vSim(vPtr v1, vPtr v2);      //calculate similarity of two nodes/vertices

/*Graph function declaration*/
void InitialGraph( SG& sgraph, int vNum, int label[]);
Edge MakeEdge(int v, int w);
void InsertEdge(SG &sgraph, const Edge& e);
void PrintGraph(SG& sgraph);

hashlinks constructLinks(hashlinks links, SG sgraph, double threshold_value); //construct 


#endif /* defined(__OntoMatch__hashLinks__) */
