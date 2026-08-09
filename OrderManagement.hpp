#ifndef ORDER_MANAGEMENT_HPP
#define ORDER_MANAGEMENT_HPP

// ============================================================
//  OrderManagement.hpp
//  CT077-3-2 DSTR  |  Lab Evaluation Work #2
//  Task 1: Order Management Module
//
//  Data Structure Used: Queue (self-implemented singly linked list)
//
//  Rationale:
//    Customer orders must be processed in the exact sequence they
//    arrive (First-In, First-Out). A Queue naturally enforces FIFO
//    ordering, giving O(1) enqueue and O(1) dequeue.
//    A linked-list-based queue avoids a fixed-size array, allowing
//    a continuous inflow of orders with no costly resizing.
//
//  CSV Files:
//    - orders.csv      : all orders (Pending/In_Progress/Completed)
//    - completed.csv   : log of completed orders only
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

const string ORDERS_FILE = "orders.csv";
const string COMPLETED_FILE = "completed.csv";

static string getTimestamp() {
    time_t now = time(nullptr);
    struct tm tmInfo;
#ifdef _WIN32
    localtime_s(&tmInfo, &now);
#else
    localtime_r(&now, &tmInfo);
#endif
    char buf[6];
    strftime(buf, sizeof(buf), "%H:%M", &tmInfo);
    return string(buf);
}

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

static string formatOrderID(int id) {
    string s = to_string(id);
    while (s.length() < 3) s = "0" + s;
    return "ORD" + s;
}

struct Order {
    string orderID;
    string customerName;
    string itemID;
    string orderStatus;
    string assignedRobotID;
    string priority;
    string orderTime;

    Order()
        : orderID(""), customerName(""), itemID(""),
        orderStatus(""), assignedRobotID("NULL"),
        priority(""), orderTime("") {
    }

    Order(int id, const string& customer, const string& item,
        const string& prio)
        : orderID(formatOrderID(id)),
        customerName(customer),
        itemID(item),
        orderStatus("Pending"),
        assignedRobotID("NULL"),
        priority(prio),
        orderTime(getTimestamp()) {
    }

    Order(const string& oid, const string& customer,
        const string& item, const string& status,
        const string& robot, const string& prio,
        const string& time)
        : orderID(oid), customerName(customer), itemID(item),
        orderStatus(status), assignedRobotID(robot),
        priority(prio), orderTime(time) {
    }

    string toCSV() const {
        return orderID + "," +
            customerName + "," +
            itemID + "," +
            orderStatus + "," +
            assignedRobotID + "," +
            priority + "," +
            orderTime;
    }
};

struct QueueNode {
    Order      data;
    QueueNode* next;
    explicit QueueNode(const Order& o) : data(o), next(nullptr) {}
};

class OrderQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int        size;
    int        maxCapacity;

public:
    explicit OrderQueue(int capacity = 100)
        : front(nullptr), rear(nullptr),
        size(0), maxCapacity(capacity) {
    }

    ~OrderQueue() {
        QueueNode* curr = front;
        while (curr != nullptr) {
            QueueNode* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
        front = rear = nullptr;
    }

    bool isEmpty() const { return front == nullptr; }
    bool isFull()  const { return size >= maxCapacity; }
    int  getSize() const { return size; }

    bool enqueue(const Order& order) {
        if (isFull()) {
            cout << "\n  [ERROR] Queue at capacity (" << maxCapacity
                << "). Cannot accept new orders.\n";
            return false;
        }
        QueueNode* node = new QueueNode(order);
        if (isEmpty()) {
            front = rear = node;
        }
        else {
            rear->next = node;
            rear = node;
        }
        ++size;
        return true;
    }

    bool dequeue(Order& out) {
        if (isEmpty()) {
            cout << "\n  [ERROR] Queue is empty. No orders to process.\n";
            return false;
        }
        out = front->data;
        QueueNode* tmp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete tmp;
        --size;
        return true;
    }

    bool peek(Order& out) const {
        if (isEmpty()) {
            cout << "\n  [INFO] Queue is empty. Nothing to peek.\n";
            return false;
        }
        out = front->data;
        return true;
    }

    void displayPending() const {
        if (isEmpty()) {
            cout << "\n  [INFO] No pending orders in the queue.\n";
            return;
        }
        cout << "\n  +--------+----------------+-------+-------------+-----------+----------+-------+\n";
        cout << "  | ID     | Customer       | Item  | Status      | Robot     | Priority | Time  |\n";
        cout << "  +--------+----------------+-------+-------------+-----------+----------+-------+\n";
        QueueNode* curr = front;
        while (curr != nullptr) {
            const Order& o = curr->data;
            string cust = o.customerName.length() > 14 ? o.customerName.substr(0, 14) : o.customerName;
            string robot = o.assignedRobotID.length() > 9 ? o.assignedRobotID.substr(0, 9) : o.assignedRobotID;
            cout << "  | " << left
                << setw(6) << o.orderID << " | "
                << setw(14) << cust << " | "
                << setw(5) << o.itemID << " | "
                << setw(11) << o.orderStatus << " | "
                << setw(9) << robot << " | "
                << setw(8) << o.priority << " | "
                << setw(5) << o.orderTime << " |\n";
            curr = curr->next;
        }
        cout << "  +--------+----------------+-------+-------------+-----------+----------+-------+\n";
        cout << "  Total pending: " << size << " order(s)\n";
    }

    QueueNode* getFront() const { return front; }
};

