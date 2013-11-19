#include <cstdlib>
#include <iostream>
using namespace std;

#include "RedBlackTree.h"
#include "BTree.h"
#include "SkipList.h"

int getRandomNumber(int lowerBound, int upperBound);

int main(int argc, char** argv)
{
 	srand((int)time(0));

	cout<<"Welcome to the app.\n";
	cout<<"===============================\n";

	int choice = 0;
	do 
	{
		cout<<"Please select one ADT to show:\nType 1 for Red Black Tree;\nType 2 for B-Tree; and\nType 3 for Skip List.\n";
		cin>>choice;
	} while (choice > 3 || choice < 1);

	cout<<"===============================\n";

	if (choice == 1)
	{
		int insertCount = 0;
		do 
		{
			cout<<"Please type the count of inserted integers (below 20 but above 0) for the red black tree:\n";
			cin>>insertCount;
		} while (insertCount > 20 || insertCount < 1);

		RBTree* pTree = new RBTree();
		int key = 0;
		for (int i = 0; i < insertCount; i++)
		{
			int temp = 0;
			do 
			{
				temp = rand() % 500 + 1;
			} while (key == temp);
			key = temp;
			pTree->Insert(key);
		}

		cout<<"\nBelow is the tree data:\n(in pre-order; each nil-leaf is shown)\n";
		pTree->Tranverse(pTree->root, TranverseOrder::PRE_ORDER);

		int insertee = 0;
		cout<<"\nPlease type an integer to be inserted:\n";
		cin>>insertee;
		if (pTree->Insert(insertee) == false)
			cout<<"\nThe number "<<insertee<<" cannot be inserted as it is in the tree.\n";
		else
		{
			cout<<"\nBelow is the tree data after insertion:\n(in pre-order; each nil-leaf is shown)\n";
			pTree->Tranverse(pTree->root, TranverseOrder::PRE_ORDER);
		}

		int deletee = 0;
		cout<<"\nPlease choose an integer, currently being in the tree, to be deleted:\n";
		cin>>deletee;
		if (pTree->Delete(deletee) == false)
			cout<<"\nThe number "<<deletee<<" cannot be deleted as it is not in the tree.\n";
		else
		{
			cout<<"\nBelow is the skip list structure after deletion:\n(in pre-order; each nil-leaf is shown)\n";
			pTree->Tranverse(pTree->root, TranverseOrder::PRE_ORDER);
		}
	} 
	else if (choice == 2)
	{
		int minDegree = 0;
		do 
		{
			cout<<"Please type the minimum degree (below 10 and at least 2) of the B-Tree:\n";
			cin>>minDegree;
		} while (minDegree > 10 || minDegree < 2);

		BTree* pTree = new BTree(minDegree);
		int key = 0;
		for (int i = 0; i < 100; i++)
		{
			pTree->Insert(i);
		}

		cout<<"\nBelow is the tree data:\n(in pre-order; each nil-leaf is shown)\n";
		pTree->Show(pTree->root);

		int insertee = 0;
		cout<<"\nPlease type an integer to be inserted:\n";
		cin>>insertee;
		if (pTree->Search(insertee))
			cout<<"\nThe number "<<insertee<<" cannot be inserted as it is in the tree.\n";
		else
		{
			pTree->Insert(insertee);
			cout<<"\nBelow is the tree data after insertion:\n(in pre-order; each nil-leaf is shown)\n";
			pTree->Show(pTree->root);
		}

		int deletee = 0;
		cout<<"\nPlease choose an integer, currently being in the tree, to be deleted:\n";
		cin>>deletee;
		if (!pTree->Search(insertee))
		{
			cout<<"\nThe number "<<deletee<<" cannot be deleted as it is not in the tree.\n";
		}
		else
		{
			pTree->Delete(deletee);
			cout<<"\nBelow is the skip list structure after deletion:\n(in pre-order; each nil-leaf is shown)\n";
			pTree->Show(pTree->root);
		}
	}
	else
	{
		int maxLevel = 0;
		do 
		{
			cout<<"Please type the level (below 10 but above 0) for the skip list:\n";
			cin>>maxLevel;
		} while (maxLevel > 9 || maxLevel < 1);

		SkipList* pList = new SkipList(maxLevel);
		int inputCount = rand() % 10 + 10;
		int key = 0;
		for (int i = 0; i < inputCount; i++)
		{
			int temp = 0;
			do 
			{
				temp = rand() % 500 + 1;
			} while (key == temp);
			key = temp;
			pList->Insert(key);
		}

		cout<<"\nBelow is the skip list structure:\n";
		pList->Show();

		int insertee = 0;
		cout<<"\nPlease type an integer to be inserted:\n";
		cin>>insertee;
		if (pList->Insert(insertee) == pList->nil)
			cout<<"\nThe number "<<insertee<<" cannot be inserted as it is in the list.\n";
		else
		{
			cout<<"\nBelow is the skip list structure after insertion:\n";
			pList->Show();
		}

		int deletee = 0;
		cout<<"\nPlease choose an integer, currently being in the list, to be deleted:\n";
		cin>>deletee;
		if (pList->Delete(deletee) == false)
			cout<<"\nThe number "<<deletee<<" cannot be deleted as it is not in the list.\n";
		else
		{
			cout<<"\nBelow is the skip list structure after deletion:\n";
			pList->Show();
		}
	}

	cout<<"===============================\n";
	cout<<"Thanks for your testing~:)\n";
	
	system("pause");
	return 0;
}