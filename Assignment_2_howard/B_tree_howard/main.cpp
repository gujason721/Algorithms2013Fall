#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
using namespace std;

#define BTree_D   2
#define ORDER    (BTree_D * 2)
#define cmp(a, b) ( ( ((a)-(b)) >= (0) ) ? (1) : (0) )

typedef int KeyType;
typedef struct BTNode{
    int keynum; // The number of keys
    KeyType key[ORDER-1];  // vector of keys, the number is t-1
    struct BTNode * child[ORDER]; // the child pointers which points to the nodes, and has number as t
    bool isLeaf;  // indicates whether it is leaf or not
}BTNode;

typedef BTNode * BTree; //define a B-tree

void BTree_insert_nonfull(BTNode * node, KeyType key);
void BTree_insert(BTree * tree, KeyType key);
void BTree_split_child(BTNode * parent, int index, BTNode * node);
void BTree_create(BTree * tree, const KeyType * data, int length);
void BTree_merge_child(BTree* tree, BTNode* node, int index);
void BTree_remove(BTree* tree, KeyType key);
void BTree_recursive_remove(BTree* tree, KeyType key);
void BTree_merge_child(BTree* tree, BTNode* node, int index);
BTNode * BTree_search(const BTree tree, int key, int *position);

void traverse(const BTree Tree);


int main(){

    KeyType array[] = {
        'G','G', 'M', 'M', 'P', 'P', 'X', 'A', 'C', 'D', 'E', 'J', 'K',
        'N', 'O', 'R', 'S', 'T', 'U', 'V', 'Y', 'Z', 'F', 'X'
    };
    KeyType ary2[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };
    srand(time(NULL));
    int idx1, idx2;


    const int length_1 = sizeof(array)/sizeof(KeyType);
    const int length_2 = sizeof(ary2)/sizeof(ary2);
    BTree tree = NULL;
    BTNode* node = NULL;
    int pos = -1;

    BTree_create(&tree, array, length_1);
    printf("\nTraverse the B tree:\n");
    traverse(tree);

    idx1 = rand() % length_1;
    idx2 = rand() % length_2;

    // search
    BTree_search(tree, array[idx1], &pos);
    if(pos != -1){
        printf("\nThe key %c has been found in the tree.\n", array[idx1]);
    }
    else{
        printf("\nThe key %c was not in the tree.\n", array[idx1]);
    }

    BTree_search(tree, ary2[idx2], &pos);
    if(pos != -1){
        printf("\nThe key %c has been found in the tree.\n", ary2[idx2]);
    }
    else{
        printf("\nThe key %c was not in the tree.\n", ary2[idx2]);
    }


    BTree_remove(&tree, array[idx1]);
    printf("\n");
    printf("After deletion:\n");
    traverse(tree);

    //insert a new key
    printf("\nInsert the character %c \n", ary2[idx2]);
    BTree_insert(&tree, ary2[idx2]);
    traverse(tree);
    printf("\n");

    return 0;
}



void traverse(const BTree Tree){
    int i;
    for (i = 0; i < Tree->keynum; i++){
        if(!Tree->isLeaf){
            traverse(Tree->child[i]);
            //printf("%c\t", Tree->key[i]);
        }
        printf("%c\t", Tree->key[i]);
    }
    if(!Tree->isLeaf)
        traverse(Tree->child[i]);
}

BTNode* BTree_search(const BTree tree, KeyType k, int *position){

    if(!tree){
        printf("BTree is empty. \n");
        return NULL;
    }

    int i = 0;
    *position = -1;

    while(i < tree->keynum && k > tree->key[i]){
        ++i;
    }
    if (i < tree->keynum && k == tree->key[i]){
        *position = i;
        return tree;
    }

    else if (tree->isLeaf){
        return NULL;
    }

    else{
        return BTree_search(tree->child[i], k, position);
    }
}

