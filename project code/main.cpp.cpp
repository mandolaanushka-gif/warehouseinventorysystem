#include "wims.h"

int main() {
    int choice;

    do {
        cout << "\n===== WAREHOUSE INVENTORY SYSTEM =====\n";
        cout << "1. Add product\n";
        cout << "2. Show all products\n";
        cout << "3. Search product\n";
        cout << "4. Update product\n";
        cout << "5. Stock in\n";
        cout << "6. Stock out\n";
        cout << "7. Add order\n";
        cout << "8. Process next order\n";
        cout << "9. Show pending orders\n";
        cout << "10. Transaction history\n";
        cout << "11. Low stock check\n";
        cout << "12. Expiry check\n";
        cout << "0. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addProduct();
                break;

            case 2:
                showProducts();
                break;

            case 3:
                searchProduct();
                break;

            case 4:
                updateProduct();
                break;

            case 5:
                stockIn();
                break;

            case 6:
                stockOutMenu();
                break;

            case 7:
                enqueueOrder();
                break;

            case 8:
                processOrder();
                break;

            case 9:
                showOrders();
                break;

            case 10:
                showTransactions();
                break;

            case 11:
                checkLowStock();
                break;

            case 12:
                checkExpiry();
                break;

            case 0:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}