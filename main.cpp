#include <iostream>
#include <iomanip>
#include <string>
#include <format>

// task 1
#include "OrderManagement.hpp"

// task 2
#include "Robots.hpp"
#include "CircularQueueT2.hpp"

// task 3
#include "NavigationStack.hpp"

// task 4
#include "ItemsBST.hpp"

// task 5
#include "WarehouseNav.hpp"

using namespace std;

// TASK 1
void clearInput() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printMenu() {
	cout << "\n  +-------------------------------------------+\n";
	cout << "  |   WAREHOUSE ORDER MANAGEMENT SYSTEM      |\n";
	cout << "  +-------------------------------------------+\n";
	cout << "  |  1. Add New Order                        |\n";
	cout << "  |  2. Process Next Order (assign to robot) |\n";
	cout << "  |  3. Complete Current Order               |\n";
	cout << "  |  4. Peek at Next Order                   |\n";
	cout << "  |  5. Display Pending Orders               |\n";
	cout << "  |  6. Display Completed Orders             |\n";
	cout << "  |  7. Display Summary                      |\n";
	cout << "  |  0. Exit                                 |\n";
	cout << "  +-------------------------------------------+\n";
	cout << "  Enter choice: ";
}

string selectPriority() {
	int p;
	cout << "  Priority (1=High, 2=Medium, 3=Low): ";
	while (!(cin >> p) || p < 1 || p > 3) {
		clearInput();
		cout << "  Invalid. Enter 1, 2, or 3: ";
	}
	clearInput();
	if (p == 1) return "High";
	if (p == 2) return "Medium";
	return "Low";
}

void task1_menu() {
	OrderManagementSystem oms(50);



	Order currentOrder;
	bool  hasCurrentOrder = false;
	int   choice = -1;

	while (choice != 0) {
		printMenu();

		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "  [ERROR] Please enter a valid number.\n";
			continue;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice) {

		case 1: {
			string name, item, robot;
			cout << "  Customer name  : "; getline(cin, name);
			cout << "  Item ID (e.g. I001): "; getline(cin, item);
			string prio = selectPriority();
			oms.addOrder(name, item, prio);
			break;
		}

		case 2: {
			if (!hasCurrentOrder) {
				cout << "  [INFO] No order is currently being processed.\n";
				break;
			}
			oms.completeOrder(currentOrder);
			hasCurrentOrder = false;
			break;
		}

		case 3: {
			if (!hasCurrentOrder) {
				cout << "  [INFO] No order is currently being processed.\n";
				break;
			}
			oms.completeOrder(currentOrder);
			hasCurrentOrder = false;
			break;
		}

		case 4: {
			oms.peekNextOrder();
			break;
		}

		case 5: {
			oms.displayPendingOrders();
			break;
		}

		case 6: {
			oms.displayCompletedOrders();
			break;
		}
		
		case 7: {
			oms.displaySummary();
			break;
		}

		case 0: {
			cout << "\n  [BYE] Shutting down Order Management System.\n\n";
			break;
		}

		default: {
			cout << "  [ERROR] Invalid option. Choose 0-7.\n";
			break;
		}
		}
	}
}



//TASK 2 CHAN JIE LIN
//arrays
Robots robotList[20];	//store all robots
Orders orderList[20];	//store all orders
int countRobot = 0;		//count how many robots loaded
int countOrder = 0;		//count how many orders loaded

CircularQueueT2 robotWheel;
AssignmentsList assignmentLog;  //linked list for tracking

// task 2 load data from csv 
void loadRobotsFromCSV() {
	countRobot = 0;
	ifstream file("robots.csv");
	string line;
	getline(file, line); //skip header when retrieve

	while (getline(file, line)) {
		stringstream ss(line);

		getline(ss, robotList[countRobot].robotID, ',');
		getline(ss, robotList[countRobot].robotName, ',');
		getline(ss, robotList[countRobot].status, ',');
		getline(ss, robotList[countRobot].currentTask, ',');

		string total;
		getline(ss, total, ',');
		robotList[countRobot].totalTasks = stoi(total);  //change from string to int
		getline(ss, robotList[countRobot].maintenance, ',');
		countRobot++;
	}
	file.close();
	cout << "All " << countRobot << " robots have loaded successfully." << endl;
}