void BTree_split_child(BTNode * parent, int index, BTNode * node){
    BTNode * z = (BTNode *)malloc(sizeof(BTNode));// create a new node
    int i;

    if(!z){
        printf("Error! out of memory!\n");
    }

    z->isLeaf = node->isLeaf;
    z->keynum = BTree_D - 1; // z.n = t - 1

    for (i=0; i<z->keynum; i++){
        z->key[i] = node->key[BTree_D + i];
        //node->key[BTree_D + i] = 0;
    }

    if (! node->isLeaf ){
        for(i = 0; i < BTree_D; i++){
            z->child[i] = node->child[BTree_D + i];
            //node->child[BTree_D + i] = NULL;
        }
    }

    node->keynum = BTree_D - 1;

    // deal with the parent nodes
    for (i = parent->keynum; i > index; --i){
        parent->child[i+1] = parent->child[i];
    }

    parent->child[index + 1] = z;

    for (i = parent->keynum-1; i >= index; --i){
        parent->key[i+1] = parent->key[i];
    }
    parent->key[index] = node->key[BTree_D - 1];
    parent->keynum += 1;
    //node->key[BTree_D - 1] = 0;
}

void BTree_insert_nonfull(BTNode * node, KeyType key){
    int i;
    i = node->keynum - 1;

    if(node->isLeaf){
        while(i >=0 && key < node->key[i]){
            node->key[i+1] = node->key[i];
            --i;
        }
        node->key[i+1] = key;
        node->keynum = node->keynum + 1;
    }

    else{
        while( i>=0 && key < node->key[i])
            --i;
        ++i;

        if(node->child[i]->keynum == (ORDER-1)){
            BTree_split_child(node, i, node->child[i]);

            if(key > node->key[i])
                ++i;
        }
        BTree_insert_nonfull(node->child[i], key);
    }
}

void BTree_insert(BTree * tree, KeyType key){
    BTNode * root = *tree;
    //BTNode * dino;

    if(NULL == root){
        root = (BTNode *)malloc(sizeof(BTNode));
        if(!root){
            printf("Error! out of memory!\n");
            return;
        }
        root->isLeaf = true;
        root->keynum = 1;
        root->key[0] = key;

        *tree = root;
        return;
    }

    if(root->keynum == ORDER-1){//root is full
        BTNode * dino = (BTNode *)malloc(sizeof(BTNode));
        if(!dino){
            printf("Error! out of memeory!\n");
            return;
        }
        dino->isLeaf = false;
        dino->keynum = 0;
        dino->child[0] = root;

        *tree = dino; // tree parent is node dino

        BTree_split_child(dino, 0, root);
        BTree_insert_nonfull(dino, key);
    }

    else
    BTree_insert_nonfull(root, key);
}

void BTree_create(BTree * tree, const KeyType * data, int length){
    int i;

    printf("Creating the B-tree:\n");
    for (i = 0; i<length; i++){
        printf("%c ", data[i]);
    }
    printf("\n");

    for (i = 0; i<length; i++){
        int pos = -1;
        BTree_search(*tree, data[i], &pos);
        if(pos != -1){
            printf("The key %c has already been in the tree, not insert.\n", data[i]);
        }
        else{
            BTree_insert(tree, data[i]);
        }
    }
}

///Did not check whether it is true or not yet
void BTree_merge_child(BTree* tree, BTNode* node, int index)
{
    int i;

    KeyType key = node->key[index];
    BTNode* leftChild = node->child[index];
    BTNode* rightChild = node->child[index + 1];


    leftChild->key[leftChild->keynum] = key;
    leftChild->child[leftChild->keynum + 1] = rightChild->child[0];
    ++leftChild->keynum;

    for (i = 0; i < rightChild->keynum; ++i) {
        leftChild->key[leftChild->keynum] = rightChild->key[i];
        leftChild->child[leftChild->keynum + 1] = rightChild->child[i + 1];
        ++leftChild->keynum;
    }

    for (i = index; i < node->keynum - 1; ++i) {
        node->key[i] = node->key[i + 1];
        node->child[i + 1] = node->child[i + 2];
    }
    node->key[node->keynum - 1] = 0;
    node->child[node->keynum] = NULL;
    --node->keynum;

    if (node->keynum == 0) {
        if (*tree == node) {
            *tree = leftChild;
        }

        free(node);
        node = NULL;
    }

    free(rightChild);
    rightChild = NULL;
}

