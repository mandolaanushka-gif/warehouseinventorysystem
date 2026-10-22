#include "wims.h"

Product* head = NULL;

Product* getProductHead() {
    return head;
}

Product* findProduct(int id) {
    Product* temp = head;

    while (temp != NULL) {
        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

void addProduct() {
    Product* p = new Product;

    cout << "Enter ID: ";
    cin >> p->id;

    if (findProduct(p->id) != NULL) {
        cout << "ID already exists!\n";
        delete p;
        return;
    }

    cout << "Enter name: ";
    cin >> p->name;

    cout << "Enter quantity: ";
    cin >> p->qty;

    cout << "Enter price: ";
    cin >> p->price;

    p->next = NULL;

    if (head == NULL) {
        head = p;
    } else {
        Product* temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = p;
    }

    cout << "Product added.\n";
}

void showProducts() {
    if (head == NULL) {
        cout << "Inventory is empty.\n";
        return;
    }

    Product* temp = head;

    cout << "\nID\tName\tQty\tPrice\n";

    while (temp != NULL) {
        cout << temp->id << "\t"
             << temp->name << "\t"
             << temp->qty << "\t"
             << temp->price << endl;

        temp = temp->next;
    }
}

void searchProduct() {
    int id;

    cout << "Enter ID to search: ";
    cin >> id;

    Product* p = findProduct(id);

    if (p == NULL) {
        cout << "Product not found.\n";
    } else {
        cout << "Found: " << p->name
             << ", Qty = " << p->qty
             << ", Price = " << p->price << endl;
    }
}

void updateProduct() {
    int id;

    cout << "Enter ID to update: ";
    cin >> id;

    Product* p = findProduct(id);

    if (p == NULL) {
        cout << "Product not found.\n";
        return;
    }

    cout << "Enter new name: ";
    cin >> p->name;

    cout << "Enter new price: ";
    cin >> p->price;

    cout << "Product updated.\n";
}