void loadOrdersFromCSV() {
	countOrder = 0;
	ifstream file("orders.csv");
	string line;
	getline(file, line);

	while (getline(file, line)) {
		stringstream ss(line);

		getline(ss, orderList[countOrder].orderID, ',');
		getline(ss, orderList[countOrder].customerName, ',');
		getline(ss, orderList[countOrder].itemID, ',');
		getline(ss, orderList[countOrder].orderStatus, ',');
		getline(ss, orderList[countOrder].assignedRobotID, ',');
		getline(ss, orderList[countOrder].priority, ',');
		getline(ss, orderList[countOrder].orderTime, ',');

		countOrder++;
	}
	file.close();
	cout << "All " << countOrder << " orders have loaded successfully." << endl;
}

// task 2 save all to csv files, not just asgm only (robot & orders)
void saveAllToCSV() {
	ofstream robotFile("robots.csv");
	robotFile << "robotID,robotName,status,currentTask,totalTasks,maintenance" << endl;
	for (int i = 0; i < countRobot; i++) {
		robotFile << robotList[i].robotID << ","
			<< robotList[i].robotName << ","
			<< robotList[i].status << ","
			<< robotList[i].currentTask << ","
			<< robotList[i].totalTasks << ","
			<< robotList[i].maintenance << endl;
	}
	robotFile.close();

	ofstream orderFile("orders.csv");
	orderFile << "orderID,customerName,itemID,orderStatus,assignedRobotID,priority,orderTime" << endl;
	for (int i = 0; i < countOrder; i++) {
		orderFile << orderList[i].orderID << ","
			<< orderList[i].customerName << ","
			<< orderList[i].itemID << ","
			<< orderList[i].orderStatus << ","
			<< orderList[i].assignedRobotID << ","
			<< orderList[i].priority << ","
			<< orderList[i].orderTime << endl;
	}
	orderFile.close();

	cout << "Save all the data successfully." << endl;
}

void buildRobotWheel() {
	cout << "========== Building Robot Assignment Wheel (T2) ==========" << endl;

	for (int i = 0; i < countRobot; i++) {
		//only add robots that are available only, X maintenance
		if (robotList[i].status == "Available" && robotList[i].maintenance == "No") {
			robotWheel.enqueue(robotList[i].robotID);

			cout << "Added: " << robotList[i].robotID << " (" << robotList[i].robotName << ") " << endl;
		}
		else {
			cout << "Skipped: " << robotList[i].robotID << " (" << robotList[i].robotName << ") " << endl;
			if (robotList[i].maintenance == "Yes")
				cout << "- The robot is under maintenance." << endl;
			else
				cout << "- The robot is busy now." << endl;
		}
	}
	cout << " " << endl;
}

void showRobots() {
	cout << " " << endl;
	cout << "================== 1. VIEW ALL ROBOTS (T2) ==================" << endl;
	cout << "ID | Name | Status | Task | Total | Maintenance" << endl;
	cout << "=============================================================" << endl;

	for (int i = 0; i < countRobot; i++) {
		cout << robotList[i].robotID << " | " << robotList[i].robotName << " | " << robotList[i].status << " | " << (robotList[i].currentTask == "NULL" ? "-" : robotList[i].currentTask) << " | " << robotList[i].totalTasks << " | " << robotList[i].maintenance << endl;
	}
}

