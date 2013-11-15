#ifndef _RED_BLACK_TREE_H
#define _RED_BLACK_TREE_H

#include <cstdlib>
#include <iostream>
using namespace std;

enum TranverseOrder
{
	IN_ORDER = 0,
	PRE_ORDER = 1,
	POST_ORDER = 2
};

enum RBTreeNodeColor
{
	BLACK = 0,
	RED = 1
};

struct RBTreeNode
{
	int key;
	RBTreeNodeColor color;
	RBTreeNode *left, *right;
	RBTreeNode *p;

	RBTreeNode();
};

class RBTree
{
public:
	RBTreeNode *root;
	RBTreeNode *nil;

	RBTree();
	RBTree(RBTreeNode* root);

	void LeftRotate(RBTreeNode* x);
	void RightRotate(RBTreeNode* y);
	bool Insert(int key);
	void InsertFixup(RBTreeNode* z);
	void Transplant(RBTreeNode* u, RBTreeNode* v);
	bool Delete(int key);
	void DeleteFixup(RBTreeNode* x);
	RBTreeNode* Minimum(RBTreeNode* root);
	void Tranverse(RBTreeNode* root, TranverseOrder order);
	RBTreeNode* Search(RBTreeNode* root, int target);
};
#endif