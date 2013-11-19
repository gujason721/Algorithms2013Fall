//
//  main.cpp
//  Alg_Project2
//
//  Created by D&F on 7/11/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <iomanip>
#include <queue>

#define SIZE 10                     //the initial number of elements in red-black tree
#define RED 0
#define BLACK 1

using namespace std;


typedef struct RBTNode{
    int key;
    struct RBTNode *parent, *left, *right;
    int color;
    int size;
} RBTNode, *RBTree;

RBTree nil, root;               //define nil nodes and root node

//Initialize leaf nodes and root nodes
void Init_leafNode(){
    nil=new RBTNode;
    root= new RBTNode;
    nil->color= BLACK;
    nil->left=nil->right=NULL;
    nil->key=-1;
    nil->size=0;                //initial size 0
    root=nil;                   //root points to nil node
}

//Create a RBTree Node
RBTree Create_node(int key, int color){
    RBTree x;
    x=new RBTNode;
    x->color=color;
    x->key=key;
    x->left=nil; x->right=nil;
    x->parent=NULL;
    x->size=1;
    return x;
}

//RBTree Minimum Node
RBTree Tree_minimum(RBTree x){
    while(x->left!=nil)
        x=x->left;
    return  x;
}

//RBTree Maxmimum Node
RBTree Tree_maxmimum(RBTree x){
    while(x->right !=nil)
        x=x->right;
    return  x;
}

//RBTree node successor
RBTree Tree_successor(RBTree x){
    RBTree y;
    if (x->right != nil) return Tree_minimum(x->right);
    y=x->parent;
    while (y!=nil && x==y->right){
        x=y;
        y=y->parent;
    }
    return  y;
}