void showOrders() {
	cout << " " << endl;
	cout << "========== 2. VIEW ALL ORDERS (T2) ==========" << endl;
	cout << "ID | Customer | Item | Status | Robot | Priority | Time" << endl;
	cout << "=============================================" << endl;

	for (int i = 0; i < countOrder; i++) {
		cout << orderList[i].orderID << " | " << orderList[i].customerName << " | " << orderList[i].itemID << " | " << orderList[i].orderStatus << " | " << (orderList[i].assignedRobotID == "NULL" ? "-" : orderList[i].assignedRobotID) << " | " << orderList[i].priority << " | " << orderList[i].orderTime << endl;
	}
}

void showPendingOrders() {
	cout << " " << endl;
	cout << "========== 3. PENDING ORDERS (T2) ==========" << endl;
	cout << "ID | Customer | Item | Priority | Time" << endl;
	cout << "============================================" << endl;

	bool found = false;
	for (int i = 0; i < countOrder; i++) {
		if (orderList[i].orderStatus == "Pending") {
			cout << orderList[i].orderID << " | " << orderList[i].customerName << " | " << orderList[i].itemID << " | " << orderList[i].priority << " | " << orderList[i].orderTime << endl;
			found = true;
		}
	}

	if (!found) cout << "There is no pending orders!" << endl;
}

// task 2 assign robot to the order
void assignRobot() {
	cout << " " << endl;
	cout << "========== 5. ASSIGNING ROBOT TO ORDERS (T2) ==========" << endl;

	//find first pending order
	int orderIndex = -1;
	for (int i = 0; i < countOrder; i++) {
		if (orderList[i].orderStatus == "Pending") {
			orderIndex = i;
			break;
		}
	}

	if (orderIndex == -1) {
		cout << "There is no pending orders!" << endl;
		return;
	}

	cout << "The next pending orders: " << orderList[orderIndex].orderID << endl;

	//check if wheel has robots or not
	if (robotWheel.isEmpty()) {
		cout << "There are no robots available in the wheels." << endl;
		return;
	}

	//display currrent wheel
	robotWheel.display();

	//get next robot from wheel 
	string robotID = robotWheel.dequeue();

	if (robotID == "") {
		cout << "Fail to get robot." << endl;
		return;
	}

	//search for robot name
	string robotName = "";
	for (int i = 0; i < countRobot; i++) {
		if (robotList[i].robotID == robotID) {
			robotName = robotList[i].robotName;
			break;
		}
	}

	cout << " " << endl;
	cout << "===== ASSIGNED ROBOT =====" << endl;
	cout << "Order: " << orderList[orderIndex].orderID << " (" << orderList[orderIndex].customerName << ") " << endl;
	cout << "Robot: " << robotID << " (" << robotName << ") " << endl;

	//update order
	orderList[orderIndex].assignedRobotID = robotID;
	orderList[orderIndex].orderStatus = "In_Progress";

	//update robot
	for (int i = 0; i < countRobot; i++) {
		if (robotList[i].robotID == robotID) {
			robotList[i].status = "Busy";
			robotList[i].currentTask = orderList[orderIndex].orderID;
			robotList[i].totalTasks++;
			break;
		}
	}

	//record in asgm log
	assignmentLog.addAssignment(orderList[orderIndex].orderID, robotID);
	saveAllToCSV();
	cout << "The assignment is complete!" << endl;
	cout << "Robot " << robotID << " removed from wheel because status change to Busy." << endl;

}

