#ifndef WIMS_H
#define WIMS_H

#include <iostream>
#include <string>
using namespace std;

#define LOW_LIMIT 10
#define MAX 50

struct Product {
    int id;
    string name;
    int qty;
    float price;
    Product* next;
};

struct Transaction {
    int no;
    string type;
    int id;
    int qty;
    Transaction* next;
};

struct Order {
    int orderNo;
    int id;
    int qty;
    Order* next;
};

// Product functions
Product* getProductHead();
Product* findProduct(int id);
void addProduct();
void showProducts();
void searchProduct();
void updateProduct();

// Stock functions
void stockIn();
int stockOut(int id, int qty);
void stockOutMenu();
void checkLowStock();
void checkExpiry();

// Transaction functions
void addTransaction(string type, int id, int qty);
void showTransactions();

// Order functions
void enqueueOrder();
void processOrder();
void showOrders();

#endif
