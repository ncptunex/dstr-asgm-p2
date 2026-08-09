#ifndef ITEMS_BST_HPP
#define ITEMS_BST_HPP

#include "Items.hpp"

class ItemsBST {
private:
	Item* root = nullptr;

	Item* searchItemById(string key, Item* item, bool doPrint); // main

	Item* searchItemByExactName(string key, Item* item); // main

	Item* deleteNode(Item* item, string key); // main

	int getHeight(Item* item);

	int getBalance(Item* item);

	Item* rotateRight(Item* item);

	Item* rotateLeft(Item* item);

	Item* balanceBST(Item* root); // helper

	Item* balanceBST(Item* root, Item* newItem); // main

public:
	int size = 0;

	void insertNode(Item* item);

	void deleteNode(Item* item); // helper

	Item* searchItemById(string key, bool doPrint); // helper

	Item* searchItemByExactName(string key); // helper

	void itemsPageTraversal(Item* item, int& count, int start, int end);

	void displayAllItems();

	Item* getRightmostItem(Item* item);

	Item* getRoot();

	int getSize();
};

#endif
