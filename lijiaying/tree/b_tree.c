#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAX 3



typedef struct BTNode {
	int keyMAX;
	struct BTNode * parent;
	int key[MAX+1];
	struct BTNode * ptr[MAX+1];
}BTNode,*BTree;

typedef struct  {
	BTNode * pt;
	int i;
	int tag;
}Result;



Result SearchBTree(BTree T,int k)
{
	Result result;
	BTree p,pre;
	int found = false;
	int i = 1;
	
	p = T;
	pre = NULL;
	while (p && !found)
	{
		for(i=1;i<p->keyMAX;i++)
		{
			if (k <= p->key[i])
				break;
		}
		if (k == p->key[i])
			found = true;
		else
		{
			pre = p;
			p = p->ptr[i-1];
		}
	}

	result.i = i-1;
	
	if(found)
		result.pt = p;
	else
		result.pt = pre;

	result.tag = found;
	return result;
}


int InsertBTree(BTree &T,int K, BTree q,int i)
{
	int j, s;
	int x;
	BTree ap, temp;
	bool finished = false;
	
	x = K;
	ap = NULL;
	while (q && !finished)
	{
		for (j = q->keyMAX; j > i; j --)
		{
			q->key[j + 1] = q->key[j];
			q->ptr[j + 1] = q->ptr[j];
		}
		q->key[i + 1] = x;
		q->ptr[i + 1] = ap;
		q->keyMAX ++;
		
		if (q->keyMAX < MAX)
			finished = true;
		else
		{
			s = MAX / 2 + 1;
			x = q->key[s];
			q->keyMAX = s - 1;
			ap = (BTree)malloc(sizeof(BTNode));
			assert(ap != NULL);
			ap->keyMAX = MAX - s;
			for ( j = 1; j <= MAX - s; j ++)
			{
				ap->key[j] = q->key[s + j];
				ap->ptr[j - 1] = q->ptr[s + j - 1];
				if (ap->ptr[j - 1]!= NULL)
					ap->ptr[j - 1]->parent = ap;
			}
			ap->ptr[MAX - s] = q->ptr[MAX];
			if (ap->ptr[MAX - s] != NULL)
				ap->ptr[j - 1]->parent = ap;
			
			q = q->parent;
			
			if (q)
			{
				for (j = 1; j <= q->keyMAX && x > q->key[j]; j ++)
					NULL;
				ap->parent = q;
				i = j - 1;
			}
		}
	}
	if (!finished)
	{
		temp = (BTree)malloc(sizeof(BTNode));
		assert(temp != NULL);
		temp->parent = NULL;
		temp->keyMAX = 1;
		temp->key[1] = x;
		temp->ptr[0] = T;
		temp->ptr[1] = ap;
		if (ap != NULL)
			ap->parent = temp;
		if (T != NULL)
			T->parent = temp;
		T = temp;
	}
	return true;
}

int CreateBTree(BTree &T, int size, int *key)
{
	Result result;
	int i;
	
	for (i = 0; i < size; i ++)
	{
		result = SearchBTree(T, key[i]);
		if (!result.tag)
			if (!InsertBTree(T, key[i],result.pt, result.i))
				return false;
	}
	return true;
}

void PrintBTree(BTree T)
{
	int front,rear, i, count;
	BTree Queue[1000], temp;
	
	front = rear = 0;
	Queue[rear ++] = T;
	while (front < rear)  
	{
		temp = Queue[front++];
		if (temp != NULL)
		{
			count = 0;
			for (i = 0; i <= temp->keyMAX; i ++)
				if (temp->ptr[i] != NULL)
					count ++;
				printf("[%d]", count);
				for ( i = 1; i <= temp->keyMAX; i ++)
					printf("%d ", temp->key[i]);
				printf("/n");
				for (i = 0; i <= temp->keyMAX; i ++)
					Queue[rear ++] = temp->ptr[i];
		}
	}
}

int main(void)
{
	BTree T = NULL;
	int key[15] = { 45, 24, 53, 90, 3, 12, 37, 50, 61, 70, 100, 30, 26, 85, 7};
	
	CreateBTree(T, 15, key);
	PrintBTree(T);
	return 1;
}
