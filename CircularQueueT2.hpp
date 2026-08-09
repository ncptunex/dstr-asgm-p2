#ifndef CIRCULARQUEUET2_HPP
#define CIRCULARQUEUET2_HPP

#include <iostream>
#include <string>
using namespace std;

class CircularQueueT2 {
private:
	string arr[20];	//array fixed size 20
	int front;		//points to front element
	int rear;		//points to back element
	int size;		//max size

public:
	//constructor
	CircularQueueT2() {
		size = 20;
		front = -1;
		rear = -1;
	}

	bool isEmpty() {
		return (front == -1);
	}

	bool isFull() {
		return ((rear + 1) % size == front);
	}

	void enqueue(string robotID) {
		//full X insert
		if (isFull()) {
			cout << "Wheel is Full." << endl;
			return;
		}

		//first element
		if (isEmpty()) {
			front = 0;
			rear = 0;
		}
		else {
			//circular movement
			rear = (rear + 1) % size;
		}
		arr[rear] = robotID; //insert robotID
	}

	string dequeue() {
		string robotID;

		//X remove if empty
		if (isEmpty()) {
			cout << "Wheel is empty, nothing to remove." << endl;
			return "";
		}

		robotID = arr[front];
		cout << "Robots selected: " << arr[front] << endl;

		//only one element
		if (front == rear) {
			front = rear = -1;
		}
		else {
			//circular move front
			front = (front + 1) % size;
		}
		return robotID;
	}

	//peek value w/o removing
	void peek() {
		if (isEmpty()) {
			cout << "There is no next robot because it's empty" << endl;
		}
		else {
			cout << "Next robot in line: " << arr[front] << endl;
		}
	}

	void display() {
		if (isEmpty()) {
			cout << "Wheel is empty." << endl;
			return;
		}

		cout << " " << endl;
		cout << "4. VIEW ROBOT WHEEL: ";

		int i = front;
		while (true) {
			cout << arr[i] << " ";

			if (i == rear)
				break;

			//circular move to the next element
			i = (i + 1) % size;
		}

		cout << endl;
	}

	//count the no of robots in list
	int getCount() {
		if (isEmpty())
			return 0;

		if (rear >= front) {
			return rear - front + 1;
		}
		else {
			return size - front + rear + 1;
		}
	}
};

#endif
#pragma once