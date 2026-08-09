#include "WarehouseNav.hpp"

using namespace std;

// --- WarehouseLocationList Implementation ---

WarehouseLocationList::WarehouseLocationList() : head(nullptr), tail(nullptr), size(0) {}

WarehouseLocationList::~WarehouseLocationList() {
    LocationNode* current = head;
    while (current) {
        LocationNode* next = current->next;
        delete current;
        current = next;
    }
}

void WarehouseLocationList::push_back(const LayoutLocation& loc) {
    LocationNode* newNode = new LocationNode(loc);
    if (!head) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

int WarehouseLocationList::getSize() const {
    return size;
}

LocationNode* WarehouseLocationList::getHead() const {
    return head;
}

// --- CSVParser Implementation ---

CSVParser::CSVParser(const string& path)
    : filePath(path), parsedLayoutLocations(new WarehouseLocationList()) {
}

CSVParser::~CSVParser() {
    delete parsedLayoutLocations;
}

LayoutLocation CSVParser::parseLayoutCsvLine(const string& line) {
    stringstream ss(line);
    string segment;
    LayoutLocation location;
    int columnCount = 0;

    while (getline(ss, segment, ',')) {
        if (columnCount == 0) {
            location.locationID = segment;
        }
        else if (columnCount == 1) {
            location.locationName = segment;
        }
        else if (columnCount == 2) {
            location.locationType = segment;
        }
        else if (columnCount == 3) {
            location.parentLocationID = segment;
            break;
        }
        columnCount++;
    }
    return location;
}

void CSVParser::parseLayoutData() {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Could not open CSV file: " + filePath);
    }

    string line;
    if (getline(file, line)) {
        // Skip header
    }

    while (getline(file, line)) {
        if (line.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }
        parsedLayoutLocations->push_back(parseLayoutCsvLine(line));
    }
    file.close();
}

WarehouseLocationList* CSVParser::getLayoutLocations() const {
    return parsedLayoutLocations;
}

// --- LayoutNode Implementation ---

LayoutNode::LayoutNode(const string& nodeId, const string& nodeName, const string& nodeType)
    : id(nodeId), name(nodeName), type(nodeType), firstChild(nullptr), nextSibling(nullptr) {
}

LayoutNode::~LayoutNode() {}

// --- LayoutTree Implementation ---

LayoutTree::LayoutTree(const string& rootName)
    : root(new LayoutNode("L000", rootName, "Root")) {
    if (!root) {
        throw runtime_error("Failed to allocate memory for root node.");
    }
}

LayoutTree::~LayoutTree() {
    destroyNode(root);
}

void LayoutTree::destroyNode(LayoutNode* node) {
    if (!node) return;
    destroyNode(node->firstChild);
    destroyNode(node->nextSibling);
    delete node;
}

LayoutNode* LayoutTree::getRoot() const {
    return root;
}

LayoutNode* LayoutTree::addChild(LayoutNode* parent, const string& childId, const string& childName, const string& childType) {
    if (!parent) throw invalid_argument("Parent node cannot be null.");
    if (findNodeById(root, childId)) return nullptr;

    LayoutNode* newChild = new LayoutNode(childId, childName, childType);
    if (!newChild) throw runtime_error("Failed to allocate memory for new child node.");

    newChild->nextSibling = parent->firstChild;
    parent->firstChild = newChild;
    return newChild;
}

LayoutNode* LayoutTree::findChild(LayoutNode* parent, const string& childName) const {
    if (!parent) return nullptr;
    LayoutNode* current = parent->firstChild;
    while (current) {
        if (current->name == childName) return current;
        current = current->nextSibling;
    }
    return nullptr;
}

LayoutNode* LayoutTree::findNodeById(LayoutNode* current, const string& id) const {
    if (!current) return nullptr;
    if (current->id == id) return current;
    LayoutNode* found = findNodeById(current->firstChild, id);
    if (found) return found;
    return findNodeById(current->nextSibling, id);
}

void LayoutTree::displayNode(LayoutNode* node, const string& parentId) const {
    if (!node) return;
    cout << left << setw(10) << node->id
        << setw(20) << node->name
        << setw(10) << node->type
        << setw(10) << parentId << endl;

    LayoutNode* currentChild = node->firstChild;
    while (currentChild) {
        displayNode(currentChild, node->id);
        currentChild = currentChild->nextSibling;
    }
}

void LayoutTree::displayLayout() const {
    if (!root) {
        cout << "Tree is empty." << endl;
        return;
    }
    cout << "\n--- Warehouse Layout Table ---" << endl;
    cout << left << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Type"
        << setw(10) << "Parent ID" << endl;
    cout << string(50, '-') << endl;
    displayNode(root, "None");
    cout << string(50, '-') << endl;
}

