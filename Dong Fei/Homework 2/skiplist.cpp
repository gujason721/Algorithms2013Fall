//
//  main.cpp
//  Alg_Project2-2
//
//  Created by D&F on 12/11/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

//Skip list implementation

#include <iostream>
//#include <math.h>

#define valueType char
//#define MaxLevel 4

using namespace std;

// SNode class
class SNode
{
public:
    //int level;
    valueType value;
    int Max_Level;
    SNode **forward;
    SNode(int size){ forward= new SNode *[size];};   //constructor of class
    ~SNode(){delete []forward;}
    
    friend class Slist;
};

//Slist class
class Slist
{
public:
    Slist(valueType tKey, int MaxL=4);                    //
    ~Slist();
    
    int MaxLevel;
    int tailKey;
    int levels;                         //current node's level
    
    SNode *head;                        //head node/pointer
    SNode *tail;                        //tail pointer
    SNode **next;                       //poniter array
    
    int level();                        //generate random level number
    
    bool Search(valueType v);
    SNode *Slist_Search(valueType v);
    
    Slist &Slist_Insert(valueType v);
    
    Slist &Slist_Delete(valueType v);
    
    
};


//Slist constructor

Slist::Slist(valueType tKey, int MaxL)                 //constructor
{
    tailKey=tKey;                                           //tail key set as a big value
    MaxLevel=MaxL;                    //calculate MaxLevel
    
    head=new SNode (MaxLevel+1);
    tail=new SNode (0);
    next=new SNode *[MaxLevel+1];
    
    tail->value=tKey;
    
    
    for (int i=0; i<=MaxLevel;i++)
        head->forward[i]=tail;

}


//Slist destructor
Slist::~Slist()
{
    SNode *p;
    
    while(head!=tail)
    {
        p=head->forward[0];
        delete head;
        head=p;
    }
    delete  tail;
    delete []next;
}


//Search function
//return find result
bool Slist::Search(valueType v)
{
    valueType e;
    if(v>=tailKey)
     return false;
    
    SNode *p=head;
    for(int i=levels; i>=0; i--)                //find the predescessor of  v
    {
        while(p->forward[i]->value<v)
            p=p->forward[i];
    }
    e=p->forward[0]->value;
    
    if(e==v)
        cout<<"Congratulations! find the result!" <<endl;
    else
        cout<<"Sorry, not found!" <<endl;
    
    return (e==v);                          //if the value of  next node equals to v, return 1.
    
}

//Slist_Search function : return SNode

SNode *Slist::Slist_Search(valueType v)  //find the predescessor of v and return the node.
{
    
    if(v>=tailKey)
        return NULL;
    
    SNode *p=head;
    for(int i=levels; i>=0; i--)                //find the predescessor node of  v
    {
        while(p->forward[i]->value<v)
            p=p->forward[i];
        next[i]=p;                              //record every matching node
    }
    
    return (p->forward[0]);
    
}


//level function
// to randomly generate a level number of SNode
int Slist::level()
{
    int lev=0;
    lev=rand()%MaxLevel;
    return lev>0?lev:lev+1;
}

// Slist_Insertion function : insert new node
Slist &Slist::Slist_Insert(valueType v)
{
    if(v>=tailKey)
    {
        cout<<"Inserting number is too big, please change a small one! "<< endl;
        return *this;
    }

    SNode *p=Slist_Search(v);
    if(p->value==v)
    {
        cout<<"Value repete, input another one!"<<endl;
        return *this;
    }
    
    int lev=level();//randomly generate a level number for new node
    
    if(lev>levels)
    {
        lev=++levels;       //fix level to levels +1 if lev >levels
        next[lev]=head;
    }
    
    SNode *y=new SNode (lev+1);
    y->value=v;
    for(int i=0; i<lev; i++)
    {
        y->forward[i]=next[i]->forward[i];
        next[i]->forward[i]=y;
    }
    return *this;
    
}

//Slist_Delete function : delete node with value v
Slist &Slist::Slist_Delete(valueType v)
{
    
    SNode *p=Slist_Search(v);
    if(v>=tailKey || p->value!=v)
    {
        cout<< "no such value!"<<endl;
        return *this;
    }
    
    for(int i=0; i<levels && next[i]->forward[i]==p; i++)       //directly delete node
    {
        next[i]->forward[i]=p->forward[i];
    }
    
    while(levels>0 && head->forward[levels]==tail)
        levels--;
    valueType e=p->value;
    cout<< "delete value:  "<< e<<endl;
    delete p;
    return *this;
}


//Main Function
int main()
{

//if valueType is int type
    
 /*   Slist SL(10000,4);                  //10000,tail node key, set as large as possible
    SL.Slist_Insert(100);
    SL.Slist_Insert(10);
    SL.Slist_Insert(17);
    SL.Slist_Insert(46);
    SL.Slist_Insert(9);
    SL.Slist_Insert(19);
    SL.Slist_Insert(88);
    SL.Search(10);
    SL.Slist_Delete(10);
    SL.Search(10);
  */

//if  valueType is char type
    valueType v1,v2;


    Slist SL('~',4);

    SL.Slist_Insert('C');
    SL.Slist_Insert('B');
    SL.Slist_Insert('h');
    SL.Slist_Insert('X');
    SL.Slist_Insert('j');
    SL.Slist_Insert('K');
//    SL.Slist_Insert('p');
    SL.Slist_Insert('Z');
    
    cout<<"Please input the key to search(A-Z, or a-z): \n";
    cin>>v1;
    
    SL.Search(v1);
    cout<<""<<endl;
    cout<<"Please input the key to delete(A-Z, or a-z): \n";
    cin>>v2;
    
    SL.Slist_Delete(v2);

    cout<< " Search after delete: "<<v2<<endl;
    SL.Search(v2);
    return 0;
}