void robotCompleteTask() {
	cout << " " << endl;
	cout << "===== 6.ROBOT COMPLETE TASK AND RETURN TO WHEEL (T2) =====" << endl;

	//display all busy robots
	cout << "Busy robots: " << endl;
	bool isBusy = false;
	for (int i = 0; i < countRobot; i++) {
		if (robotList[i].status == "Busy") {
			cout << robotList[i].robotID << " (" << robotList[i].robotName << ") " << " - Task:  " << robotList[i].currentTask << endl;
			isBusy = true;
		}
	}

	if (!isBusy) {
		cout << "No busy robots to complete." << endl;
		return;
	}

	//ask which robot has completed
	cout << "Enter RobotID that completed their tasks: ";
	string robotID;
	cin >> robotID;

	//find & update the robot
	bool found = false;
	for (int i = 0; i < countRobot; i++) {
		if (robotList[i].robotID == robotID && robotList[i].status == "Busy") {
			//update order to completed 
			for (int j = 0; j < countOrder; j++) {
				if (orderList[j].orderID == robotList[i].currentTask) {
					orderList[j].orderStatus = "Completed";
					break;
				}
			}

			//update robot back to available
			robotList[i].status = "Available";
			robotList[i].currentTask = "NULL";

			//add back to circular queue
			robotWheel.enqueue(robotList[i].robotID);
			saveAllToCSV();
			cout << "=====ROBOT TASK COMPLETED (T2)=====" << endl;
			cout << "Robot " << robotID << " is now Available and rejoined the wheel. " << endl;

			found = true;
			break;
		}
	}
	if (!found) {
		cout << "Robot is busy." << endl;
	}
}

void showMenu() {
	cout << " " << endl;
	cout << "========================================" << endl;
	cout << "  ROBOT ASSIGNMENT MODULE" << endl;
	cout << "  TASK 2 - CIRCULAR QUEUE SYSTEM" << endl;
	cout << "========================================" << endl;
	cout << "1. View All Robots" << endl;
	cout << "2. View All Orders" << endl;
	cout << "3. View Pending Orders" << endl;
	cout << "4. View Robot Wheel" << endl;
	cout << "5. Assign Next Robot to Order" << endl;
	cout << "6. Complete Task (Return Robot to Wheel)" << endl;
	cout << "7. View Assignment History" << endl;
	cout << "8. Save Assignments to File" << endl;
	cout << "9. Save All Data to File (robots & assignments csv file)" << endl;
	cout << "10. Exit" << endl;
	cout << "========================================" << endl;
	cout << "Please type your choice based on the number display above: ";
}

void task2_menu() {
	cout << "=========================================================" << endl;
	cout << "WAREHOUSE ROBOT NAVIGATION SYSTEM (TP086244 CHAN JIE LIN)" << endl;
	cout << "Task 2: Robot Assignment Module" << endl;
	cout << "=========================================================" << endl;
	cout << "Data Structure: Circular Queue" << endl;
	cout << "To ensure fair rotation of robot tasks" << endl;
	cout << "=========================================================" << endl << endl;

	//load data
	loadRobotsFromCSV();
	loadOrdersFromCSV();

	//build wheel w available robots only
	buildRobotWheel();

	int choice;
	do {
		showMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			showRobots();
			break;
		case 2:
			showOrders();
			break;
		case 3:
			showPendingOrders();
			break;
		case 4:
			robotWheel.display();
			break;
		case 5: assignRobot();
			break;
		case 6:
			robotCompleteTask();
			break;
		case 7:
			assignmentLog.display();
			break;
		case 8:
			assignmentLog.saveToCSV("assignments.csv");
			break;
		case 9:
			saveAllToCSV();
			break;
		case 10:
			cout << " " << endl;
			cout << "Close Task 2." << endl;
			break;
		default:
			cout << "Invalid choice." << endl;
		}

		if (choice != 10) {
			cout << " " << endl;
			cout << "Press Enter to continue the next one.";
			cin.ignore();
			cin.get();
			cout << " " << endl;
		}
	} while (choice != 10);
}



// TASK 3
void task3_menu()
{


	//Random initiation
	srand(time(0));
	// Create navigation stack object
	NavigationStack robotPath;

	robotPath.executeAssignedRobot();
}



// TASK 4
void loadItemsFromCSV(ItemsBST& bst) {
	ifstream file("items.csv");
	string line;
	getline(file, line); //skip header

	while (getline(file, line)) {
		stringstream ss(line);
		string id, name, qty, cId, z, a, s;

		getline(ss, id, ',');
		getline(ss, name, ',');
		getline(ss, qty, ',');
		getline(ss, cId, ',');
		getline(ss, z, ',');
		getline(ss, a, ',');
		getline(ss, s, ',');

		int quantity = stoi(qty);  //change from string to int

		Item* item = new Item(id, name, quantity, cId, z, a, s);
		bst.insertNode(item);
	}
	file.close();
}

