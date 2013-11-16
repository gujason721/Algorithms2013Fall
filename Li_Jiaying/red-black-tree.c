#include<stdio.h>
#include<stdlib.h>


#define bool int
#define RED 0
#define BLACK 1


typedef struct _node node;
struct _node{
	int key;
	node* left;
	node* right;
	node* p;
	bool color;
};

struct _node NIL = {0, &NIL, &NIL, &NIL, BLACK};


typedef struct _rb{
	node* root;
} rb;



node* rb_search(node* x, int k)
{
	while ((x != &NIL) && (k != x->key)) {
		if (k < x->key) {
			x = x->left;
		} else {
			x = x->right;
		}
	}
	return x;
}

node* rb_min(node* x)
{
	while (x->left != &NIL) {
		x = x->left;
	}
	return x;
}

node* rb_max(node* x)
{
	while (x->right != &NIL) {
		x = x->right;
	}
	return x;
}



int left_rotate(rb* T, node* x)
{
	node* y;
	y = x->right;
	x->right = y->left;
	if (y->left != &NIL) {
		y->left->p = x;
	}
	y->p = x->p;
	if (x->p == &NIL) {
		T->root = y;
	} else if (x == x->p->left) {
		x->p->left = y;
	} else {
		x->p->right = y;
	}
	y->left = x;
	x->p = y;
	return 0;
}



int right_rotate(rb* T, node* x)
{
	node* y;
	y = x->left;
	x->left = y->right;
	if (y->right != &NIL) {
		y->right->p = x;
	}
	y->p = x->p;
	if (x->p == &NIL) {
		T->root = y;
	} else if (x == x->p->left) {
		x->p->left = y;
	} else {
		x->p->right = y;
	}
	y->right = x;
	x->p = y;
	return 0;
}



int rb_insert_fixup(rb* T, node* z)
{
	node* y;
	while (z->p->color == RED) {
		printf("In!\n");
		/**/
		if (z->p == z->p->p->left) {
			y = z->p->p->right;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			} else if (z == z->p->right) {
				z = z->p;
				left_rotate(T, z);
			}
			z->p->color = BLACK;
			z->p->p->color = RED;
			right_rotate(T, z->p->p);
		}
		else {
			y = z->p->p->left;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			} else if (z == z->p->left) {
				z = z->p;
				right_rotate(T, z);
			}
			z->p->color = BLACK;
			z->p->p->color = RED;
			left_rotate(T, z->p->p);
		}
	}
	T->root->color = BLACK;
	return 0;
}




int rb_insert(rb* T, node* z)
{
//printf("@");
	node* y = &NIL;
	node* x = T->root;
	while (x != &NIL) {
		y = x;
		if (z->key < x->key) {
			x = x->left;
		} else {
			x = x-> right;
		}
	}

//printf("@");

	z->p = y;
	if (y == &NIL) {
		T->root = z;
	} else if (z->key < y->key) {
		y->left = z;
	} else {
		y->right = z;
	}
printf("@");

	z->left = &NIL;
	z->right = &NIL;
	z->color = RED;
	rb_insert_fixup(T, z);

	return 0;
}



int rb_transplant(rb* T, node* u, node* v)
{
	if (u->p == &NIL) {
		T->root = v;
	} else if (u == u->p->left) {
		u->p->left = v;
	} else {
		u->p->right = v;
	}
	v->p = u->p;
	return 0;
}


int rb_delete_fixup(rb* T, node* x)
{
	node* w;
	while ((x != T->root) && (x->color == BLACK)) {
		if (x == x->p->left) {
			w = x->p->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				left_rotate(T, x->p);
				w = x->p->right;
			}
			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->p;
			} else if (w->right->color == BLACK) {
				w->left->color = BLACK;
				w->color = RED;
				right_rotate(T, w);
				w = x->p->right;
			}
			w->color = x->p->color;
			x->p->color = BLACK;
			w->right->color = BLACK;
			left_rotate(T, x->p);
			x = T->root;
		} else {
			w = x->p->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				right_rotate(T, x->p);
				w = x->p->left;
			}
			if ((w->right->color == BLACK) && (w->left->color == BLACK)) {
				w->color = RED;
				x = x->p;
			} else if (w->left->color == BLACK) {
				w->right->color = BLACK;
				w->color = RED;
				left_rotate(T, w);
				w = x->p->left;
			}
			w->color = x->p->color;
			x->p->color = BLACK;
			w->left->color = BLACK;
			right_rotate(T, x->p);
			x = T->root;
		}
	}
	x->color = BLACK;
	return 0;
}



int rb_delete(rb* T, node* z)
{
	node* y = z;
	node* x;
	bool y_original_color = y->color;
	if (z->left == &NIL) {
		x = z->left;
		rb_transplant(T, z, z->left);
	} else if (z->right == &NIL) {
		x = z->left;
		rb_transplant(T, z, z->right);
	} else {
		y = rb_min(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->p == z) {
			x->p = y;
		} else {
			rb_transplant(T, y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		rb_transplant(T, z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if (y_original_color == BLACK) {
		rb_delete_fixup(T, x);
	}
	return 0;
}





int inorder_tree_walk(node* x)
{
	if (x == &NIL) return 0;
	inorder_tree_walk(x->left);
	printf(" %d ", x->key);
	inorder_tree_walk(x->right);
	return 0;
}


node* makenode(int key)
{
	node* x = (node*)malloc(sizeof(node));
	x->key = key;
	return x;
}



int main()
{
	rb T;
	T.root = &NIL;
	printf("Tree Starts Building.\n");

	int i;
	scanf("%d", &i);
	while (i) {
		rb_insert(&T, makenode(i));
		scanf("%d", &i);
	}

	printf("Tree Built.\n");
	inorder_tree_walk(T.root);
	return 0;
}
