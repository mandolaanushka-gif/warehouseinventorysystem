
#include "wims.h"

// ================= TRANSACTION CONSTRUCTOR =================

Transaction::Transaction(int transactionId, string type,
                         int productId, int quantity,
                         string date)
{
    this->transactionId = transactionId;
    this->type = type;
    this->productId = productId;
    this->quantity = quantity;
    this->date = date;
    next = NULL;
}

// ================= GETTER FUNCTIONS =================

int Transaction::getTransactionId()
{
    return transactionId;
}

string Transaction::getType()
{
    return type;
}

int Transaction::getProductId()
{
    return productId;
}

int Transaction::getQuantity()
{
    return quantity;
}

string Transaction::getDate()
{
    return date;
}

Transaction* Transaction::getNext()
{
    return next;
}

// ================= STOCK IN =================

void WarehouseSystem::stockIn(int id, int quantity)
{
    if (quantity <= 0)
    {
        cout << "\nInvalid quantity!\n";
        return;
    }

    Product* product = searchProduct(id);

    if (product == NULL)
    {
        cout << "\nProduct not found!\n";
        return;
    }

    product->setQuantity(
        product->getQuantity() + quantity
    );

    addTransaction("STOCK IN", id, quantity);

    cout << "\nStock added successfully!\n";
    cout << "Product: " << product->getName() << endl;
    cout << "Added Quantity: " << quantity << endl;
    cout << "Available Stock: "
         << product->getQuantity() << endl;
}

// ================= STOCK OUT =================

void WarehouseSystem::stockOut(int id, int quantity)
{
    if (quantity <= 0)
    {
        cout << "\nInvalid quantity!\n";
        return;
    }

    Product* product = searchProduct(id);

    if (product == NULL)
    {
        cout << "\nProduct not found!\n";
        return;
    }

    if (product->getQuantity() < quantity)
    {
        cout << "\nInsufficient stock available!\n";
        return;
    }

    product->setQuantity(
        product->getQuantity() - quantity
    );

    addTransaction("STOCK OUT", id, quantity);

    cout << "\nStock removed successfully!\n";
    cout << "Product: " << product->getName() << endl;
    cout << "Removed Quantity: " << quantity << endl;
    cout << "Remaining Stock: "
         << product->getQuantity() << endl;
}

// ================= DISPLAY TRANSACTIONS =================

void WarehouseSystem::displayTransactions()
{
    if (transactionHead == NULL)
    {
        cout << "\nNo transactions available!\n";
        return;
    }

    Transaction* temp = transactionHead;

    cout << "\n========== TRANSACTION HISTORY ==========\n";

    while (temp != NULL)
    {
        cout << "\nTransaction ID : "
             << temp->getTransactionId();

        cout << "\nType           : "
             << temp->getType();

        cout << "\nProduct ID     : "
             << temp->getProductId();

        cout << "\nQuantity       : "
             << temp->getQuantity();

        cout << "\nDate           : "
             << temp->getDate();

        cout << "\n-----------------------------------------\n";

        temp = temp->getNext();
    }
}
