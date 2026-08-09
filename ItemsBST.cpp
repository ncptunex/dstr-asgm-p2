#include "ItemsBST.hpp"
#include <format>

using namespace std;

int ItemsBST::getHeight(Item* item) {
	if (item == nullptr) return 0;
	return item->height;
}

int ItemsBST::getBalance(Item* item) {
	if (item == nullptr) return 0;
	return getHeight(item->left) - getHeight(item->right);
}

Item* ItemsBST::rotateRight(Item* item) {
	Item* root = item->left;
	Item* temp = root->right;

	root->right = item;
	item->left = temp;

	item->height = 1 + max(getHeight(item->left), getHeight(item->right));
	root->height = 1 + max(getHeight(root->left), getHeight(root->right));

	return root;
}

Item* ItemsBST::rotateLeft(Item* item) {
	Item* root = item->right;
	Item* temp = root->left;

	root->left = item;
	item->right = temp;

	item->height = 1 + max(getHeight(item->left), getHeight(item->right));
	root->height = 1 + max(getHeight(root->left), getHeight(root->right));

	return root;
}

// https://www.geeksforgeeks.org/dsa/introduction-to-avl-tree/
// https://www.geeksforgeeks.org/dsa/insertion-in-an-avl-tree/
Item* ItemsBST::balanceBST(Item* root) {
	if (root == nullptr) return nullptr;

	root->height = 1 + max(getHeight(root->left), getHeight(root->right));
	int balance = getBalance(root);

	if (balance > 1) {
		// left-left
		if (getBalance(root->left) >= 0) {
			return rotateRight(root);
		}
		// left-right
		if (getBalance(root->left) < 0) {
			root->left = rotateLeft(root->left);
			return rotateRight(root);
		}
	}

	if (balance < -1) {
		// right-right
		if (getBalance(root->right) <= 0) {
			return rotateLeft(root);
		}
		// right-left
		if (getBalance(root->right) > 0) {
			root->right = rotateRight(root->right);
			return rotateLeft(root);
		}
	}

	return root;
}

Item* ItemsBST::balanceBST(Item* root, Item* newItem) {
	if (root == nullptr) return newItem;

	if (newItem->itemId < root->itemId) {
		root->left = balanceBST(root->left, newItem);
	}
	else if (newItem->itemId > root->itemId) {
		root->right = balanceBST(root->right, newItem);
	}
	else {
		return root;
	}

	return balanceBST(root);
}

void ItemsBST::insertNode(Item* item) {
	if (item == nullptr) return;

	item->left = nullptr;
	item->right = nullptr;
	item->height = 1;

	root = balanceBST(root, item);
	size++;
}

void ItemsBST::deleteNode(Item* item) {
	if (item == nullptr) return;

	root = deleteNode(root, item->itemId);
}

Item* ItemsBST::deleteNode(Item* item, string key) {
	if (item == nullptr) return nullptr;

	if (key < item->itemId) {
		item->left = deleteNode(item->left, key);
	}
	else if (key > item->itemId) {
		item->right = deleteNode(item->right, key);
	}
	else {
		// branch with 0-1 leaf
		if (item->left == nullptr || item->right == nullptr) {
			Item* temp = item->left ? item->left : item->right;

			if (temp == nullptr) { // no leaf, delete branch
				temp = item;
				item = nullptr;
				delete temp;
			}
			else { // 1 leaf, replace branch with leaf
				Item* selectedItem = item;
				item = temp;
				delete selectedItem;
			}
			size--;
		}
		// branch with 2 leaf
		else {
			Item* temp = getRightmostItem(item->left);

			item->itemId = temp->itemId;
			item->itemName = temp->itemName;
			item->quantity = temp->quantity;
			item->categoryId = temp->categoryId;
			item->zone = temp->zone;
			item->aisle = temp->aisle;
			item->shelf = temp->shelf;

			item->left = deleteNode(item->left, temp->itemId);
		}

	}

	return balanceBST(item);
}

Item* ItemsBST::searchItemById(string key, bool doPrint) {
	return searchItemById(key, root, doPrint);
}

