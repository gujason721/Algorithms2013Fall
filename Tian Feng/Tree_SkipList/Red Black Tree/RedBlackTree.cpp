#include "RedBlackTree.h"

RBTreeNode::RBTreeNode()
{

}

RBTree::RBTree()
{
	this->nil = new RBTreeNode();
	this->nil->color = RBTreeNodeColor::BLACK;
	this->nil->left = NULL;
	this->nil->right = NULL;
	this->nil->p = NULL;
	this->root = this->nil;
}

RBTree::RBTree(RBTreeNode* root)
{
	this->nil = new RBTreeNode();
	this->nil->color = RBTreeNodeColor::BLACK;
	this->nil->left = NULL;
	this->nil->right = NULL;
	this->nil->p = NULL;
	this->root = root;
}

void RBTree::LeftRotate(RBTreeNode* x)
{
	RBTreeNode *y = x->right;
	x->right = y->left;
	if (y->left != this->nil)
		y->left->p = x;

	y->p = x->p;
	if (x->p == this->nil)
		this->root = y;
	else if (x == x->p->left)
		x->p->left = y;
	else
		x->p->right = y;

	y->left = x;
	x->p = y;
}

void RBTree::RightRotate(RBTreeNode* y)
{
	RBTreeNode *x = y->left;
	y->left = x->right;
	if (x->right != this->nil)
		x->right->p = y;

	x->p = y->p;
	if (y->p == this->nil)
		this->root = x;
	else if (y == y->p->left)
		y->p->left = x;
	else
		y->p->right = x;

	x->right = y;
	y->p = x;
}

