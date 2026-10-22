#include "wims.h"

Transaction* thead = NULL;
Transaction* ttail = NULL;
int tcount = 0;

void addTransaction(string type, int id, int qty) {
    Transaction* t = new Transaction;

    tcount++;

    t->no = tcount;
    t->type = type;
    t->id = id;
    t->qty = qty;
    t->next = NULL;

    if (thead == NULL) {
        thead = t;
        ttail = t;
    } else {
        ttail->next = t;
        ttail = t;
    }
}

void showTransactions() {
    if (thead == NULL) {
        cout << "No transactions yet.\n";
        return;
    }

    Transaction* temp = thead;

    cout << "\nNo\tType\tID\tQty\n";

    while (temp != NULL) {
        cout << temp->no << "\t"
             << temp->type << "\t"
             << temp->id << "\t"
             << temp->qty << endl;

        temp = temp->next;
    }
}
