#ifndef ITEMS_HPP
#define ITEMS_HPP

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Item {
	string itemId;
	string itemName;
	int quantity;
	string categoryId;
	string zone;
	string aisle;
	string shelf;

	Item* left = nullptr;
	Item* right = nullptr;

	int height = 1; // AVL BST

	Item(string id, string name, int qty, string cId, string z, string a, string s) {
		itemId = id;
		itemName = name;
		quantity = qty;
		categoryId = cId;
		zone = z;
		aisle = a;
		shelf = s;
	}
};

#endif