bool RBTree::Insert(int key)
{
	RBTreeNode* z = this->Search(this->root, key);
	if (z != this->nil)
		return false;

	z = new RBTreeNode();
	z->key = key;

	RBTreeNode *x = this->root;
	RBTreeNode *y = this->nil;
	while (x != this->nil)
	{
		y = x;
		if (z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	z->p = y;
	if (y == this->nil)
		this->root = z;
	else if (z->key < y->key)
		y->left = z;
	else
		y->right = z;

	z->left = this->nil;
	z->right = this->nil;
	z->color = RBTreeNodeColor::RED;

	InsertFixup(z);
	return true;
}

void RBTree::InsertFixup(RBTreeNode* z)
{
	RBTreeNode *y = this->nil;
	while (z->p->color == RBTreeNodeColor::RED)
	{
		if (z->p == z->p->p->left)
		{
			y = z->p->p->right;

			if (y->color == RBTreeNodeColor::RED) //CASE 1
			{
				z->p->color = RBTreeNodeColor::BLACK;
				y->color = RBTreeNodeColor::BLACK;
				z->p->p->color = RBTreeNodeColor::RED;
				z = z->p->p;
			} 
			else
			{
				if (z == z->p->right) //CASE 2
				{
					z = z->p;
					this->LeftRotate(z);
				}
				
				//CASE 3
				z->p->color = RBTreeNodeColor::BLACK;
				z->p->p->color = RBTreeNodeColor::RED;
				this->RightRotate(z->p->p);
			}
		} 
		else
		{
			y = z->p->p->left;
			
			if (y != this->nil && y->color == RBTreeNodeColor::RED) //CASE 1
			{
				z->p->color = RBTreeNodeColor::BLACK;
				y->color = RBTreeNodeColor::BLACK;
				z->p->p->color = RBTreeNodeColor::RED;
				z = z->p->p;
			} 
			else 
			{
				if (z == z->p->left) //CASE 2
				{
					z = z->p;
					this->RightRotate(z);
				}

				//CASE 3
				z->p->color = RBTreeNodeColor::BLACK;
				z->p->p->color = RBTreeNodeColor::RED;
				this->LeftRotate(z->p->p);
			}
		}
	}

	this->root->color = RBTreeNodeColor::BLACK;
}

void RBTree::Transplant(RBTreeNode* u, RBTreeNode* v)
{
	if (u->p == this->nil)
		this->root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;

	v->p = u->p;
}

bool RBTree::Delete(int key)
{
	RBTreeNode* z = this->Search(this->root, key);
	if (z == this->nil)
		return false;

	RBTreeNode *y = z;
	RBTreeNodeColor y_old_color = y->color;

	RBTreeNode *x = this->nil;

	if (z->left == this->nil && z->right == this->nil)
		this->Transplant(z, this->nil);
	else if (z->left == this->nil)
	{
		x = z->right;
		this->Transplant(z, z->right);
	}
	else if (z->right == this->nil)
	{
		x = z->left;
		this->Transplant(z, z->left);
	} 
	else
	{
		y = Minimum(z->right);
		y_old_color = y->color;
		x = y->right;

		if (y->p == z)
			x->p = y;
		else
		{
			Transplant(y, y->right);
			y->right = z->right;
			y->right->p = y;
		}

		Transplant(z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}

	if (y_old_color == RBTreeNodeColor::BLACK)
		DeleteFixup(x);

	return true;
}

void RBTree::DeleteFixup(RBTreeNode* x)
{
	while (x != this->root && x->color == RBTreeNodeColor::BLACK)
	{
		if (x == x->p->left)
		{
			RBTreeNode *w = x->p->right;
			if (w->color == RBTreeNodeColor::RED)
			{
				w->color = RBTreeNodeColor::BLACK;
				x->p->color = RBTreeNodeColor::RED;
				this->LeftRotate(x->p);
				w = x->p->right;
			}

			if (w->left->color == RBTreeNodeColor::BLACK && w->right->color == RBTreeNodeColor::BLACK)
			{
				w->color = RBTreeNodeColor::RED;
				x = x->p;
			}

			else
			{
				if (w->right->color == RBTreeNodeColor::BLACK)
				{
					w->left->color = RBTreeNodeColor::BLACK;
					w->color = RBTreeNodeColor::RED;
					this->RightRotate(w);
					w = x->p->right;
				}

				w->color = x->p->color;
				x->p->color = RBTreeNodeColor::BLACK;
				w->right->color = RBTreeNodeColor::BLACK;
				this->LeftRotate(x->p);
				x = this->root;
			}
		} 
		else
		{
			RBTreeNode *w = x->p->left;
			if (w->color == RBTreeNodeColor::RED)
			{
				w->color = RBTreeNodeColor::BLACK;
				x->p->color = RBTreeNodeColor::RED;
				this->RightRotate(x->p);
				w = x->p->left;
			}

			if (w->right->color == RBTreeNodeColor::BLACK && w->left->color == RBTreeNodeColor::BLACK)
			{
				w->color = RBTreeNodeColor::RED;
				x = x->p;
			}

			else
			{
				if (w->left->color == RBTreeNodeColor::BLACK)
				{
					w->right->color = RBTreeNodeColor::BLACK;
					w->color = RBTreeNodeColor::RED;
					this->LeftRotate(w);
					w = x->p->left;
				}

				w->color = x->p->color;
				x->p->color = RBTreeNodeColor::BLACK;
				w->left->color = RBTreeNodeColor::BLACK;
				this->RightRotate(x->p);
				x = this->root;
			}
		}
	}

	x->color = RBTreeNodeColor::BLACK;
}

RBTreeNode* RBTree::Minimum(RBTreeNode* root)
{
	if (root == this->nil)
		return this->nil;

	RBTreeNode *x = root;
	while (x->left != this->nil || x->right != this->nil)
	{
		if (x->left != this->nil)
			x = x->left;
		else if (x->right != this->nil)
			x = x->right;
	}

	return x;
}

void RBTree::Tranverse(RBTreeNode* root, TranverseOrder order)
{
	if (root == this->nil)
	{
		cout<<"Key: NIL | Color: BLACK (Leaf)\n";
		return;
	}

	char* color = "BLACK";
	if (root->color == 1)
		color = "RED";

	if (order == TranverseOrder::IN_ORDER)
	{
		Tranverse(root->left, order);
		cout<<"Key: "<<root->key<<" | Color: "<<color<<endl;
		Tranverse(root->right, order);
	}
	else if (order == TranverseOrder::PRE_ORDER)
	{
		cout<<"Key: "<<root->key<<" | Color: "<<color<<endl;
		Tranverse(root->left, order);
		Tranverse(root->right, order);
	}
	else
	{
		Tranverse(root->left, order);
		Tranverse(root->right, order);
		cout<<"Key: "<<root->key<<" | Color: "<<color<<endl;
	}
}

RBTreeNode* RBTree::Search(RBTreeNode* root, int target)
{
	if (root == this->nil)
		return this->nil;
	else if (root->key == target)
		return root;
	else if (root->key < target)
		return Search(root->right, target);
	else
		return Search(root->left, target);
}