void appendItemToCSV(Item* item) {
	ofstream file("items.csv", ios::app); // open in append mode

	file << item->itemId << ","
		<< item->itemName << ","
		<< item->quantity << ","
		<< item->categoryId << ","
		<< item->zone << ","
		<< item->aisle << ","
		<< item->shelf << "\n";

	file.close();
}

bool categoryExists(string selection) {
	ifstream file("categories.csv");
	string line;
	getline(file, line); //skip header

	while (getline(file, line)) {
		stringstream ss(line);
		string id, name, desc;

		if (getline(ss, id, ',')) {
			if (id == selection) {
				file.close();
				return true;
			}
		};
	}
	file.close();
	return false;
}

void displaySearchMenu(ItemsBST& bst) {
	const int SEARCH_MENU_OPTIONS = 2;
	int searchType = -1;

	// search menu
	while (searchType != 0) {
		cout << "--- SEARCH MENU INTERFACE ---" << endl;
		cout << "1. Item ID" << endl;
		cout << "2. Exact Name" << endl;
		cout << "0. Go Back" << endl;
		cout << "Enter selection (0-" << SEARCH_MENU_OPTIONS << "): ";
		cin >> searchType;

		switch (searchType) {
		case 0:
			cout << "Returning to main menu." << endl << endl;
			break;
		case 1: { // id search
			int id;
			cout << "Enter item ID: I";
			cin >> id;

			string selectedId = "I" + format("{:03}", id); // format item ID to match CSV format

			bst.searchItemById(selectedId, true);
			break;
		}
		case 2: { // name search
			string name;
			cout << "Enter exact item name: ";
			cin >> name;

			bst.searchItemByExactName(name);
			break;
		}
		default:
			cout << "Please enter a value between 0 and " << SEARCH_MENU_OPTIONS << "." << endl << endl;
			break;
		}
	}
	return;
}

void insertNewItem(ItemsBST& bst) {
	string newId = "I001";
	Item* rightmost = bst.getRightmostItem(bst.getRoot());

	const int PREFIX_LENGTH = 1;

	if (rightmost != nullptr) {
		newId = rightmost->itemId;
		int num = stoi(newId.substr(PREFIX_LENGTH)); // remove prefix & get id number

		num++;
		newId = "I" + format("{:03}", num);
	}

	string name, cId, z, a, s;
	int qty = 0;
	const int PADDING = 21;

	cout << left << setw(PADDING) << "Enter Item Name" << ": ";
	cin >> name;

	while (true) {
		cout << left << setw(PADDING) << "Enter Category ID" << ": C";
		cin >> cId;

		cId = "C" + format("{:03}", stoi(cId)); // format category ID to match CSV format

		if (categoryExists(cId)) {
			break;
		}
		else {
			cout << "Category ID not found. Please enter a valid Category ID." << endl;
		}
	}

	cout << left << setw(PADDING) << "Enter Zone" << ": Z";
	cin >> z;
	z = "Z" + z;

	cout << left << setw(PADDING) << "Enter Aisle" << ": A";
	cin >> a;
	a = "A" + a;

	cout << left << setw(PADDING) << "Enter Shelf" << ": S";
	cin >> s;
	s = "S" + s;

	while (true) {
		cout << endl << "--- CHECK NEW ITEM DETAILS ---" << endl;
		cout << left << setw(PADDING) << "Item ID" << ": " << newId << endl;
		cout << left << setw(PADDING) << "Name" << ": " << name << endl;
		cout << left << setw(PADDING) << "Category" << ": " << cId << endl;
		cout << left << setw(PADDING) << "Warehouse Location" << ": Zone " << z << ", Aisle " << a << ", Shelf " << s << endl;

		cout << "Confirm insert new item? (Y/N): ";
		char choice;
		cin >> choice;

		Item* newItem = new Item(newId, name, qty, cId, z, a, s);

		switch (choice) {
		case 'Y': case 'y':
			bst.insertNode(newItem);
			appendItemToCSV(newItem);

			cout << "New item inserted successfully." << endl << endl;
			break;

		case 'N': case 'n':
			cout << "Cancelled insert new item." << endl << endl;
			break;

		default:
			cout << "Invalid input. Please enter Y or N." << endl;
			continue;
		}
		break;
	}
}

