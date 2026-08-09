#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

// --- CSV Parser & Location Structures ---

struct LayoutLocation {
    std::string locationID;
    std::string locationName;
    std::string locationType;
    std::string parentLocationID;
};

struct LocationNode {
    LayoutLocation data;
    LocationNode* next;
    LocationNode(const LayoutLocation& loc) : data(loc), next(nullptr) {}
};

class WarehouseLocationList {
private:
    LocationNode* head;
    LocationNode* tail;
    int size;
public:
    WarehouseLocationList();
    ~WarehouseLocationList();
    void push_back(const LayoutLocation& loc);
    int getSize() const;
    LocationNode* getHead() const;
    WarehouseLocationList(const WarehouseLocationList&) = delete;
    WarehouseLocationList& operator=(const WarehouseLocationList&) = delete;
};

class CSVParser {
private:
    std::string filePath;
    WarehouseLocationList* parsedLayoutLocations;
    LayoutLocation parseLayoutCsvLine(const std::string& line);
public:
    CSVParser(const std::string& path);
    void parseLayoutData();
    WarehouseLocationList* getLayoutLocations() const;
    ~CSVParser();
    CSVParser(const CSVParser&) = delete;
    CSVParser& operator=(const CSVParser&) = delete;
};

// --- Layout Tree Structures ---

struct LayoutNode {
    std::string id;
    std::string name;
    std::string type;
    LayoutNode* firstChild;
    LayoutNode* nextSibling;
    LayoutNode(const std::string& nodeId, const std::string& nodeName, const std::string& nodeType);
    ~LayoutNode();
    LayoutNode(const LayoutNode&) = delete;
    LayoutNode& operator=(const LayoutNode&) = delete;
};

class LayoutTree {
private:
    LayoutNode* root;
    void destroyNode(LayoutNode* node);
    void displayNode(LayoutNode* node, const std::string& parentId) const;
public:
    LayoutTree(const std::string& rootName);
    ~LayoutTree();
    LayoutNode* getRoot() const;
    LayoutNode* addChild(LayoutNode* parent, const std::string& childId, const std::string& childName, const std::string& childType);
    LayoutNode* findChild(LayoutNode* parent, const std::string& childName) const;
    LayoutNode* findNodeById(LayoutNode* current, const std::string& id) const;
    void displayLayout() const;
    LayoutTree(const LayoutTree&) = delete;
    LayoutTree& operator=(const LayoutTree&) = delete;
};

// --- Stack & Queue Structures ---

struct StackNode {
    std::string data;
    StackNode* next;
    StackNode(const std::string& value);
    ~StackNode();
    StackNode(const StackNode&) = delete;
    StackNode& operator=(const StackNode&) = delete;
};

class WarehouseStack {
private:
    StackNode* top;
    int count;
    void destroyStack();
public:
    WarehouseStack();
    ~WarehouseStack();
    void push(const std::string& value);
    std::string pop();
    std::string peek() const;
    bool isEmpty() const;
    int size() const;
    WarehouseStack(const WarehouseStack&) = delete;
    WarehouseStack& operator=(const WarehouseStack&) = delete;
};

struct QueNode {
    std::string data;
    QueNode* next;
    QueNode(const std::string& value);
    ~QueNode();
    QueNode(const QueNode&) = delete;
    QueNode& operator=(const QueNode&) = delete;
};

class WarehouseQueue {
private:
    QueNode* front;
    QueNode* rear;
    int count;
    void destroyQueue();
public:
    WarehouseQueue();
    ~WarehouseQueue();
    void enqueue(const std::string& value);
    std::string dequeue();
    std::string peek() const;
    bool isEmpty() const;
    int size() const;
    WarehouseQueue(const WarehouseQueue&) = delete;
    WarehouseQueue& operator=(const WarehouseQueue&) = delete;
};

// --- Builder & Navigation ---

class WarehouseBuilder {
private:
    LayoutTree& layoutTree;
    WarehouseLocationList* warehouseLocations;
public:
    WarehouseBuilder(LayoutTree& tree, WarehouseLocationList* locations);
    void buildLayout();
    WarehouseBuilder(const WarehouseBuilder&) = delete;
    WarehouseBuilder& operator=(const WarehouseBuilder&) = delete;
};

class WarehouseNavigation {
private:
    const LayoutTree& layoutTree;
    WarehouseStack& pathStack;
    bool dfs(LayoutNode* currentNode, const std::string& targetShelf);
public:
    WarehouseNavigation(const LayoutTree& tree, WarehouseStack& stack);
    std::string findPath(const std::string& targetShelf);
    WarehouseNavigation(const WarehouseNavigation&) = delete;
    WarehouseNavigation& operator=(const WarehouseNavigation&) = delete;
};
