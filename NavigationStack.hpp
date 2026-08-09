#ifndef NAVIGATION_STACK_HPP
#define NAVIGATION_STACK_HPP

#include <iostream>
#include <string>

using namespace std;

// Structure to store one movement step
struct NavigationStep
{
    string robotID;
    int stepNumber;
    string movement;
};

// Node structure for linked list stack
struct Node
{
    NavigationStep data;
    Node* next;
};



class NavigationStack
{
private:

    // Pointer to top node of stack
    Node* top;

    int totalSteps;
    bool obstacleDetected;

    int failedStepNum;
    string failedMove;

public:

    // Constructor
    NavigationStack();

    // Check whether stack is empty
    bool isEmpty();
    //get info from csv file 
    void loadRouteFromCSV(string filename, string robotID);


    //reads assignments and executes the navigation process
    void executeAssignedRobot();

    // Insert movement into stack
    void push(string robotID, int stepNumber, string movement);

    // Remove latest movement from stack
    NavigationStep pop();

    // Display forward robot movement
    void displayForwardPath();

    // Display reverse return path
    void displayReversePath();

    // Destructor to free memory
    ~NavigationStack();
};

#endif