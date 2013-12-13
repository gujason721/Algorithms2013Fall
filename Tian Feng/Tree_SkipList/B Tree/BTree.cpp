#include "BTree.h"

BTreeNode::BTreeNode(int a, bool b){
	this ->t = a;
	this ->len = 0;
	this ->leaf = b;
	this ->key = new int[2 * a - 1]();
	this ->child = new BTreeNode*[2 * a];

	for (int i = 0; i < 2 * a; i++)
		this->child[i] = NULL;
}

BTree::BTree(int a){
	this ->t = a;
	this ->root = new BTreeNode(a, true);
}

void BTree::DeleteTree(BTreeNode* p){
	if(!p ->leaf){
		for(int i = 0; i < p ->len + 1; i ++)
			this ->DeleteTree(p ->child[i]);
	}
	delete p;
}

bool BTree::Search(int k){
	return this ->SearchHelper(this ->root, k);
}

bool BTree::SearchHelper(BTreeNode* x, int k){
	int i;
	if(x ->leaf){
		for(i = 0; i < x ->len; i ++)
			if(x ->key[i] == k)
				return true;
		return false;
	}
	else{
		for(i = 0; i < x ->len; i ++)
			if(x ->key[i] > k)
				break;
		return this ->SearchHelper(x ->child[i], k);
	}
}

void BTree::Insert(int k){
	BTreeNode* r = this ->root;
	if(this ->root ->len == 2 * this ->t - 1){
		BTreeNode* s = new BTreeNode(this ->t, false);
		this ->root = s;
		s ->child[0] = r;
		this ->SplitChild(s, 0, r);
		this ->InsertNonfull(s, k);
	}
	else
		this ->InsertNonfull(r, k);
}

void BTree::SplitChild(BTreeNode* x, int i, BTreeNode* y){
	BTreeNode* z = new BTreeNode(this ->t, y ->leaf);
	int j;
	z ->len = this ->t - 1;
	for(j = 0; j < t -1 ; j ++)
		z ->key[j] = y ->key[this ->t + j];
	if(!y ->leaf){
		for(j = 0; j < t; j ++)
			z ->child[j] = y ->child[this -> t + j];
	}
	y ->len = this ->t - 1;

	for(j = x ->len; j > i + 1; j --)
		x ->child[j + 1] = x ->child[j];
	x ->child[i + 1] = z;
	for(j = x ->len - 1; j > i; j --)
		x ->key[j + 1] = x ->key[j];
	x ->key[i] = y ->key[this ->t - 1];
	x ->len ++;
}

void BTree::InsertNonfull(BTreeNode* x, int k){
	int i = x ->len;
	if(x ->leaf){
		while(i > 0 && k < x ->key[i - 1]){
			x ->key[i] = x ->key[i - 1];
			i --;
		}
		x ->key[i] = k;
		x ->len ++;
	}
	else{
		while(i > 0 && k < x ->key[i - 1])
			i --;
		if(x ->child[i] ->len == 2 * this ->t - 1){
			this ->SplitChild(x, i , x ->child[i]);
			if(k > x ->key[i])
				i ++;
		}
		this ->InsertNonfull(x ->child[i], k);
	}
}

bool BTree::Delete(int k){
	BTreeNode* p = this ->root;
	if(this ->root ->len == 1 && !this ->root ->leaf
		&& this ->root ->child[0] ->len == this ->t - 1
		&& this ->root ->child[1] ->len == this ->t - 1){
			this ->MergeChild(this ->root, 0);
			this ->root = this ->root ->child[0];
			delete p;
			return this ->DeleteHelper(this ->root, k);
	}
	return this ->DeleteHelper(p, k);
}

void BTree::MergeChild(BTreeNode* x, int i){
	int j;
	BTreeNode* y = x ->child[i];
	BTreeNode* z = x ->child[i + 1];
	// assert delete
	if(y ->len != this ->t - 1 || z ->len != this ->t - 1){
		printf("Error delete!\n");
		exit(0);
	}
	y ->len = this ->t * 2 - 1; 
	for(j = 0; j < this ->t - 1; j ++)
		y ->key[this ->t + j] = z ->key[j];
	y ->key[this ->t - 1] = x ->key[i];

	if(!z ->leaf){
		for(j = 0; j < this ->t; j ++)
			y ->child[this ->t + j] = z ->child[j];
	}
	for(j = i; j < x ->len - 1; j ++)
		x ->key[j] = x ->key[j + 1];
	for(j = i + 1; j < x ->len; j ++)
		x ->child[j] = x ->child[j + 1];
	x ->len --;
}