void updateItemName(Item* item) {
	string name;
	cout << "Enter new item name (type . to discard): ";
	cin >> name;

	if (name != ".") {
		item->itemName = name;
	}
	else {
		cout << "Discarded changes." << endl << endl;
	}
}

void updateItemQuantity(Item* item) {
	int qty;
	cout << "Enter new quantity (type -1 to discard): ";
	cin >> qty;

	if (qty == -1) {
		cout << "Discarded changes." << endl << endl;
		return;
	}

	if (qty >= 0) {
		item->quantity = qty;
	}
	else {
		cout << "Quantity cannot be negative." << endl << endl;
	}
}

void updateItemCategory(Item* item) {
	string cId;

	while (true) {
		cout << "Enter new category ID (type . to discard): C";
		cin >> cId;

		if (cId == ".") {
			cout << "Discarded changes." << endl << endl;
			return;
		}

		cId = "C" + format("{:03}", stoi(cId)); // format category ID to match CSV format
		if (categoryExists(cId)) {
			item->categoryId = cId;
			return;
		}
		else {
			cout << "Category ID not found. Please enter a valid Category ID." << endl;
		}
	}
}

void updateWarehouseLocation(Item* item, string area) {
	string idx;
	char prefix = area[0];

	while (true) {
		cout << "Enter new " << area << " (type . to discard): " << prefix;
		cin >> idx;

		if (idx == ".") {
			cout << "Discarded changes." << endl << endl;
			return;
		}

		if (stoi(idx) <= 0) {
			cout << area << " number cannot be must be higher than 0." << endl << endl;
			continue;
		}

		switch (prefix) {
		case 'Z':
			item->zone = prefix + idx;
			break;
		case 'A':
			item->aisle = prefix + idx;
			break;
		case 'S':
			item->shelf = prefix + idx;
			break;
		}
		break;
	}
}

void pushUpdateItem(Item* item) {
	if (item == nullptr) return;

	ofstream file("items.csv", ios::app);
	pushUpdateItem(item->left);

	file << item->itemId << ","
		<< item->itemName << ","
		<< item->quantity << ","
		<< item->categoryId << ","
		<< item->zone << ","
		<< item->aisle << ","
		<< item->shelf << "\n";

	file.close();
	pushUpdateItem(item->right);
}

void pushUpdateItem(Item* item, Item* temp, Item* root) {
	if (item == nullptr) return;

	while (true) {
		cout << "Confirm update item details? (Y/N): ";
		char choice;
		cin >> choice;

		switch (choice) {
		case 'Y': case 'y': {
			// update tree
			item->itemName = temp->itemName;
			item->quantity = temp->quantity;
			item->categoryId = temp->categoryId;
			item->zone = temp->zone;
			item->aisle = temp->aisle;
			item->shelf = temp->shelf;

			ofstream file("items.csv", ios::trunc);
			file << "itemID,itemName,quantity,categoryID,zone,aisle,shelf\n";
			file.close();

			pushUpdateItem(root);

			cout << "Item " << item->itemId << "updated successfully." << endl << endl;
			break;
		}

		case 'N': case 'n':
			cout << "Changes discarded." << endl << endl;
			break;

		default:
			cout << "Invalid input. Please enter Y or N." << endl << endl;
			continue;
		}
	}
}