struct HistoryNode {
    Order        data;
    HistoryNode* next;
    explicit HistoryNode(const Order& o) : data(o), next(nullptr) {}
};

class CompletedList {
private:
    HistoryNode* head;
    int          count;
public:
    CompletedList() : head(nullptr), count(0) {}

    ~CompletedList() {
        HistoryNode* curr = head;
        while (curr) {
            HistoryNode* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }

    void addCompleted(const Order& o) {
        HistoryNode* node = new HistoryNode(o);
        node->next = head;
        head = node;
        ++count;
    }

    void displayCompleted() const {
        if (!head) {
            cout << "\n  [INFO] No completed orders yet.\n";
            return;
        }
        cout << "\n  +--------+----------------+-------+-------------+-----------+----------+-------+\n";
        cout << "  | ID     | Customer       | Item  | Status      | Robot     | Priority | Time  |\n";
        cout << "  +--------+----------------+-------+-------------+-----------+----------+-------+\n";
        HistoryNode* curr = head;
        while (curr) {
            const Order& o = curr->data;
            string cust = o.customerName.length() > 14 ? o.customerName.substr(0, 14) : o.customerName;
            string robot = o.assignedRobotID.length() > 9 ? o.assignedRobotID.substr(0, 9) : o.assignedRobotID;
            cout << "  | " << left
                << setw(6) << o.orderID << " | "
                << setw(14) << cust << " | "
                << setw(5) << o.itemID << " | "
                << setw(11) << o.orderStatus << " | "
                << setw(9) << robot << " | "
                << setw(8) << o.priority << " | "
                << setw(5) << o.orderTime << " |\n";
            curr = curr->next;
        }
        cout << "  +--------+----------------+-------+-------------+-----------+----------+-------+\n";
        cout << "  Total completed: " << count << " order(s)\n";
    }

    HistoryNode* getHead() const { return head; }
    int getCount() const { return count; }
};

class CSVHandler {
public:
    static bool parseLine(const string& line, Order& out) {
        stringstream ss(line);
        string f[7];
        int i = 0;
        string token;
        while (getline(ss, token, ',') && i < 7)
            f[i++] = trim(token);
        if (i < 7) return false;
        out = Order(f[0], f[1], f[2], f[3], f[4], f[5], f[6]);
        return true;
    }

    static int loadOrders(const string& filename,
        OrderQueue& queue,
        int& maxIdSeen)
    {
        ifstream file(filename);
        if (!file.is_open()) return 0;
        string line;
        getline(file, line); // skip header
        int loaded = 0;
        while (getline(file, line)) {
            if (trim(line).empty()) continue;
            Order o;
            if (parseLine(line, o) && o.orderStatus == "Pending") {
                queue.enqueue(o);
                // Extract numeric part of ORD001 → 1
                int num = 0;
                try { num = stoi(o.orderID.substr(3)); }
                catch (...) {}
                if (num > maxIdSeen) maxIdSeen = num;
                ++loaded;
            }
        }
        file.close();
        return loaded;
    }

    static int loadCompleted(const string& filename,
        CompletedList& list,
        int& maxIdSeen)
    {
        ifstream file(filename);
        if (!file.is_open()) return 0;
        string line;
        getline(file, line); // skip header
        string lines[1000];
        int count = 0;
        while (getline(file, line) && count < 1000)
            if (!trim(line).empty()) lines[count++] = line;
        file.close();
        for (int i = 0; i < count; ++i) {
            Order o;
            if (parseLine(lines[i], o)) {
                list.addCompleted(o);
                int num = 0;
                try { num = stoi(o.orderID.substr(3)); }
                catch (...) {}
                if (num > maxIdSeen) maxIdSeen = num;
            }
        }
        return count;
    }

