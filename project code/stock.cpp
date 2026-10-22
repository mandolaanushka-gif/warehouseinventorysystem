#include "wims.h"

void stockIn() {
    int id, qty;

    cout << "Enter ID and quantity: ";
    cin >> id >> qty;

    Product* p = findProduct(id);

    if (p == NULL || qty <= 0) {
        cout << "Wrong ID or quantity.\n";
        return;
    }

    p->qty = p->qty + qty;

    addTransaction("IN", id, qty);

    cout << "Stock added.\n";
}

int stockOut(int id, int qty) {
    Product* p = findProduct(id);

    if (p == NULL || qty <= 0)
        return 0;

    if (p->qty < qty)
        return 0;

    p->qty = p->qty - qty;

    addTransaction("OUT", id, qty);

    return 1;
}

void stockOutMenu() {
    int id, qty;

    cout << "Enter ID and quantity: ";
    cin >> id >> qty;

    if (stockOut(id, qty)) {
        cout << "Stock removed.\n";
    } else {
        cout << "Not possible (wrong ID or not enough stock).\n";
    }
}

void checkLowStock() {
    Product* low[MAX];
    int count = 0;

    Product* temp = getProductHead();

    while (temp != NULL && count < MAX) {
        if (temp->qty < LOW_LIMIT) {
            low[count] = temp;
            count++;
        }

        temp = temp->next;
    }

    if (count == 0) {
        cout << "No low stock items.\n";
        return;
    }

    cout << "Low stock items:\n";

    for (int i = 0; i < count; i++) {
        cout << low[i]->id << " "
             << low[i]->name
             << " Qty = " << low[i]->qty << endl;
    }
}

void checkExpiry() {
    cout << "Expiry feature will be added in Phase 3.\n";
}
