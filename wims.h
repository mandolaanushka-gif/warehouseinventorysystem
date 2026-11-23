
#ifndef WIMS_H
#define WIMS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstdio>

using namespace std;

// ================= PRODUCT CLASS =================

class Product
{
private:
    int id;
    string name;
    int quantity;
    double price;
    string expiry;
    Product* next;

public:
    Product(int id, string name, int quantity,
            double price, string expiry);

    int getId();
    string getName();
    int getQuantity();
    double getPrice();
    string getExpiry();
    Product* getNext();

    void setName(string name);
    void setQuantity(int quantity);
    void setPrice(double price);
    void setExpiry(string expiry);

    friend class WarehouseSystem;
    friend class Database;
};

// ================= AVL NODE =================

class AVLNode
{
public:
    Product* product;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Product* product);
};

// ================= AVL TREE =================

class AVLTree
{
private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);

    AVLNode* rotateLeft(AVLNode* node);
    AVLNode* rotateRight(AVLNode* node);

    AVLNode* insertNode(AVLNode* node, Product* product);
    AVLNode* deleteNode(AVLNode* node, int id);
    AVLNode* findMin(AVLNode* node);

    Product* searchNode(AVLNode* node, int id);

    void displayInOrder(AVLNode* node);

public:
    AVLTree();
    ~AVLTree();

    void insert(Product* product);
    void remove(int id);
    Product* search(int id);
    void display();
};

// ================= TRANSACTION CLASS =================

class Transaction
{
private:
    int transactionId;
    string type;
    int productId;
    int quantity;
    string date;
    Transaction* next;

public:
    Transaction(int transactionId, string type,
                int productId, int quantity, string date);

    int getTransactionId();
    string getType();
    int getProductId();
    int getQuantity();
    string getDate();
    Transaction* getNext();

    friend class WarehouseSystem;
    friend class Database;
};

// ================= ORDER CLASS =================

class Order
{
private:
    int orderId;
    int productId;
    int quantity;
    string status;
    string date;
    Order* next;

public:
    Order(int orderId, int productId, int quantity,
          string status, string date);

    int getOrderId();
    int getProductId();
    int getQuantity();
    string getStatus();
    string getDate();
    Order* getNext();

    void setStatus(string status);

    friend class WarehouseSystem;
    friend class Database;
};

// ================= WAREHOUSE SYSTEM =================

class WarehouseSystem
{
private:
    Product* productHead;
    Transaction* transactionHead;

    Order* orderFront;
    Order* orderRear;

    AVLTree productIndex;

    int nextProductId;
    int nextTransactionId;
    int nextOrderId;

    void addTransaction(string type, int productId, int quantity);

public:
    WarehouseSystem();
    ~WarehouseSystem();

    void addProduct(string name, int quantity,
                    double price, string expiry);

    void updateProduct(int id, string name, int quantity,
                       double price, string expiry);

    void deleteProduct(int id);

    Product* searchProduct(int id);

    void displayProducts();

    void stockIn(int id, int quantity);
    void stockOut(int id, int quantity);

    void placeOrder(int productId, int quantity);
    void processNextOrder();
    void displayOrders();

    void displayTransactions();

    void generateInventoryReport();
    void generateTransactionReport();
    void generateOrderReport();

    Product* getProductHead();
    Transaction* getTransactionHead();
    Order* getOrderFront();

    friend class Database;
};

// ================= DATE FUNCTION =================

string getCurrentDate();

#endif