//RBTree Left Rotation
void Left_rotate(RBTree &T, RBTree x){
    RBTree y; y=new RBTNode;
    y=x->right;
    x->right=y->left;
    if(y->left !=nil) y->left->parent=x;
    y->parent=x->parent;
    if(x->parent == nil)        //x is root node
        root=y;
    else if (x== x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;
    y->left=x;
    x->parent=y;
    
    //setting size
    y->size= x->size;
    x->size= x->right->size + x->left->size +1;
}

//RBTree Right Rotation
void Right_rotate(RBTree &T, RBTree x){
    RBTree y; y=new RBTNode;
    y=x->left;
    x->left=y->right;
    if(y->right !=nil) y->right->parent=x;
    y->parent=x->parent;
    if(x->parent ==nil) root=y;
    else if (x==x->parent->left) x->parent->left=y;
    else x->parent->right=y;
    
    y->right=x;
    x->parent=y;
    
    //setting nodes size
    y->size=x->size;
    x->size=x->right->size + x->left->size +1;
    
}

//Tree insert function
void Tree_insert(RBTree &T, RBTree z){
    RBTree x=root, y=nil;
    while (x != nil){
        y=x;
        if (z->key < y->key)
            x=x->left;
        else x=x->right;
    }
    z->parent=y;
        
    if (y==nil) root=z;
    else if (z->key <y->key)
        y->left=z;
    else y->right=z;
    z->left=nil; z->right=nil;
    z->color=RED;
    
    //setting nodes size
    while (z !=root){
        z->parent->size =z->parent->right->size + z->parent->left->size +1;
        z=z->parent;
    }

}

//RBTree insert fixup function
void RBTree_insert_Fixup(RBTree &T, RBTree z)
{
    RBTree y;
    while (z->parent->color == RED)
    {
        if(z->parent ==z->parent->parent->left)
        {
            y=z->parent->parent->right;
            
            if(y->color ==RED)
            {
                z->parent->color =BLACK;                //case 1
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;                    //case 1
            }
            
            else
            {
                if(z== z->parent->right)
                {
                    z=z->parent;                        //case 2
                    Left_rotate(T, z);                  //case 2
                }
                z->parent->color =BLACK;                //case 3
                z->parent->parent->color= RED;
                Right_rotate(T, z->parent->parent);     //case 3
            }
        }
        else                                            // opposite to the above
        {
            y=z->parent->parent->left;
            
            if(y->color ==RED)
            {
                z->parent->color=BLACK;
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;
            }
            
            else
            {
                if (z==z->parent->left)
                {
                    z=z->parent;
                    Right_rotate(T, z);
                }
                
                z->parent->color =BLACK;
                z->parent->parent->color= RED;
                Left_rotate(T, z->parent->parent);
            }
        }
        
    }
    
    root->color =BLACK;
}

//RBTree insert function
void RBTree_insert(RBTree &T, RBTree z){
    Tree_insert(T,z );
    RBTree_insert_Fixup(T,z);
}

//RBTree Delete Fixup function
void RBTree_delete_Fixup(RBTree &T, RBTree x)
{
    RBTree w;
    while ( x!=root && x->color ==BLACK)
    {
        if(x == x->parent->left)
        {
            w=x->parent->right;
            if(w->color ==RED)
            {
                w->color=BLACK;
                x->parent->color =RED;
                Left_rotate(T, x->parent);
                w=x->parent->right;
        
            }
            if(w->left->color ==BLACK && w->right->color ==BLACK)
            {
                w->color=RED;
                x=x->parent;
            }
            else
            {
                if(w->right->color == BLACK)
                {
                    w->left->color= BLACK;
                    w->color = RED;
                    Right_rotate(T, w);
                    w=x->parent->right;
                }
                
                w->color = x->parent->color;
                x->parent->color =BLACK;
                w->right->color =BLACK;
                Left_rotate(T, x->parent);
                x=root;
            }
        
        }
        else
        {
            w=x->parent->left;
            if(w->color ==RED)
            {
                w->color=BLACK;
                x->parent->color =RED;
                Right_rotate(T, x->parent);
                w=x->parent->left;
                
            }
            if(w->left->color ==BLACK && w->right->color ==BLACK)
            {
                w->color=RED;
                x=x->parent;
            }
            else
            {
                if(w->left->color == BLACK)
                {
                    w->right->color= BLACK;
                    w->color = RED;
                    Left_rotate(T, w);
                    w=x->parent->left;
                }
                
                w->color = x->parent->color;
                x->parent->color =BLACK;
                w->left->color =BLACK;
                Right_rotate(T, x->parent);
                x=root;
            }
        }
    }
    
}


//RBTree Delete Function
void RBTree_delete(RBTree &T, RBTree z)
{
    RBTree x, y;
    if (z->left ==nil || z->right == nil) y=z;          //only left or right child
    else y=Tree_successor(z);                           //have both left and right children
    
    if (y->left ==nil ) x=y->left;
    else x=y->right;
    
    x->parent= y->parent;                               //let parents of y's child points to y's parents
    if(y->parent == nil)                                //y's parent is root
        root =x;
    else
    {
        if(y == y->parent->left) y->parent->left = x;   //y is left child of its parent
        else y->parent->right = x;
    }
    
    if (y!=z) z->key = y->key;                          //
    if (y->color ==BLACK) RBTree_delete_Fixup(T, x);
    free(y);
}

//RBTree Search Function
RBTree RBTree_Search(RBTree T, int key, int &result)
{
    RBTree Tree =T;
    while (Tree !=nil) {
        if (key == Tree->key)
        {
            result =1; break;                           // find the right key
            
        }
        else if (key < Tree->key) Tree= Tree->left;
        else Tree= Tree->right;
     
    }
    if(Tree ==nil) result =0;                           // cannot find the key
    return Tree;
}


//RBTree Traverse
void Inorder_Tree_walk(RBTree x)
{
    if(x!= nil)
    {
        Inorder_Tree_walk(x->left);
        cout<<"V: "<<setw(3) << x->key << '\t';
        cout<<"C: "<<setw(3)<< x->color <<endl;
        Inorder_Tree_walk(x->right);
    }
    return;
}

//Preorder Tree walk based on queue
void Preorder_Tree_walk(RBTree x)
{
    std::queue<RBTree> q;
    RBTree temp;
    if(x !=nil)
    {
        q.push(x);
        while(! q.empty())
        {
            temp=q.front(),q.pop();
            cout << temp->key << '\t';
          //  cout << temp->color << endl;
            if(temp->left !=nil) q.push(temp->left);
            if(temp->right !=nil) q.push(temp->right);
            
        }
        cout << endl;
    }
}

//Preorder Tree  walk based on recursion
void Preorder_Tree_walk2(RBTree x)
{
    if(x!=nil)
    {
        cout<<"V: "<<setw(3) << x->key << '\t';
        cout<<"C: "<<setw(3)<< x->color <<endl;
        Preorder_Tree_walk2(x->left);
        Preorder_Tree_walk2(x->right);

    }
    return;
}

//Postorder Tree walk function
void Postorder_Tree_walk(RBTree x)
{
    if(x!=nil)
    {
        Postorder_Tree_walk(x->left);
        Postorder_Tree_walk(x->right);
        cout<<"V: "<<setw(3) << x->key << '\t';
        cout<<"C: "<<setw(3)<< x->color <<endl;
        
    }
    return;
}

int main()
{
   
    int A[SIZE]={13,10,4,6,1,7,5,2,9,28};
    
    int key;
    int search_result=0;                            // success:1, failure: 0
    
    RBTree Node[SIZE];
    for(int i=0; i<SIZE; i++)

    Init_leafNode();                                //initialize leaf nodes and root node
    
    //Construct Red-black Tree
    for (int j=0; j<SIZE; j++)
    {
        Node[j]= Create_node(A[j], RED);
        RBTree_insert(root, Node[j]);
    }
    cout<< "Inorder Tree Walk" <<endl;
    Inorder_Tree_walk(root);
    
    cout<< "Preorder Tree Walk" <<endl;
    Preorder_Tree_walk2(root);
    
    cout<< "Postorder Tree Walk" <<endl;
    Postorder_Tree_walk(root);
    
   // cout<< "Please choose operations:\n";
    
    //Output the maximum number!
    RBTree Max=Tree_maxmimum(root);
    cout<<"The maximum is  "<< Max->key<< endl;
    
    
    //Output the moinimum number!
    RBTree Min=Tree_minimum(root);
    cout<<"The minimum is  "<< Min->key<< endl;
    
    
    
    cout<< "Please input the number you want to search(0-30): \n";
    cin>> key;
    
    RBTree_Search(root, key, search_result);
    if(search_result==1)
    cout<< "Find the key!" << endl;                       //if search is successful, output 1
    else
        cout<< "Not find!"<<endl;
    return 0;
}

