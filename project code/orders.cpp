#include "wims.h"

Order* front = NULL;
Order* rear = NULL;
int ocount = 0;

void enqueueOrder() {
    Order* o = new Order;

    ocount++;
    o->orderNo = ocount;

    cout << "Enter product ID and quantity: ";
    cin >> o->id >> o->qty;

    o->next = NULL;

    if (rear == NULL) {
        front = o;
        rear = o;
    } else {
        rear->next = o;
        rear = o;
    }

    cout << "Order " << o->orderNo << " added.\n";
}

void processOrder() {
    if (front == NULL) {
        cout << "No pending orders.\n";
        return;
    }

    if (stockOut(front->id, front->qty)) {
        cout << "Order " << front->orderNo
             << " processed.\n";

        Order* temp = front;

        front = front->next;

        if (front == NULL)
            rear = NULL;

        delete temp;
    } else {
        cout << "Order " << front->orderNo
             << " cannot be processed (not enough stock).\n";
    }
}

void showOrders() {
    if (front == NULL) {
        cout << "No pending orders.\n";
        return;
    }

    Order* temp = front;

    cout << "\nOrderNo\tProductID\tQty\n";

    while (temp != NULL) {
        cout << temp->orderNo << "\t"
             << temp->id << "\t\t"
             << temp->qty << endl;

        temp = temp->next;
    }
}