void displayUpdateMenu(Item* item, Item* root) {
	const int UPDATE_MENU_OPTIONS = 7;
	const int PADDING = 30;
	int choice = -1;

	Item* temp = new Item(
		item->itemId,
		item->itemName,
		item->quantity,
		item->categoryId,
		item->zone,
		item->aisle,
		item->shelf
	);

	while (true) {
		cout << "--- UPDATE ITEMS INTERFACE ---" << endl;
		cout << "Currently updating: " << item->itemId << endl;
		cout << left << setw(PADDING) << "1. Update Item Name" << ": " << temp->itemName << endl;
		cout << left << setw(PADDING) << "2. Update Quantity" << ": " << temp->quantity << endl;
		cout << left << setw(PADDING) << "3. Update Category" << ": " << temp->categoryId << endl;
		cout << left << setw(PADDING) << "4. Update Zone" << ": " << temp->zone << endl;
		cout << left << setw(PADDING) << "5. Update Aisle" << ": " << temp->aisle << endl;
		cout << left << setw(PADDING) << "6. Update Shelf" << ": " << temp->shelf << endl;
		cout << left << setw(PADDING) << "7. Save Changes" << endl;
		cout << "0. Go Back" << endl;

		cout << "Enter selection (0-" << UPDATE_MENU_OPTIONS << "): ";
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			choice = -1; // mark as invalid input
		}

		switch (choice) {
		case 0:
			cout << "Returning to main menu." << endl << endl;
			delete temp;
			return;

		case 1:
			updateItemName(temp);
			continue;

		case 2:
			updateItemQuantity(temp);
			continue;

		case 3:
			updateItemCategory(temp);
			continue;

		case 4:
			updateWarehouseLocation(temp, "Zone");
			continue;

		case 5:
			updateWarehouseLocation(temp, "Aisle");
			continue;

		case 6:
			updateWarehouseLocation(temp, "Shelf");
			continue;

		case 7:
			pushUpdateItem(item, temp, root);
			delete temp;
			return;

		default:
			cout << "Please enter a value between 0 and " << UPDATE_MENU_OPTIONS << "." << endl << endl;
		}
	}
}

void selectItemToUpdate(ItemsBST& bst) {
	string id;

	cout << "Enter item ID to update: I";
	cin >> id;

	string selectedId = "I" + format("{:03}", stoi(id)); // format item ID to match CSV format

	Item* item = bst.searchItemById(selectedId, false);

	if (item == nullptr) {
		cout << "Item ID not found. Returning to main menu." << endl << endl;
		return;
	}

	displayUpdateMenu(item, bst.getRoot());
}

void deleteItem(ItemsBST& bst) {
	string selectedId;
	cout << "Enter item ID to delete: I";
	cin >> selectedId;

	Item* item = bst.searchItemById("I" + format("{:03}", stoi(selectedId)), false);
	if (item == nullptr) {
		cout << "Item ID not found. Returning to main menu." << endl << endl;
		return;
	}

	while (true) {
		cout << "Confirm delete item " << item->itemId << "? (Y/N): ";
		char choice;
		cin >> choice;

		switch (choice) {
		case 'Y': case 'y': {
			ofstream file("items.csv", ios::trunc);
			file << "itemID,itemName,quantity,categoryID,zone,aisle,shelf\n";
			file.close();

			bst.deleteNode(item);
			pushUpdateItem(bst.getRoot());

			cout << "Item I" << format("{:03}", stoi(selectedId)) << " deleted successfully." << endl << endl;
			return;
		}

		case 'N': case 'n':
			cout << "Cancelled delete item." << endl << endl;
			return;

		default:
			cout << "Invalid input. Please enter Y or N." << endl << endl;
			continue;
		}
	}
}

