
//  cluster.cpp
//  OntoMatch
//
//  Created by D&F on 27/11/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

#include "cluster.h"
#include <algorithm>

ClusterC newCluster(CC clu, int nodeIndx)
{
    clu.Node.push_back(nodeIndx);
    clu.elementNum = 1;
    return clu;
}

void clusterAdd(ClusterC C, CSet& cluSet)
{
    cluSet.ElemCluster.push_back(C);
    cluSet.size++;
    
    //return cluSet;
}

double intra_sim(CC clu, hashlinks L)   //intra_similarity with a cluster
{
    double simValue =0;
    //double Array[M]={0};
    int totalNum =1, k=0;
    int eNum =clu.elementNum;
    if (eNum ==1)                   //cluster has only one node
        simValue= 0;
    else
    {
        totalNum= eNum* (eNum-1)/2;
    
    for(vector<int>::iterator i= clu.Node.begin(); i!= clu.Node.end(); i++)
        for(vector<int>::iterator j= clu.Node.begin()+1; j!=clu.Node.end(); j++)
            if(j!=i)
            {
                simValue += L.hash_value[*i][*j];
                k++;
            }
    }
   // cout<< simValue/totalNum <<endl;
    
    return simValue/totalNum;
}

//similarity between clusters in a cluster set at different levl
double computeIntraSim(CSet cluSet, hashlinks L)
{
    double SimValue=0;
    for(vector<ClusterC>::iterator i= cluSet.ElemCluster.begin(); i!=cluSet.ElemCluster.end(); i++)
        SimValue+=intra_sim(*i, L);
    
    return SimValue/cluSet.ElemCluster.size();   //divide by set size
}

double simBtwCluster(CC clu1, CC clu2, hashlinks L) //similarity between clusters
{
    double simBtwValue= 0;
    for(vector<int>::iterator i= clu1.Node.begin(); i!=clu1.Node.end(); i++)
        for(vector<int>::iterator j= clu2.Node.begin(); j!=clu2.Node.end(); j++)
            simBtwValue += L.hash_value[*i][*j];
    
    //cout<< simBtwValue<< endl;
    return simBtwValue/(clu1.Node.size()+clu2.Node.size());    // divide by sum of two clusters
}

/*ClusterC mergeCluster(CC& clu1, CC& clu2, hashlinks L, double distance) //merge function
{
    double simBtwValue =0;
    simBtwValue= simBtwCluster(clu1, clu2, L);
    if( simBtwValue > 1/distance)                       //merge if similarity> 1/distance
    {
        for(vector<int>::iterator j= clu2.Node.begin(); j!=clu2.Node.end(); j++)
        {
            clu1.Node.push_back(*j);
            clu1.elementNum++;
        }
    }
    return clu1;
}
*/
void Sim_BtwCluster_Hash(CSet cluSet, hashlinks node_L, hashlinks& clu_L)
{

    double value;
    int k, m;
    
    for(k=0; k<cluSet.ElemCluster.size(); k++)
    {
        clu_L.hash_value[k][k]= 1;
        for( m=k+1; m< cluSet.ElemCluster.size(); m++)
        {
            value= simBtwCluster(cluSet.ElemCluster[k], cluSet.ElemCluster[m], node_L);
            clu_L.hash_value[k][m]= value;
       //    cout<<k << '\t' << m << ":  " <<clu_L.hash_value[k][m]<< endl;
        }
        
    }
}

