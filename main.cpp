
#include "wims.h"

void runAPIServer(WarehouseSystem& system);

int main()
{
    WarehouseSystem system;

    cout << "====================================\n";
    cout << " WAREHOUSE INVENTORY MANAGEMENT\n";
    cout << "====================================\n";

    int choice = -1;

    do
    {
        cout << "\n========== WIMS MAIN MENU ==========\n";

        cout << "1. Add Product\n";
        cout << "2. Display Products\n";
        cout << "3. Search Product\n";
        cout << "4. Update Product\n";
        cout << "5. Delete Product\n";
        cout << "6. Stock In\n";
        cout << "7. Stock Out\n";
        cout << "8. Place Order\n";
        cout << "9. Process Next Order\n";
        cout << "10. Display Orders\n";
        cout << "11. Display Transactions\n";
        cout << "12. Generate Inventory Report\n";
        cout << "13. Generate Transaction Report\n";
        cout << "14. Generate Order Report\n";
        cout << "15. Start API Server\n";
        cout << "0. Exit\n";

        cout << "====================================\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid input! Enter a number.\n";
            continue;
        }

        int id, quantity;
        double price;
        string name, expiry;

        switch (choice)
        {
            case 1:
                cout << "Enter Product Name: ";
                getline(cin >> ws, name);

                cout << "Enter Quantity: ";
                cin >> quantity;

                cout << "Enter Price: ";
                cin >> price;

                cout << "Enter Expiry Date: ";
                getline(cin >> ws, expiry);

                system.addProduct(name, quantity, price, expiry);
                break;

            case 2:
                system.displayProducts();
                break;

            case 3:
            {
                cout << "Enter Product ID: ";
                cin >> id;

                Product* product = system.searchProduct(id);

                if (product != NULL)
                {
                    cout << "\nProduct Found!\n";
                    cout << "ID: " << product->getId();
                    cout << "\nName: " << product->getName();
                    cout << "\nQuantity: " << product->getQuantity();
                    cout << "\nPrice: " << product->getPrice();
                    cout << "\nExpiry: " << product->getExpiry() << endl;
                }
                else
                {
                    cout << "\nProduct not found!\n";
                }
                break;
            }

            case 4:
                cout << "Enter Product ID: ";
                cin >> id;

                cout << "Enter New Name: ";
                getline(cin >> ws, name);

                cout << "Enter New Quantity: ";
                cin >> quantity;

                cout << "Enter New Price: ";
                cin >> price;

                cout << "Enter New Expiry Date: ";
                getline(cin >> ws, expiry);

                system.updateProduct(id, name, quantity, price, expiry);
                break;

            case 5:
                cout << "Enter Product ID: ";
                cin >> id;

                system.deleteProduct(id);
                break;

            case 6:
                cout << "Enter Product ID: ";
                cin >> id;

                cout << "Enter Quantity to Add: ";
                cin >> quantity;

                system.stockIn(id, quantity);
                break;

            case 7:
                cout << "Enter Product ID: ";
                cin >> id;

                cout << "Enter Quantity to Remove: ";
                cin >> quantity;

                system.stockOut(id, quantity);
                break;

            case 8:
                cout << "Enter Product ID: ";
                cin >> id;

                cout << "Enter Order Quantity: ";
                cin >> quantity;

                system.placeOrder(id, quantity);
                break;

            case 9:
                system.processNextOrder();
                break;

            case 10:
                system.displayOrders();
                break;

            case 11:
                system.displayTransactions();
                break;

            case 12:
                system.generateInventoryReport();
                break;

            case 13:
                system.generateTransactionReport();
                break;

            case 14:
                system.generateOrderReport();
                break;

            case 15:
                runAPIServer(system);
                break;

            case 0:
                cout << "\nExiting WIMS...\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }

    } while (choice != 0);

    cout << "\nThank you for using WIMS!\n";

    return 0;
}
