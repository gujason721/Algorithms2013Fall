#include "SkipList.h"

LevelNode::LevelNode(LevelNode* p, SkipListNode* n)
{
	this->p = p;
	this->n = n;
}

SkipListNode::SkipListNode(int key, int level)
{
	this->level = level;
	this->key = key;
	this->c = new LevelNode*[this->level]; 
}

SkipList::SkipList(int maxLevel)
{
	this->maxLevel = maxLevel;
	this->head = new SkipListNode(HEAD_KEY, this->maxLevel);
	this->nil = new SkipListNode(NIL_KEY, this->maxLevel);

	for (int i = 0; i < this->maxLevel; i++) 
	{
		this->nil->c[i] = new LevelNode(NULL, this->nil);
		this->head->c[i] = new LevelNode(this->nil->c[i], this->head);
	}
}

SkipListNode* SkipList::Insert(int key)
{
	if (this->Search(key) != this->nil) 
		return this->nil;

	SkipListNode* t = this->head;
	SkipListNode* d = new SkipListNode(key, this->GetRandomLevel());
	for (int i = d->level - 1; i >= 0; i--) 
	{
		while (t->c[i]->p->n->key < key && t->c[i]->p->n != this->nil) 
			t = t->c[i]->p->n;

		d->c[i] = new LevelNode(t->c[i]->p, d);
		t->c[i]->p = d->c[i];
	}

	return d;
}

SkipListNode* SkipList::Search(int key)
{
	SkipListNode* t = this->head;
	for (int i = this->maxLevel - 1; i >= 0; i--)
	{
		while (t->c[i]->p->n->key < key && t->c[i]->p->n != this->nil)
			t = t->c[i]->p->n;

		if (key == t->c[i]->p->n->key)
			return t->c[i]->p->n;
		else
			continue;
	}

	return this->nil;
}

bool SkipList::Delete(int key)
{
	SkipListNode* d = this->Search(key);
	if (d == this->nil) 
		return false;

	SkipListNode* t = this->head;
	for (int i = d->level - 1; i >= 0; i--)
	{
		while (t->c[i]->p->n->key < d->key && t->c[i]->p->n != this->nil)
			t = t->c[i]->p->n;

		if (t->c[i]->p->n->key == key)
			t->c[i]->p = d->c[i]->p;
		else
			continue;
	}

	delete d;
	return true;
}

void SkipList::Show()
{
	SkipListNode* t = this->head;
	for (int i = this->maxLevel - 1; i >= 0; i--)
	{
		cout<<"Level "<<i + 1<<" : { ";
		while (t->c[i]->p->n != this->nil)
		{
			t = t->c[i]->p->n;
			cout<<t->c[i]->n->key<<" ";
		}
		cout<<"}"<<endl;
		t = this->head;
	}
}

int SkipList::GetRandomLevel()
{
	return rand() % this->maxLevel + 1;
}
