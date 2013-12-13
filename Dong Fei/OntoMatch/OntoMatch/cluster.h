//
//  cluster.h
//  OntoMatch
//
//  Created by D&F on 27/11/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

#ifndef __OntoMatch__cluster__
#define __OntoMatch__cluster__

#include "hashLinks.h"

typedef struct ClusterC{

    int elementNum=0;
    vector<int> Node;
}CC;


typedef struct ClusterSet{

    int size= 0;
    vector<ClusterC> ElemCluster;
    
}CSet;

typedef struct Dendrogram
{
    int level =0;
    vector<ClusterSet> addLevel;
    
}Dendro;


/*cluster function declaration*/
ClusterC newCluster(CC clu, int nodeIndx);

void clusterAdd(CC C, CSet& cluSet);


double Max_Sim_Value(CSet cluSet, int& k, int& m, hashlinks& clu_L);


CSet MergeCluster_FromSet(CSet cluSet, int k, int m, double distance);

CC Merge_Set(CSet cluSet, int k, int m);

CSet Merge_EveryLevel(CSet cluSet, CSet& newSet, hashlinks& clu_L, double distance);

double computeIntraSim(CSet cluSet, hashlinks L);

double intra_sim(CC clu, hashlinks L);           //calculate intra-cluster similarity

double simBtwCluster(CC clu1, CC clu2, hashlinks L);

void Sim_BtwCluster_Hash(CSet cluSet, hashlinks node_L, hashlinks& clu_L);

ClusterC mergeCluster(CC clu1, CC clu2, double distance);

//initialize the bottom level of dendrogram
void InitDendrogram(CSet cluSet, Dendro& Dgram);

void construct_Hierarchy(CSet& cluSet, Dendro& Dgram, hashlinks L, double distance, double IntraValue, int& BestLevel);

int cut_off_cluster(int level);        //find the largest intra similarity

//Dendro getBestCluster(int Bestlevel);

//void CDSim();                       //calculate two cluster document similarity






#endif /* defined(__OntoMatch__cluster__) */