// --- WarehouseStack Implementation ---

StackNode::StackNode(const string& value) : data(value), next(nullptr) {}
StackNode::~StackNode() {}

WarehouseStack::WarehouseStack() : top(nullptr), count(0) {}
WarehouseStack::~WarehouseStack() { destroyStack(); }

void WarehouseStack::destroyStack() {
    while (!isEmpty()) pop();
}

void WarehouseStack::push(const string& value) {
    StackNode* newNode = new StackNode(value);
    if (!newNode) throw runtime_error("Failed to allocate memory for stack node.");
    newNode->next = top;
    top = newNode;
    count++;
}

string WarehouseStack::pop() {
    if (isEmpty()) throw out_of_range("Stack is empty. Cannot pop.");
    StackNode* temp = top;
    string poppedValue = temp->data;
    top = top->next;
    delete temp;
    count--;
    return poppedValue;
}

string WarehouseStack::peek() const {
    if (isEmpty()) throw out_of_range("Stack is empty. Cannot peek.");
    return top->data;
}

bool WarehouseStack::isEmpty() const { return top == nullptr; }
int WarehouseStack::size() const { return count; }

// --- WarehouseQueue Implementation ---

QueNode::QueNode(const string& value) : data(value), next(nullptr) {}
QueNode::~QueNode() {}

WarehouseQueue::WarehouseQueue() : front(nullptr), rear(nullptr), count(0) {}
WarehouseQueue::~WarehouseQueue() { destroyQueue(); }

void WarehouseQueue::destroyQueue() {
    while (!isEmpty()) dequeue();
}

void WarehouseQueue::enqueue(const string& value) {
    QueNode* newNode = new QueNode(value);
    if (!newNode) throw runtime_error("Failed to allocate memory for queue node.");
    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    count++;
}

string WarehouseQueue::dequeue() {
    if (isEmpty()) throw out_of_range("Queue is empty. Cannot dequeue.");
    QueNode* temp = front;
    string dequeuedValue = temp->data;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    delete temp;
    count--;
    return dequeuedValue;
}

string WarehouseQueue::peek() const {
    if (isEmpty()) throw out_of_range("Queue is empty. Cannot peek.");
    return front->data;
}

bool WarehouseQueue::isEmpty() const { return front == nullptr; }
int WarehouseQueue::size() const { return count; }

// --- WarehouseBuilder Implementation ---

WarehouseBuilder::WarehouseBuilder(LayoutTree& tree, WarehouseLocationList* locations)
    : layoutTree(tree), warehouseLocations(locations) {
}

void WarehouseBuilder::buildLayout() {
    LayoutNode* root = layoutTree.getRoot();
    if (!root) {
        cerr << "Error: LayoutTree root is null." << endl;
        return;
    }
    LocationNode* current = warehouseLocations->getHead();
    while (current) {
        const LayoutLocation& loc = current->data;
        if (loc.locationID == "L000") {
            current = current->next;
            continue;
        }
        LayoutNode* parentNode = layoutTree.findNodeById(root, loc.parentLocationID);
        if (!parentNode) {
            cerr << "Error: Parent location " << loc.parentLocationID << " not found for " << loc.locationID << endl;
            current = current->next;
            continue;
        }
        layoutTree.addChild(parentNode, loc.locationID, loc.locationName, loc.locationType);
        current = current->next;
    }
}

// --- WarehouseNavigation Implementation ---

WarehouseNavigation::WarehouseNavigation(const LayoutTree& tree, WarehouseStack& stack)
    : layoutTree(tree), pathStack(stack) {
}

bool WarehouseNavigation::dfs(LayoutNode* currentNode, const string& targetShelf) {
    if (!currentNode) return false;
    pathStack.push(currentNode->name);
    if (currentNode->type == "Shelf" && currentNode->name == targetShelf) return true;
    LayoutNode* child = currentNode->firstChild;
    while (child) {
        if (dfs(child, targetShelf)) return true;
        child = child->nextSibling;
    }
    pathStack.pop();
    return false;
}

string WarehouseNavigation::findPath(const string& targetShelf) {
    while (!pathStack.isEmpty()) pathStack.pop();
    LayoutNode* root = layoutTree.getRoot();
    if (!root) return "Error: Warehouse layout is empty.";
    if (!dfs(root, targetShelf)) return "Error: Shelf " + targetShelf + " not found.";

    WarehouseStack reversedStack;
    while (!pathStack.isEmpty()) reversedStack.push(pathStack.pop());
    string pathString;
    while (!reversedStack.isEmpty()) {
        pathString += reversedStack.pop();
        if (!reversedStack.isEmpty()) pathString += " -> ";
    }
    return pathString;
}
