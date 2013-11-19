//
//  hashLinks.cpp
//  OntoMatch
//
//  Created by D&F on 8/12/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

#include "hashLinks.h"

//find common context of two nodes v1 and v2
int find_common(vPtr v1, vPtr v2)
{
    int cNum=0;
    for(vector<int>:: iterator i= v1->Ctext.begin(); i!= v1->Ctext.end(); i++)
        for(vector<int>:: iterator j= v2->Ctext.begin(); j!= v2->Ctext.end(); j++)
            if(*i== *j) cNum++;
    //  cout<< cNum <<endl;
    return cNum;
}


//calculate similarity of two nodes/vertices
double vSim(vPtr v1, vPtr v2)
{
    double sim_value=0;
    double cNum = 0, v1_Num =1, v2_Num =1;
    
    cNum = find_common(v1, v2);
    v1_Num = v1->Ctext.size();
    v2_Num = v2->Ctext.size();
    
    sim_value = cNum/sqrt(v1_Num* v2_Num);
    
    return sim_value;
};

hashlinks constructLinks(hashlinks links, SG sgraph, double thValue)
{
    SG g = sgraph;
    int vNum = sgraph->vertexNum;
    double vertSim= 0;
    
    //calculate similarity of parent node and children nodes
    for (int i=0; i< vNum; i++)
    {
        for(vector<int>::iterator j= g->vecNode[i].Ctext.begin(); j!= g->vecNode[i].Ctext.end(); j++)
        {
            vertSim = vSim(&g->vecNode[i], &g->vecNode[*j]);
            if(i != *j && vertSim >=thValue)
            {
                links.hash_value[i][*j] = vertSim;
                links.hash_value[*j][i] = vertSim;
            }
            else
            {
                links.hash_value[i][*j] = 0;
                links.hash_value[*j][i] = 0;
                
            }
        }
    }
    //calculate similarity among sibling nodes based on parents i
    for (int i=0; i< vNum; i++)
    {
        
        for(vector<int>::iterator j= g->vecNode[i].Ctext.begin()+1; j!= g->vecNode[i].Ctext.end(); j++)
        {
            for( vector<int>:: iterator k= g->vecNode[i].Ctext.begin(); k< j; k++)
            {
                vertSim = vSim(&g->vecNode[*k], &g->vecNode[*j]);
                if(*k != *j && vertSim >=thValue)
                {
                    links.hash_value[*k][*j] = vertSim;
                    links.hash_value[*j][*k] = vertSim;
                }
                else
                {
                    links.hash_value[*k][*j] = 0;
                    links.hash_value[*j][*k] = 0;
                }
            }
        }
    }
    for(int i= 0; i<vNum; i++)
        links.hash_value[i][i]= 1;
    return links;
}