void BTree::RotateChild(BTreeNode* x, int i, bool rol_right){
	BTreeNode* y;
	BTreeNode* z;
	int j;
	if(rol_right){
		y = x ->child[i];
		z = x ->child[i + 1];
		// change right child
		for(j = z ->len; j > 0; j --)
			z ->key[j] = z ->key[j - 1];
		for(j = z ->len + 1; j > 0; j --)
			z ->child[j] = z ->child[j - 1];
		z ->key[0] = x ->key[i];
		z ->child[0] = y ->child[y ->len];
		z ->len ++;
		// change parant
		x ->key[i] = y ->key[y ->len - 1];
		// change left child
		y ->len --;
	}
	else{
		y = x ->child[i - 1];
		z = x ->child[i];
		// change left child
		y ->key[y ->len] = x ->key[i - 1];
		y ->child[y ->len + 1] = z ->child[0];
		y ->len ++;
		// change parent
		x ->key[i - 1] = z ->key[0];
		// change right child
		for(j = 0; j < z ->len - 1; j ++)
			z ->key[j] = z ->key[j + 1];
		for(j = 0; j < z ->len; j ++)
			z ->child[j] = z ->child[j + 1];
		z ->len --;
	}
}

bool BTree::DeleteHelper(BTreeNode* x, int k){
	int i;
	if(x ->leaf){
		for(i = 0; i < x ->len; i ++){
			if(x ->key[i] == k){
				while(i < x ->len - 1){
					//x ->key[i] = x ->key[i ++];
					x ->key[i] = x ->key[i + 1];
					i ++;
				}
				x ->len --;
				return true;
			}
		}
		return false;
	}
	else{
		for(i = 0; i < x ->len; i ++){
			if(x ->key[i] > k)
				break;
		}
		if(x ->child[i] ->len > this ->t - 1)
			return this ->DeleteHelper(x ->child[i], k);
		else{
			if(i == 0){
				if(x ->child[i + 1] ->len > this ->t - 1)
					this ->RotateChild(x, i + 1, false);
				else
					this ->MergeChild(x, i);
				return this ->DeleteHelper(x ->child[i], k);
			}
			else if(i == x ->len){
				if(x ->child[i - 1] ->len > this ->t - 1)
					this ->RotateChild(x, i - 1, true);
				else
					this ->MergeChild(x, i - 1);
				return this ->DeleteHelper(x ->child[i - 1], k);
			}
			else{
				if(x ->child[i - 1] ->len > this ->t - 1)
					this ->RotateChild(x, i - 1, true);
				else if(x ->child[i + 1] ->len > this -> t - 1)
					this ->RotateChild(x, i + 1, false);
				else
					this ->MergeChild(x, i);
				return this ->DeleteHelper(x ->child[i], k);
			}
		}
	}
}

void BTree::Show(BTreeNode* node)
{
	vector<BTreeNode*> nodes;
	nodes.push_back(node);

	int level = 0;
	while (!nodes.empty()){
		vector<BTreeNode*> temp;
		
		cout<<"Level "<<level<<": ";
		
		for (int i = 0; i < nodes.size(); i++)
		{
			BTreeNode* n = nodes[i];			
			cout<<"[ ";
			for (int i = 0; i < n->len; i++)
			{
				cout<<n->key[i]<<" ";
			}
			cout<<"]";
			temp.push_back(n);
		}
		cout<<endl;
		nodes.clear();
		level++;

		for (int i = 0; i < temp.size(); i++){
			BTreeNode* n = temp[i];
			for (int j = 0; j < n->len + 1; j++)
			{
				BTreeNode* m = n->child[j];
				if (m == NULL)
					continue;
				nodes.push_back(m);
			}
		}
	}


// 	if(node->leaf)
// 	{
// 		if(node->len == 0)
// 			cout<<"NIL | LEAF\n";
// 
// 		for(int i = 0; i < node->len; i++)
// 			cout<<node->key[i]<<" | LEAF\n";
// 	}
// 	else
// 	{
// 		for(int i = 0; i < node->len; i++)
// 		{
// 			Show(node->child[i]);
// 
// 			if (i < node->len)
// 			{
// 				if (node == root)
// 					cout<<node->key[i]<<" | ROOT\n";
// 				else
// 					cout<<node->key[i]<<" | INTERNAL NODE\n";
// 			}
// 		}
// 	}
}