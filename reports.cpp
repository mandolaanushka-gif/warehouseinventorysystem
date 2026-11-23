
#include "wims.h"

// ================= INVENTORY REPORT =================

void WarehouseSystem::generateInventoryReport()
{
    ofstream file("inventory_report.txt");

    if (!file)
    {
        cout << "\nUnable to create inventory report!\n";
        return;
    }

    Product* temp = productHead;

    int totalProducts = 0;
    int totalQuantity = 0;
    double totalValue = 0;

    file << "========== WIMS INVENTORY REPORT ==========\n\n";

    cout << "\n========== INVENTORY REPORT ==========\n";

    while (temp != NULL)
    {
        file << "Product ID: " << temp->getId() << endl;
        file << "Name: " << temp->getName() << endl;
        file << "Quantity: " << temp->getQuantity() << endl;
        file << "Price: " << temp->getPrice() << endl;
        file << "Expiry: " << temp->getExpiry() << endl;
        file << "-------------------------------------\n";

        cout << "\nProduct ID: " << temp->getId();
        cout << "\nName: " << temp->getName();
        cout << "\nQuantity: " << temp->getQuantity();
        cout << "\nPrice: " << temp->getPrice();

        totalProducts++;
        totalQuantity += temp->getQuantity();
        totalValue += temp->getQuantity() * temp->getPrice();

        temp = temp->getNext();
    }

    file << "\nTotal Products: " << totalProducts;
    file << "\nTotal Quantity: " << totalQuantity;
    file << "\nTotal Inventory Value: " << totalValue;

    cout << "\n\nTotal Products: " << totalProducts;
    cout << "\nTotal Quantity: " << totalQuantity;
    cout << "\nTotal Inventory Value: " << totalValue;

    file.close();

    cout << "\n\nInventory report saved successfully!\n";
}

// ================= TRANSACTION REPORT =================

void WarehouseSystem::generateTransactionReport()
{
    ofstream file("transaction_report.txt");

    if (!file)
    {
        cout << "\nUnable to create transaction report!\n";
        return;
    }

    Transaction* temp = transactionHead;
    int totalTransactions = 0;

    file << "========== WIMS TRANSACTION REPORT ==========\n\n";

    cout << "\n========== TRANSACTION REPORT ==========\n";

    while (temp != NULL)
    {
        file << "Transaction ID: "
             << temp->getTransactionId() << endl;

        file << "Type: " << temp->getType() << endl;
        file << "Product ID: " << temp->getProductId() << endl;
        file << "Quantity: " << temp->getQuantity() << endl;
        file << "Date: " << temp->getDate() << endl;
        file << "-------------------------------------\n";

        cout << "\nTransaction ID: "
             << temp->getTransactionId();

        cout << "\nType: " << temp->getType();
        cout << "\nProduct ID: " << temp->getProductId();
        cout << "\nQuantity: " << temp->getQuantity();
        cout << "\nDate: " << temp->getDate();

        totalTransactions++;
        temp = temp->getNext();
    }

    file << "\nTotal Transactions: " << totalTransactions;
    cout << "\n\nTotal Transactions: " << totalTransactions;

    file.close();

    cout << "\n\nTransaction report saved successfully!\n";
}

// ================= ORDER REPORT =================

void WarehouseSystem::generateOrderReport()
{
    ofstream file("order_report.txt");

    if (!file)
    {
        cout << "\nUnable to create order report!\n";
        return;
    }

    Order* temp = orderFront;
    int totalOrders = 0;

    file << "========== WIMS ORDER REPORT ==========\n\n";

    cout << "\n========== ORDER REPORT ==========\n";

    while (temp != NULL)
    {
        file << "Order ID: " << temp->getOrderId() << endl;
        file << "Product ID: " << temp->getProductId() << endl;
        file << "Quantity: " << temp->getQuantity() << endl;
        file << "Status: " << temp->getStatus() << endl;
        file << "Date: " << temp->getDate() << endl;
        file << "-------------------------------------\n";

        cout << "\nOrder ID: " << temp->getOrderId();
        cout << "\nProduct ID: " << temp->getProductId();
        cout << "\nQuantity: " << temp->getQuantity();
        cout << "\nStatus: " << temp->getStatus();
        cout << "\nDate: " << temp->getDate();

        totalOrders++;
        temp = temp->getNext();
    }

    file << "\nTotal Pending Orders: " << totalOrders;
    cout << "\n\nTotal Pending Orders: " << totalOrders;

    file.close();

    cout << "\n\nOrder report saved successfully!\n";
}