    static bool saveOrders(const string& filename,
        const OrderQueue& queue)
    {
        ofstream file(filename, ios::trunc);
        if (!file.is_open()) {
            cout << "\n  [ERROR] Cannot write to " << filename << "\n";
            return false;
        }
        file << "orderID,customerName,itemID,orderStatus,assignedRobotID,priority,orderTime\n";
        QueueNode* curr = queue.getFront();
        while (curr) {
            file << curr->data.toCSV() << "\n";
            curr = curr->next;
        }
        file.close();
        return true;
    }

    static bool appendCompleted(const string& filename,
        const Order& order)
    {
        ifstream check(filename);
        bool needsHeader = !check.is_open() ||
            check.peek() == ifstream::traits_type::eof();
        check.close();

        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cout << "\n  [ERROR] Cannot write to " << filename << "\n";
            return false;
        }
        if (needsHeader)
            file << "orderID,customerName,itemID,orderStatus,assignedRobotID,priority,orderTime\n";
        file << order.toCSV() << "\n";
        file.close();
        return true;
    }
};

class OrderManagementSystem {
private:
    OrderQueue    pendingQueue;
    CompletedList completedHistory;
    int           nextOrderId;

    static void line() {
        cout << "\n  " << string(63, '-') << "\n";
    }

public:
    explicit OrderManagementSystem(int capacity = 100)
        : pendingQueue(capacity), nextOrderId(1)
    {
        int maxId = 0;
        int pending = CSVHandler::loadOrders(ORDERS_FILE, pendingQueue, maxId);
        int completed = CSVHandler::loadCompleted(COMPLETED_FILE, completedHistory, maxId);
        nextOrderId = maxId + 1;

        if (pending + completed > 0) {
            cout << "\n  [CSV] Loaded " << pending
                << " pending order(s) from " << ORDERS_FILE << "\n";
            cout << "  [CSV] Loaded " << completed
                << " completed order(s) from " << COMPLETED_FILE << "\n";
        }
        else {
            cout << "\n  [CSV] No existing data found. Starting fresh.\n";
        }
    }

    bool addOrder(const string& customer, const string& itemID,
        const string& priority)
    {
        if (customer.empty() || itemID.empty() || priority.empty()) {
            cout << "\n  [ERROR] All fields are required.\n";
            return false;
        }
        Order o(nextOrderId, customer, itemID, priority);
        if (pendingQueue.enqueue(o)) {
            CSVHandler::saveOrders(ORDERS_FILE, pendingQueue);
            cout << "\n  [ORDER RECEIVED] " << o.orderID
                << " | Customer: " << customer
                << " | Item: " << itemID
                << " | Priority: " << priority << "\n";
            cout << "  [CSV] orders.csv updated.\n";
            ++nextOrderId;
            return true;
        }
        return false;
    }

    bool processNextOrder(Order& out) {
        if (!pendingQueue.dequeue(out)) return false;
        out.orderStatus = "In_Progress";
        CSVHandler::saveOrders(ORDERS_FILE, pendingQueue);
        cout << "\n  [IN PROGRESS] " << out.orderID
            << " dispatched → Item: " << out.itemID << "\n";
        cout << "  [CSV] orders.csv updated.\n";
        return true;
    }

    void completeOrder(Order& o) {
        o.orderStatus = "Completed";
        completedHistory.addCompleted(o);
        CSVHandler::appendCompleted(COMPLETED_FILE, o);
        cout << "\n  [COMPLETED] " << o.orderID
            << " (" << o.itemID << ") done.\n";
        cout << "  [CSV] completed.csv updated.\n";
    }

    void peekNextOrder() const {
        Order o;
        if (pendingQueue.peek(o)) {
            cout << "\n  [NEXT IN QUEUE] " << o.orderID
                << " | " << o.customerName
                << " | Item: " << o.itemID
                << " | Priority: " << o.priority << "\n";
        }
    }

    void displayPendingOrders()   const { pendingQueue.displayPending(); }
    void displayCompletedOrders() const { completedHistory.displayCompleted(); }

    void displaySummary() const {
        line();
        cout << "  ORDER MANAGEMENT SUMMARY\n";
        line();
        cout << "  Pending   : " << pendingQueue.getSize() << " order(s)\n";
        cout << "  Completed : " << completedHistory.getCount() << " order(s)\n";
        cout << "  Next ID   : " << formatOrderID(nextOrderId) << "\n";
        cout << "  Files     : " << ORDERS_FILE << " | " << COMPLETED_FILE << "\n";
        line();
    }

    bool hasPendingOrders() const { return !pendingQueue.isEmpty(); }
    int  pendingCount()     const { return pendingQueue.getSize(); }
};

#endif 