Item* ItemsBST::searchItemById(string key, Item* item, bool doPrint) {
	if (item == nullptr) {
		cout << "Item with ID " << key << " not found." << endl;
		return nullptr;
	}

	if (key == item->itemId) {
		if (doPrint) {
			cout << "--- SEARCH RESULT ---" << endl;
			cout << "Item ID" << ": " << item->itemId << endl;
			cout << "Item Name" << ": " << item->itemName << endl;
			cout << "Quantity" << ": " << item->quantity << endl;
			cout << "Category ID" << ": " << item->categoryId << endl;
			cout << "Warehouse Location" << ": Zone " << item->zone << ", Aisle " << item->aisle << ", Shelf " << item->shelf << endl << endl;
		}

		return item;
	}
	else if (key < item->itemId) {
		return searchItemById(key, item->left, doPrint);
	}
	else {
		return searchItemById(key, item->right, doPrint);
	}
}

Item* ItemsBST::searchItemByExactName(string key) {
	Item* search = searchItemByExactName(key, root);

	if (search == nullptr) {
		cout << "Item with name " << key << " not found." << endl;
	}

	return search;
}

Item* ItemsBST::searchItemByExactName(string key, Item* item) {
	if (item == nullptr) return nullptr;

	Item* left = searchItemByExactName(key, item->left);
	if (left != nullptr) return left;

	if (key == item->itemName) {
		cout << "--- SEARCH RESULT ---" << endl;
		cout << "Item ID" << ": " << item->itemId << endl;
		cout << "Item Name" << ": " << item->itemName << endl;
		cout << "Quantity" << ": " << item->quantity << endl;
		cout << "Category ID" << ": " << item->categoryId << endl;
		cout << "Warehouse Location" << ": Zone " << item->zone << ", Aisle " << item->aisle << ", Shelf " << item->shelf << endl << endl;

		return item;
	}

	return searchItemByExactName(key, item->right);
}

void ItemsBST::itemsPageTraversal(Item* item, int& count, int start, int end) {
	if (item == nullptr || count >= end) return;

	itemsPageTraversal(item->left, count, start, end);

	if (count >= start && count < end) {
		string location = "Zone " + item->zone + ", Aisle " + item->aisle + ", Shelf " + item->shelf;
		cout << format(" {:<8} | {:<25} | {:<8} | {:<18}\n",
			item->itemId,
			item->itemName,
			item->quantity,
			location);
	}

	count++;
	itemsPageTraversal(item->right, count, start, end);
}

void ItemsBST::displayAllItems() {
	if (root == nullptr) {
		cout << "No items to display." << endl;
		return;
	}

	const int ITEMS_PER_PAGE = 10;
	int currPage = 1;
	int totalPages = (size + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

	while (true)
	{
		cout << "------------------------------------------------------------------------------" << endl;
		cout << format(" {:<8} | {:<25} | {:<8} | {:<18}\n",
			"ITEM ID", "ITEM NAME", "QUANTITY", "WAREHOUSE LOCATION");
		cout << "------------------------------------------------------------------------------" << endl;

		int start = (currPage - 1) * ITEMS_PER_PAGE;
		int end = start + ITEMS_PER_PAGE;

		int count = 0;

		itemsPageTraversal(root, count, start, end);

		cout << "Now viewing Page " << currPage << " of " << totalPages << endl;
		if (currPage < totalPages) cout << "N. Next page" << endl;
		if (currPage > 1) cout << "P. Previous page" << endl;
		cout << "Q. Return to main menu" << endl;

		cout << "Enter selection: ";
		char action;
		cin >> action;
		action = toupper(action);

		if (action == 'N' && currPage < totalPages) {
			currPage++;
		}
		else if (action == 'P' && currPage > 1) {
			currPage--;
		}
		else if (action == 'Q') {
			cout << "Returning to main menu." << endl << endl;
			break;
		}
		else {
			cout << "Invalid selection. Please try again." << endl;
		}
	}
}

Item* ItemsBST::getRightmostItem(Item* item) {
	if (item == nullptr) return nullptr;

	while (item->right != nullptr) {
		item = item->right;
	}

	return item;
}

Item* ItemsBST::getRoot() {
	return root;
}

int ItemsBST::getSize() {
	return size;
}