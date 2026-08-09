#include "NavigationStack.hpp"
#include <fstream>
#include <sstream>

#include <cstdlib>//for random
#include <ctime>


// Constructor
NavigationStack::NavigationStack()
{
    // Initially stack is empty
    top = NULL;
    totalSteps = 0;
    obstacleDetected = false;

    failedStepNum = 0;
    failedMove = "";
}



// Check whether stack contains data
bool NavigationStack::isEmpty()
{
    return top == NULL;

}

void NavigationStack::loadRouteFromCSV(string filename, string robotID)
{
    while (!isEmpty()) {
        pop();
    }

    obstacleDetected = false;
    // Open CSV file
    ifstream routeFile(filename);

    // Open log file in append mode
    ofstream logFile("navigation_log.csv", ios::app);

    // Check whether file exists
    if (!routeFile)
    {
        cout << "Error opening file." << endl;
        return;
    }
    string line;


    // Skip header row
    getline(routeFile, line);

    // Read file line by line
    while (getline(routeFile, line))
    {
        stringstream ss(line);

        string csvRobotID;
        string stepText;
        string movement;

        // Read CSV columns separated by comma
        getline(ss, csvRobotID, ',');
        getline(ss, stepText, ',');
        getline(ss, movement, ',');

        // Convert step number from string to int
        int stepNumber = stoi(stepText);

        // Only load assigned robot route
        if (csvRobotID == robotID)
        {

            // Generate random obstacle event (10% probability)
            int obstacleChance = rand() % 10;

            if (obstacleChance == 0)
            {
                obstacleDetected = true;
                failedStepNum = stepNumber;
                failedMove = movement;

                logFile << csvRobotID << "," << stepNumber << "," << movement << "," << "Failed" << endl;

                break;
            }
            push(csvRobotID, stepNumber, movement);

            logFile << csvRobotID << "," << stepNumber << "," << movement << "," << "Success" << endl;
        }
    }

    logFile.close();
    routeFile.close();
}

// Push new movement into stack
void NavigationStack::push(string robotID, int stepNumber, string movement)
{
    // Create new node dynamically
    Node* newNode = new Node;

    // Store movement data
    newNode->data.robotID = robotID;
    newNode->data.stepNumber = stepNumber;
    newNode->data.movement = movement;

    // Link new node to current top
    newNode->next = top;

    // Move top pointer to new node
    top = newNode;
    totalSteps++;
}



// Remove latest movement from stack
NavigationStep NavigationStack::pop()
{
    NavigationStep removedStep;

    // Check stack underflow
    if (isEmpty())
    {
        removedStep.robotID = "";
        removedStep.stepNumber = -1;
        removedStep.movement = "EMPTY";

        return removedStep;
    }

    // Temporary pointer to current top
    Node* temp = top;

    // Store removed data
    removedStep = top->data;

    // Move top pointer down
    top = top->next;

    // Delete old top node
    delete temp;

    // Reduce step count
    totalSteps--;

    return removedStep;
}



// Display original forward path
void NavigationStack::displayForwardPath()
{
    if (isEmpty())
    {
        if (obstacleDetected)
        {
            cout << "Route could not start because of obstacle!" << endl;
        }
        else
        {
            cout << "No navigation data found." << endl;
        }
        return;
    }

    cout << "Forward Path : " << endl;

    for (int step = 1; step <= totalSteps; step++)
    {
        Node* current = top;

        while (current != NULL)
        {
            if (current->data.stepNumber == step)
            {
                cout << "Step " << current->data.stepNumber << ": " << current->data.movement << endl;

                break;
            }
            current = current->next;
        }
    }
}


// Display reverse path using pop()
void NavigationStack::displayReversePath()
{
    ofstream logFile("navigation_log.csv", ios::app);

    cout << "Returning..." << endl;

    // Continue until stack becomes empty
    while (!isEmpty())
    {
        NavigationStep step = pop();

        cout << "Step " << step.stepNumber << ": " << step.movement << endl;

        logFile << step.robotID << "," << step.stepNumber << "," << step.movement << "," << "Return" << endl;

    }
    logFile.close();
}


void NavigationStack::executeAssignedRobot()
{
    cout << "ROBOT NAVIGATION MODULE:" << endl;

    // Open assignments file
    ifstream assignmentFile("assignments.csv");

    // Create new navigation log file
    ofstream logFile("navigation_log.csv");

    logFile << "robotID," << "stepNumber," << "movement," << "status" << endl;
    logFile.close();

    // Check file existence
    if (!assignmentFile)
    {
        cout << "Error opening assignments file." << endl;
        return;
    }

    string line;
    // Skip header row
    getline(assignmentFile, line);

    // Read assignments one by one
    while (getline(assignmentFile, line))
    {
        stringstream ss(line);

        string assignmentID;
        string orderID;
        string assignedRobot;

        // Read CSV columns
        getline(ss, assignmentID, ',');
        getline(ss, orderID, ',');
        getline(ss, assignedRobot, ',');


        cout << "\nAssignment ID: " << assignmentID << endl;
        cout << "Robot Assigned: " << assignedRobot << endl;

        int attempt = 1;
        const int maxAttempts = 10;

        obstacleDetected = true;

        // Retry navigation until success or maximum attempts reached
        while (obstacleDetected && attempt <= maxAttempts)
        {
            cout << "\nAttempt " << attempt << endl;

            // Load assigned robot route into stack
            loadRouteFromCSV("robot_routes.csv", assignedRobot);

            // Show forward movement
            displayForwardPath();

            if (!obstacleDetected)
            {
                cout << "Item Collected." << endl;

                if (!isEmpty()) {
                    displayReversePath();
                    cout << "Robot Returned Successfully." << endl;
                }
                break;
            }
            else {
                cout << "Obstacle Detected at Step " << failedStepNum << ": " << failedMove << "!" << endl;

                if (!isEmpty()) {
                    displayReversePath();
                    cout << "Robot Returned to Base to Retry." << endl;
                }
                if (attempt == maxAttempts)
                {
                    cout << "Mission Failed" << endl;
                }
            }
            attempt++;
        }
    }
    assignmentFile.close();
}

// Destructor
NavigationStack::~NavigationStack()
{
    // Free all remaining nodes
    while (!isEmpty())
    {
        pop();
    }
}