void task4_menu() {
	ItemsBST bst;
	loadItemsFromCSV(bst);

	const int MAIN_MENU_OPTIONS = 5;

	// main menu
	while (true) {
		int choice = -1;

		cout << "--- MAIN MENU INTERFACE ---" << endl;
		cout << "1. View all items" << endl;
		cout << "2. Search for items based on specific criteria" << endl;
		cout << "3. Insert records" << endl;
		cout << "4. Update records" << endl;
		cout << "5. Delete records" << endl;
		cout << "0. Exit" << endl;
		cout << "Enter selection (0-" << MAIN_MENU_OPTIONS << "): ";

		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			choice = -1; // mark as invalid input
		}

		switch (choice) {
		case 0: // exit program
			cout << "Exiting program." << endl;
			return;
		case 1: // view all items
			bst.displayAllItems();
			continue;
		case 2: { // search items
			displaySearchMenu(bst);
			break;
		}
		case 3: {
			insertNewItem(bst);
			break;
		}
		case 4:
			selectItemToUpdate(bst);
			break;
		case 5:
			deleteItem(bst);
			break;
		default:
			cout << "Please enter a value between 0 and " << MAIN_MENU_OPTIONS << "." << endl << endl;
		}
	}
}



// TASK 5
void task5_menu() {
	cout << "Starting Warehouse Layout and Navigation Module..." << endl;

	try {

		string csvFilePath = "layout.csv";
		CSVParser parser(csvFilePath);
		parser.parseLayoutData();
		WarehouseLocationList* locations = parser.getLayoutLocations();
		cout << "CSV parsing complete. Extracted " << locations->getSize() << " locations." << endl;

		LayoutTree warehouse("Warehouse_Entry");
		WarehouseBuilder builder(warehouse, locations);
		builder.buildLayout();
		cout << "Warehouse layout built." << endl;

		warehouse.displayLayout();

		WarehouseStack pathStack;
		WarehouseNavigation navigator(warehouse, pathStack);

		string userInput;
		cout << "\n--- Interactive Navigation ---" << endl;
		cout << "Enter a target shelf name to find the path (or type 'exit' or 'quit' to finish):" << endl;

		while (true) {
			cout << "\nTarget Shelf: ";
			if (!(cin >> userInput)) break;

			if (userInput == "exit" || userInput == "quit") {
				break;
			}

			cout << "Finding path to " << userInput << "..." << endl;
			string path = navigator.findPath(userInput);
			cout << "Path: " << path << endl;
		}

		cout << "Demonstrating implicit memory cleanup via destructors on exit." << endl;

	}
	catch (const  exception& e) {
		cerr << "Error: " << e.what() << endl;
	}

	cout << "Warehouse Layout and Navigation Module finished." << endl;
}



// MAIN MENU
int main() {
	const int MAIN_MENU_OPTIONS = 5;
	const int PADDING = 40;

	while (true) {
		int choice = -1;

		cout << "--- WAREHOUSE ROBOT NAVIGATION SYSTEM ---" << endl;
		cout << left << setw(PADDING) << "1. Order Management" << " -> Nang Thet Htar San TP084170" << endl;
		cout << left << setw(PADDING) << "2. Robot Assignment" << " -> Chan Jie Lin TP086244" << endl;
		cout << left << setw(PADDING) << "3. Robot Navigation & Path Tracking" << " -> Kulikova Yana TP071330" << endl;
		cout << left << setw(PADDING) << "4. Item Search & Management" << " -> Her Cheng En TP084746" << endl;
		cout << left << setw(PADDING) << "5. Warehouse Layout & Navigation" << " -> Christopher Wamae Muraguri TP081493" << endl;
		cout << "0. Exit Program" << endl;
		cout << "Enter selection (0-" << MAIN_MENU_OPTIONS << "): ";

		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			choice = -1;
		}

		switch (choice) {
		case 0:
			cout << "Exiting program." << endl;
			return 0;

		case 1: {
			task1_menu();
			break;
		}
			
		case 2:
			task2_menu();
			break;
			
		case 3:
			task3_menu();
			break;

		case 4:
			task4_menu();
			break;

		case 5:
			task5_menu();
			break;

		default:
			cout << "Invalid choice. Please enter a number between 0 and " << MAIN_MENU_OPTIONS << "." << endl << endl;
		}
	}

	return 0;
}