void BTree_recursive_remove(BTree* tree, KeyType key)
{

    int i, j, index;
    BTNode *root = *tree;
    BTNode *node = root;

    if (!root) {
        printf("Failed to remove %c, it is not in the tree!\n", key);
        return;
    }

    index = 0;
    while (index < node->keynum && key > node->key[index]) {
        ++index;
    }
    BTNode *leftChild, *rightChild;
    KeyType leftKey, rightKey;
    if (index < node->keynum && node->key[index] == key) {

        if (node->isLeaf) {
            for (i = index; i < node->keynum-1; ++i) {
                node->key[i] = node->key[i + 1];
            }
            node->key[node->keynum-1] = 0;
            node->keynum -= 1;

            if (node->keynum == 0) {
                free(node);
                *tree = NULL;
            }

            return;
        }

        else if (node->child[index]->keynum >= BTree_D) {
            leftChild = node->child[index];
            leftKey = leftChild->key[leftChild->keynum - 1];
            node->key[index] = leftKey;

            BTree_recursive_remove(&leftChild, leftKey);
        }
        else if (node->child[index + 1]->keynum >= BTree_D) {
            rightChild = node->child[index + 1];
            rightKey = rightChild->key[0];
            node->key[index] = rightKey;

            BTree_recursive_remove(&rightChild, rightKey);
        }
        else if (node->child[index]->keynum == BTree_D - 1
            && node->child[index + 1]->keynum == BTree_D - 1){
            leftChild = node->child[index];
            BTree_merge_child(tree, node, index);
            BTree_recursive_remove(&leftChild, key);
        }
    }

    else {
        BTNode *leftSibling, *rightSibling, *child;

        child = node->child[index];
        if (!child) {
            printf("Failed to remove %c, it is not in the tree!\n", key);
            return;
        }

        if (child->keynum == BTree_D - 1) {
            leftSibling = NULL;
            rightSibling = NULL;

            if (index - 1 >= 0) {
                leftSibling = node->child[index - 1];
            }

            if (index + 1 <= node->keynum) {
                rightSibling = node->child[index + 1];
            }

            if ((leftSibling && leftSibling->keynum >= BTree_D)
                || (rightSibling && rightSibling->keynum >= BTree_D)) {
                int richR = 0;
                if(rightSibling) richR = 1;
                if(leftSibling && rightSibling) {
                    richR = cmp(rightSibling->keynum,leftSibling->keynum);
                }
                if (rightSibling && rightSibling->keynum >= BTree_D && richR) {
                    child->key[child->keynum] = node->key[index];
                    child->child[child->keynum + 1] = rightSibling->child[0];
                    ++child->keynum;

                    node->key[index] = rightSibling->key[0];

                    for (j = 0; j < rightSibling->keynum - 1; ++j) {
                        rightSibling->key[j] = rightSibling->key[j + 1];
                        rightSibling->child[j] = rightSibling->child[j + 1];
                    }
                    rightSibling->key[rightSibling->keynum-1] = 0;
                    rightSibling->child[rightSibling->keynum-1] = rightSibling->child[rightSibling->keynum];
                    rightSibling->child[rightSibling->keynum] = NULL;
                    --rightSibling->keynum;
                }
                else {
                    for (j = child->keynum; j > 0; --j) {
                        child->key[j] = child->key[j - 1];
                        child->child[j + 1] = child->child[j];
                    }
                    child->child[1] = child->child[0];
                    child->child[0] = leftSibling->child[leftSibling->keynum];
                    child->key[0] = node->key[index - 1];
                    ++child->keynum;

                    node->key[index - 1] = leftSibling->key[leftSibling->keynum - 1];

                    leftSibling->key[leftSibling->keynum - 1] = 0;
                    leftSibling->child[leftSibling->keynum] = NULL;

                    --leftSibling->keynum;
                }
            }
            else if ((!leftSibling || (leftSibling && leftSibling->keynum == BTree_D - 1))
                && (!rightSibling || (rightSibling && rightSibling->keynum == BTree_D - 1))) {
                if (leftSibling && leftSibling->keynum == BTree_D - 1) {

                    BTree_merge_child(tree, node, index - 1);

                    child = leftSibling;
                }

                else if (rightSibling && rightSibling->keynum == BTree_D - 1) {

                    BTree_merge_child(tree, node, index);
                }
            }
        }

        BTree_recursive_remove(&child, key);
    }
}

void BTree_remove(BTree* tree, KeyType key)
{
    if (*tree==NULL)
    {
        printf("BTree is NULL!\n");
        return;
    }

    BTree_recursive_remove(tree, key);
}



