#ifndef _B_TREE_H
#define _B_TREE_H

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class BTreeNode{
public:
	BTreeNode(int, bool);

	int t;
	int len;
	bool leaf;
	int* key;
	BTreeNode** child;
};

class BTree{
public:
	BTreeNode* root;
	int t;

	BTree(int);
	bool Search(int);
	void Insert(int);
	bool Delete(int);
	void Show(BTreeNode* node);

private:
	bool SearchHelper(BTreeNode*, int);
	void DeleteTree(BTreeNode*);
	void SplitChild(BTreeNode*, int, BTreeNode*);
	void InsertNonfull(BTreeNode*, int);
	void MergeChild(BTreeNode*, int);
	void RotateChild(BTreeNode*, int, bool);
	bool DeleteHelper(BTreeNode*, int);
};

#endif
