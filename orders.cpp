
#include "wims.h"

// ================= ORDER CONSTRUCTOR =================

Order::Order(int orderId, int productId,
             int quantity, string status, string date)
{
    this->orderId = orderId;
    this->productId = productId;
    this->quantity = quantity;
    this->status = status;
    this->date = date;

    next = NULL;
}

// ================= GETTER FUNCTIONS =================

int Order::getOrderId()
{
    return orderId;
}

int Order::getProductId()
{
    return productId;
}

int Order::getQuantity()
{
    return quantity;
}

string Order::getStatus()
{
    return status;
}

string Order::getDate()
{
    return date;
}

Order* Order::getNext()
{
    return next;
}

// ================= SETTER FUNCTION =================

void Order::setStatus(string status)
{
    this->status = status;
}

// ================= PLACE ORDER =================

void WarehouseSystem::placeOrder(int productId, int quantity)
{
    if (quantity <= 0)
    {
        cout << "\nInvalid order quantity!\n";
        return;
    }

    Product* product = searchProduct(productId);

    if (product == NULL)
    {
        cout << "\nProduct not found!\n";
        return;
    }

    if (product->getQuantity() < quantity)
    {
        cout << "\nInsufficient stock for this order!\n";
        return;
    }

    Order* newOrder = new Order(
        nextOrderId++,
        productId,
        quantity,
        "Pending",
        getCurrentDate()
    );

    // Enqueue operation

    if (orderRear == NULL)
    {
        orderFront = newOrder;
        orderRear = newOrder;
    }
    else
    {
        orderRear->next = newOrder;
        orderRear = newOrder;
    }

    cout << "\nOrder placed successfully!\n";
    cout << "Order ID: " << newOrder->getOrderId() << endl;
    cout << "Product: " << product->getName() << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Status: Pending\n";
}

// ================= PROCESS NEXT ORDER =================

void WarehouseSystem::processNextOrder()
{
    if (orderFront == NULL)
    {
        cout << "\nNo pending orders available!\n";
        return;
    }

    // Dequeue operation

    Order* currentOrder = orderFront;

    orderFront = orderFront->next;

    if (orderFront == NULL)
    {
        orderRear = NULL;
    }

    Product* product = searchProduct(
        currentOrder->getProductId()
    );

    if (product == NULL)
    {
        currentOrder->setStatus("Rejected");

        cout << "\nOrder rejected: Product not found!\n";
    }
    else if (product->getQuantity() <
             currentOrder->getQuantity())
    {
        currentOrder->setStatus("Rejected");

        cout << "\nOrder rejected: Insufficient stock!\n";
    }
    else
    {
        product->setQuantity(
            product->getQuantity() -
            currentOrder->getQuantity()
        );

        currentOrder->setStatus("Completed");

        addTransaction(
            "ORDER PROCESSED",
            currentOrder->getProductId(),
            currentOrder->getQuantity()
        );

        cout << "\nOrder processed successfully!\n";
        cout << "Order ID: "
             << currentOrder->getOrderId() << endl;

        cout << "Remaining Stock: "
             << product->getQuantity() << endl;
    }

    delete currentOrder;
}

// ================= DISPLAY ORDERS =================

void WarehouseSystem::displayOrders()
{
    if (orderFront == NULL)
    {
        cout << "\nNo pending orders available!\n";
        return;
    }

    Order* temp = orderFront;

    cout << "\n========== PENDING ORDERS ==========\n";

    while (temp != NULL)
    {
        cout << "\nOrder ID: "
             << temp->getOrderId();

        cout << "\nProduct ID: "
             << temp->getProductId();

        cout << "\nQuantity: "
             << temp->getQuantity();

        cout << "\nStatus: "
             << temp->getStatus();

        cout << "\nDate: "
             << temp->getDate();

        cout << "\n------------------------------------\n";

        temp = temp->getNext();
    }
}
