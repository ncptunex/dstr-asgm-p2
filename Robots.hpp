#ifndef ROBOTS_HPP
#define ROBOTS_HPP

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Robots {
	string robotID;
	string robotName;
	string status;
	string currentTask;
	int totalTasks;
	string maintenance;
};

struct Orders {
	string orderID;
	string customerName;
	string itemID;
	string orderStatus;
	string assignedRobotID;
	string priority;
	string orderTime;
};

struct AssignmentsNode {
	string assignmentID;
	string orderID;
	string robotID;
	AssignmentsNode* next;   //the ptr to next node for linkedlist

	AssignmentsNode(string aID, string oID, string rID) {
		assignmentID = aID;
		orderID = oID;
		robotID = rID;
		next = nullptr;
	}
};

class AssignmentsList {
private:
	AssignmentsNode* head;	//first nede
	int count;				//no of nodes
	int nextID;				//generate new assignmentID

public:
	AssignmentsList() {
		//constructor
		head = nullptr;
		count = 0;
		nextID = 4; //start from A004
	}
	//add new asgm to the end of the list
	void addAssignment(string orderID, string robotID) {
		//generate new assignment ID
		string id = "A00" + to_string(nextID);
		if (nextID >= 10) id = "A0" + to_string(nextID);

		AssignmentsNode* newNode = new AssignmentsNode(id, orderID, robotID);

		if (head == nullptr) {
			head = newNode;
		}
		else {
			AssignmentsNode* temp = head;
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = newNode;
		}

		count++;
		nextID++;
	}

	void display() {
		if (head == nullptr) {
			cout << "The assignment table is still empty, nothing is recorded yet." << endl;
			return;
		}

		cout << " " << endl;
		cout << "========== 7. ASSIGNMENT HISTORY ==========" << endl;
		cout << "AssignID | OrderID | RobotID" << endl;
		cout << "===========================================" << endl;

		AssignmentsNode* temp = head;
		while (temp != nullptr) {
			cout << temp->assignmentID << " | " << temp->orderID << " | " << temp->robotID << endl;
			temp = temp->next;
		}
	}

	//save back to csv file
	void saveToCSV(string fileName) {
		//read exiting asgm w/o removing them
		ifstream readFile(fileName);
		string existingAsgm = "";
		string line;

		getline(readFile, line); //read header
		while (getline(readFile, line)) {
			existingAsgm += line + "\n"; //keep a001,2,3
		}
		readFile.close();

		//write everything backk
		ofstream file(fileName);
		if (!file.is_open()) {
			cout << "Fail to save to " << fileName << endl;
			return;
		}
		file << "assignmentID, orderID, robotID" << endl;
		file << existingAsgm;

		//add new asgm from linked list
		AssignmentsNode* temp = head;
		while (temp != nullptr) {
			file << temp->assignmentID << "," << temp->orderID << "," << temp->robotID << endl;
			temp = temp->next;  //move to next X delete
		}
		file.close();
		cout << "Assignments saved successfully to: " << fileName << endl;
	}
};

#endif
#pragma once