/*
CSet MergeCluster_FromSet(CSet cluSet, int k, int m, double distance)
{
 //   double MaxsimValue= 0;
 //   int k,m;                //index of two merging clusters
    CSet C= cluSet;
    CSet newSet;
    CC v;
 //   for(int i=0; i<cluSet.size; i++)
        for(int j=i+1; j<cluSet.size; j++)
    {
        if(clu_L.hash_value[i][j]> MaxsimValue)
        {
            MaxsimValue= clu_L.hash_value[i][j];
            k=i;
            m=j;
        }
    }
  //  cout<< clu_L.hash_value[k][m];
   // if( MaxsimValue >=1/distance)
   // {
        //for(vector<int>::iterator i=C.ElemCluster[k].Node.begin(); i!=C.ElemCluster[k].Node.end(); i++)
            
 
        set_union(C.ElemCluster[k].Node.begin(),C.ElemCluster[k].Node.end(),C.ElemCluster[m].Node.begin(), C.ElemCluster[m].Node.end(), back_inserter(v.Node));
        
        newSet.ElemCluster.push_back(v);
        cout<< v.Node.size()<< endl;
        newSet.ElemCluster.begin()->elementNum= v.Node.size();
  //      clu_L.hash_value[k][m]=0;
  //  }
    for(int i=0; i<C.ElemCluster.size();i++)
        if(i!=k && i!= m)
        {
            newSet.ElemCluster.push_back(C.ElemCluster[i]);
           // cout<< i<< '\t' << k<< endl;
        }
    newSet.size= cluSet.size-1;
    
    return newSet;
}
*/
CC Merge_Set(CSet cluSet, int k, int m)
{
    CSet C= cluSet;
    CC v;
    
    set_union(C.ElemCluster[k].Node.begin(),C.ElemCluster[k].Node.end(),C.ElemCluster[m].Node.begin(), C.ElemCluster[m].Node.end(), back_inserter(v.Node));
    return v;
}
CSet Merge_EveryLevel(CSet cluSet, CSet& newSet, hashlinks& clu_L, double distance)
{
    double MaxsimValue=0;
    int k, m;
    int i=0;
    vector<int> flag;       //store clusters without being merged
    CC v;
    for(int k=0; k<cluSet.size; k++)
        flag.push_back(k);
    
    MaxsimValue= Max_Sim_Value(cluSet, k, m, clu_L);
    
    while(MaxsimValue >=1/ distance){
        v= Merge_Set(cluSet, k, m);
        for(int i=0; i<=cluSet.size; i++)
        {
            clu_L.hash_value[k][i]=0;
            clu_L.hash_value[i][k]=0;
        }
        for(int j=0; j<=cluSet.size; j++)
        {
            clu_L.hash_value[m][j]=0;
            clu_L.hash_value[j][m]=0;
        }

        newSet.ElemCluster.push_back(v);
        newSet.ElemCluster[i].elementNum= int (v.Node.size());
        i++;
        newSet.size++;
        flag.at(k)= -1;
        flag.at(m)= -1;
        MaxsimValue= Max_Sim_Value(cluSet, k, m, clu_L);
       // cout<< k << '\t' << m << endl;
        
    }
    
    for(int j=0; j<flag.size(); j++)
        if(flag[j]!= -1)
        {
            newSet.ElemCluster.push_back(cluSet.ElemCluster.at(flag[j]));
            
            newSet.ElemCluster[i].elementNum= cluSet.ElemCluster[flag[j]].elementNum;
            i++;
            newSet.size++;
        }
    
    return newSet;
}

double Max_Sim_Value(CSet cluSet, int& k, int& m, hashlinks& clu_L)
{
    double MaxsimValue=0;
    
    for(int i=0; i<cluSet.size; i++)
        for(int j=i+1; j<cluSet.size; j++)
        {
            if(clu_L.hash_value[i][j]>= MaxsimValue)
            {
                MaxsimValue= clu_L.hash_value[i][j];
                k=i;
                m=j;
                
            }
        }
    return MaxsimValue;
}
void InitDendrogram(CSet cluSet, Dendro& Dgram)
{
    Dgram.level=0;
    Dgram.addLevel.push_back(cluSet);
}

void construct_Hierarchy(CSet& cluSet, Dendro& Dgram, hashlinks node_L, double distance, double IntraSim_Value, int &BestLevel)
{
    int prevSize = cluSet.size;
    double temp;
  //  int BestLevel;
    hashlinks clu_L;
    CSet newSet;
    Sim_BtwCluster_Hash(cluSet,node_L, clu_L);
    
    cout<< "Before merging: \t" << IntraSim_Value <<endl;
    
    cluSet= Merge_EveryLevel(cluSet, newSet, clu_L, distance);
    temp= computeIntraSim(cluSet, node_L);
   
    cout<< "After merging: \t" << temp <<endl;;

    if( prevSize> cluSet.size)
    {
        Dgram.addLevel.push_back(cluSet);
        Dgram.level ++;
        prevSize= cluSet.size;
    }
    if(temp> IntraSim_Value){
        IntraSim_Value= temp;
        BestLevel= Dgram.level;
    }
}

