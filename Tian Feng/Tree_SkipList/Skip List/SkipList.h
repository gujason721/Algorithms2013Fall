#ifndef _SKIP_LIST_H
#define _SKIP_LIST_H

#include <cstdlib>
#include <iostream>
#include <ctime>
using namespace std;

#define HEAD_KEY -65534
#define NIL_KEY -65533

struct SkipListNode;
struct LevelNode;
class SkipList;

struct LevelNode
{
	LevelNode* p;
	SkipListNode* n;

	LevelNode(LevelNode* p, SkipListNode* n);
};

struct SkipListNode
{
	int key;
	int level;
	LevelNode** c;

	SkipListNode(int key, int level);
};

class SkipList
{
public:
	SkipListNode *head;
	SkipListNode *nil;
	int maxLevel;

	SkipList(int maxLevel);
	SkipListNode* Search(int key);
	SkipListNode* Insert(int key);
	bool Delete(int key);
	void Show();

private:
	int GetRandomLevel();
};

#endif
