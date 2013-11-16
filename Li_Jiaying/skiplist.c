#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int g_level = 1;

typedef struct _node {
	int key;
	struct _node* right;	//next_pointer
	struct _node* down;	//bottom_pointer
} node;

typedef struct _skiplist {
	node* top;
} skiplist;


node* sl_search(skiplist* S, int x)
{
	node* p = S->top;
	while (1) {
		while (p->right->key < x) {
			p = p->right;
		}
		if (p->right->key == x) {
			return p->right;
		}
		if (p->down == NULL) {
			return p;
		} else {
			p = p->down;
		}
	}
}


int get_level()
{
	int k = 1;
	srand((int)time(0)); 
	while (rand()%2) {
		k++;
		if (k > g_level) {
			return g_level + 1;
		}
	}
	return k;
}


int sl_insert(skiplist* S, int k)
{
	int level = get_level();
	node* new_nodes = (node*)malloc((level + 1) * sizeof(node));

	if (level > g_level) {
		g_level++;
		node* nodes0 = (node*)malloc(sizeof(node));
		node* nodes1 = (node*)malloc(sizeof(node));
		nodes0->key = -1;
		nodes0->right = nodes1;
		nodes0->down = S->top;
		node* p = S->top;
		while(p->right != NULL) {
			p = p->right;
		}
		nodes1->key = p->key;
		nodes1->right = NULL;
		nodes1->down = p;
		S->top = nodes0;
	}
	int i = g_level;
	node* p = S->top;
	while (i > level) {
		while (p->right->key < k) {
			p = p->right;
		}
		p = p->down;
		i--;
	}
	while(i > 0) {
		while (p->right->key < k) {
			p = p->right;
		}
		new_nodes[i].right = p->right;
		new_nodes[i].key = k;
		p->right = &new_nodes[i];
		p = p->down;
		i--;
	}
	for (i = level; i > 1; i--) {
		new_nodes[i].down = &new_nodes[i-1];
	}
	new_nodes[1].down = NULL;
	return 0;
}


int sl_delete(skiplist* S, int k)
{
	node* p = sl_search(S, k);
	if(p->key != k) return 0;
	int level = 1;
	node* q = p;
	while (q->down != NULL) {
		q = q->down;
		level++;
	}
	node* temp = S->top;
	int i = g_level;
	while (i++ > level) {
		temp = temp->down;
	}
	node* pre_p = temp;
	
	temp = S->top;
	while (1) {
		while (temp->right->key < k) {
			temp = temp->right;
		}
		if (temp->right == p) {
			break;
		}
		temp = temp->down;
	}
	temp->right = p->right;
	while (temp->down != NULL) {
		temp = temp->down;
		while (temp->right->key < k) {
			temp = temp->right;
		}
		temp->right = temp->right->right;
	}

	int need_shrink = 0;
	temp = pre_p->down;
	node* r = pre_p;
	while (r->key == temp->key) {
		if ((r->right == NULL) && (temp->right == NULL)) {
			need_shrink = 1;
			break;
		}
		if ((r->right == NULL) || (temp->right == NULL)) {
			break;
		}
		r = r->right;
		temp = temp->right;
	}
	temp = pre_p->down;
	while (pre_p != NULL) {
		pre_p->down = temp->down;
		pre_p = pre_p->right;
		temp = temp->right;
	}
	return 0;
}


void sl_print(skiplist* S)
{
	node* s = S->top;
	int i = g_level;
	while (i > 0) {
		node* p = s;
		printf("Level %d:\t", i);
		while (p != NULL) {
			printf("%2d->", p->key);
			p = p->right;
		}
		printf("\n");
		s = s->down;
		i--;
	}
	printf("\n");
}



int main()
{
	skiplist* S = (skiplist*)malloc(sizeof(skiplist));
	node nodes[2] = {
		{-1, &nodes[1], NULL},
		{1000000, NULL, NULL}
	};
	S->top = &nodes[0];
	
	int i;
	printf("Start...\n");
	scanf("%d", &i);
	while (i != -1) {
		sl_insert(S, i);
		sl_print(S);
		scanf("%d", &i);
	}
	delete(S);
	printf("End of program!\n");
	return 0;
}

