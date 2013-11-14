#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include<ctype.h>

using namespace std;

typedef int key_t; // define the key
typedef int data_t; // define the data

typedef enum color_t
{
	RED = 0,
	BLACK = 1
}color_t;

typedef struct rb_node_t
{
	struct rb_node_t *left, *right, *parent;
	key_t key;
	data_t data;
	color_t color;
}rb_node_t;

void MidTraverse(struct rb_node_t * tree){
	if(!tree){
		MidTraverse(tree->left);
		printf("%d ", tree->key);
		MidTraverse(tree->right);
	}
}

// left rotation and right rotation
void left_rotation(struct rb_node_t **tree, struct rb_node_t *x){
	struct rb_node_t * y;
	y = x->right;
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;

	y->parent = x->parent;

	if(x->parent == NULL)
		*tree = y;

	if(x == x->parent->right){
		x->parent->right = y;
	}
	else{
		x->parent->left = y;
	}

	y->left = x;
	x->parent = y;
}

void right_rotation(struct rb_node_t **tree, struct rb_node_t *y){
	struct rb_node_t * x;
	x = y->left;
	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;

	if(y->parent == NULL)
		*tree = x;

	if(y == y->parent->right){
		y->parent->right = x;
	}
	else{
		y->parent->left = x;
	}

	x->right = y;
	y->parent = x;
}

struct rb_node_t * parent(struct rb_node_t * x){
	return x->parent;
};

void RB_insertion_fixup(struct rb_node_t **tree, struct rb_node_t *z){
	while(z->parent->color == RED){
		if(z->parent == z->parent->left){
			struct rb_node_t * ucl = z->parent->parent->right;
			if(ucl->color == RED){
				z->parent->color = BLACK;
				ucl->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else if(z==z->parent->right){
				z = z->parent;
				left_rotation(tree, z);
			}
			else{
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				right_rotation(tree, z->parent->parent);
			}
		}
		/*
		else{
			if(z->parent == z->parent->right){
				struct rb_node_t * ucl = z->parent->parent->left;
				if(ucl->color == RED){
					z->parent->color = BLACK;
					ucl->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else if(z==z->parent->left){
					z = z->parent;
					right_rotation(tree, z);
				}
				else{
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					left_rotation(tree, z->parent->parent);
				}
			}
		}
		*/
	}
	(*tree)->color = BLACK;
}

void RB_insert(struct rb_node_t **tree, key_t z){
	struct rb_node_t * x, * y;
	struct rb_node_t * kid = (struct rb_node_t *)malloc(sizeof(struct rb_node_t));
	kid->key = z;
	kid->left = NULL;
	kid->right = NULL;
	kid->color = RED;

	y = NULL;
	x = *tree;

	// make the insertion as the BST
	while(NULL != x){
		y = x;
		if (kid->key < (x->key))
			x = x->left;
		else
			x = x->right;
	}
	kid->parent = y;

	if (NULL == y){
		*tree = kid;
	}
	else if(kid->key < y->key){
		y->left = kid;
	}
	else
		y->right = kid;
	RB_insertion_fixup(tree, kid);
}


int main()
{
	int i, cont = 10;
	struct rb_node_t * tree = NULL;

	for(i=0; i < cont; i++){
		RB_insert(&tree, 3*i);
	}

	MidTraverse(tree);

	return 0;
}

