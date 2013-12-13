//
//  main.cpp
//  OntoMatch
//
//  Created by D&F on 27/11/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "hashLinks.h"
#include "cluster.h"
#define MaxNodeNum 20

using namespace std;



void InitialGraph( SG& sgraph, int vNum, string label[])
{
    vNode node;
    sgraph->vertexNum = vNum;
    sgraph->edgeNum   = 0;
    for (int i =0; i< vNum; i++)
    {
        node.Index = i;
        node.label = label[i];
        sgraph->vecNode.push_back(node);
        
        sgraph->vecNode.at(i).Ctext.push_back(i);      //push self into context
    }
    
}

Edge MakeEdge(int v, int w)
{
    Edge e;
    e.startNode = v;
    e.endNode   = w;
    return e;
}

void InsertEdge(SG& sgraph, const Edge& e)
{
    sgraph->vecNode.at(e.startNode).Adj.push_back(e.endNode);
    sgraph->vecNode.at(e.startNode).Ctext.push_back(e.endNode);

    sgraph->vecNode.at(e.endNode).Ctext.push_back(e.startNode);
    
}



void PrintGraph(SG& sgraph)
{
    SG g=sgraph;
    int vNum = g->vertexNum;
    for (int i=0; i< vNum; i++ )
    {
        cout<< "Node: "<< i << "\n" ;
        cout<< "Label: "<< g->vecNode[i].label <<endl;
        cout<< "AdjList:" << "\t";
        for(vector<int>:: iterator j= g->vecNode[i].Adj.begin(); j!= g->vecNode[i].Adj.end(); j++)
            cout<< *j << "\t";
        
        cout<< endl;
 //       cout<< "Context:" << "\t";

//        for(vector<int>:: iterator k= g->vecNode[i].Ctext.begin(); k!= g->vecNode[i].Ctext.end(); k++)
//            cout<< *k << "\t";
        cout<< endl;
        
    }
}


int main()
{
//int i=0;
    SG sgraph =new SchemaGraph;
    hashlinks node_L, clu_L;
    CSet cluSet, newSet;
    Dendro Dgram;
    CC *clu = new CC [MaxNodeNum];
    double distance = 1.0;
    double delta= 0.25;
    double IntraSim_Value =0;
    
    for (int i=0; i< MaxNodeNum; i++)
    {
        clu[i]= newCluster(clu[i], i);
        clusterAdd(clu[i], cluSet);
    }
/*Test part*/
    
    
    
    string label[MaxNodeNum]={"Dept","grant","project", "faculty", "dnp", "amount", "staff", "name"};
    InitialGraph(sgraph, MaxNodeNum, label );
    InsertEdge(sgraph, MakeEdge(0,1 ));
    InsertEdge(sgraph, MakeEdge(0,2 ));
    InsertEdge(sgraph, MakeEdge(0,3 ));
    InsertEdge(sgraph, MakeEdge(1,4 ));
    InsertEdge(sgraph, MakeEdge(1,5 ));
    InsertEdge(sgraph, MakeEdge(1, 6));
    InsertEdge(sgraph, MakeEdge(2, 7));
    InsertEdge(sgraph, MakeEdge(2, 8));
    InsertEdge(sgraph, MakeEdge(3, 9));
    InsertEdge(sgraph, MakeEdge(3, 10));
    InsertEdge(sgraph, MakeEdge(4, 11));
    InsertEdge(sgraph, MakeEdge(4, 12));
    InsertEdge(sgraph, MakeEdge(4, 13));
    InsertEdge(sgraph, MakeEdge(5, 14));
    InsertEdge(sgraph, MakeEdge(5, 15));
    InsertEdge(sgraph, MakeEdge(6, 16));
    InsertEdge(sgraph, MakeEdge(7, 17));
    InsertEdge(sgraph, MakeEdge(8, 18));
    InsertEdge(sgraph, MakeEdge(8, 19));
    
     //
    //Please change MaxNodeNum to 30 if using 30-node graph
  /*   InsertEdge(sgraph, MakeEdge(9, 20));
     InsertEdge(sgraph, MakeEdge(9, 21));
     InsertEdge(sgraph, MakeEdge(9, 22));
     InsertEdge(sgraph, MakeEdge(9, 23));
     InsertEdge(sgraph, MakeEdge(10, 24));
     InsertEdge(sgraph, MakeEdge(10, 25));
     InsertEdge(sgraph, MakeEdge(11, 26));
     InsertEdge(sgraph, MakeEdge(12, 27));
     InsertEdge(sgraph, MakeEdge(12, 28));
     InsertEdge(sgraph, MakeEdge(14, 29));
*/

    
    
    PrintGraph(sgraph);

    
    InitDendrogram(cluSet, Dgram);

    node_L= constructLinks(node_L, sgraph, 0.0);
    
   // Sim_BtwCluster_Hash(cluSet, node_L, clu_L);
  //  cluSet= Merge_EveryLevel(cluSet, newSet, clu_L, distance);
    int gen=0;
    int BestLevel;
    while(cluSet.size> 1)
    {
        IntraSim_Value= computeIntraSim(cluSet, node_L);
        cout<< "Current level:  \t" << Dgram.level <<endl;
        
        cout<<"Set Size: \t"<<cluSet.size<< endl;
        
        for(vector<ClusterC>::iterator i=cluSet.ElemCluster.begin(); i!=cluSet.ElemCluster.end(); i++)
        {
            for(vector<int>::iterator j=i->Node.begin(); j!=i->Node.end(); j++)
                cout<< *j<< '\t';
            cout<< '\n';
        }
        construct_Hierarchy(cluSet, Dgram, node_L,distance, IntraSim_Value, BestLevel);
        distance+= delta;

        gen++;
        
    }
    cout<< "cluster iteration times： \t";
    cout<< gen <<endl;
    cout<< "The best level: \t" << BestLevel<< endl;
    double SimValue= computeIntraSim(Dgram.addLevel[BestLevel], node_L);
    cout<< "Biggest Similarity among the clusters: " << '\t' << SimValue <<endl;
    cout<< "Cluster Results: \n";
    CSet p= Dgram.addLevel[BestLevel];
    
    for(vector<ClusterC>::iterator t=p.ElemCluster.begin(); t!=p.ElemCluster.end();t++)
    {
        for(vector<int>::iterator j=t->Node.begin(); j!=t->Node.end(); j++)
                cout<< *j<< '\t';
        cout<< '\n';
    }
 return 0;
    
}
