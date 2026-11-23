
#include "wims.h"

// ================= PRODUCT CONSTRUCTOR =================

Product::Product(int id, string name, int quantity,
                 double price, string expiry)
{
    this->id = id;
    this->name = name;
    this->quantity = quantity;
    this->price = price;
    this->expiry = expiry;
    next = NULL;
}

// ================= GETTER FUNCTIONS =================

int Product::getId()
{
    return id;
}

string Product::getName()
{
    return name;
}

int Product::getQuantity()
{
    return quantity;
}

double Product::getPrice()
{
    return price;
}

string Product::getExpiry()
{
    return expiry;
}

Product* Product::getNext()
{
    return next;
}

// ================= SETTER FUNCTIONS =================

void Product::setName(string name)
{
    this->name = name;
}

void Product::setQuantity(int quantity)
{
    this->quantity = quantity;
}

void Product::setPrice(double price)
{
    this->price = price;
}

void Product::setExpiry(string expiry)
{
    this->expiry = expiry;
}

// ================= ADD PRODUCT =================

void WarehouseSystem::addProduct(
    string name,
    int quantity,
    double price,
    string expiry)
{
    if (name.empty() || quantity < 0 || price < 0)
    {
        cout << "\nInvalid product details!\n";
        return;
    }

    Product* newProduct = new Product(
        nextProductId++,
        name,
        quantity,
        price,
        expiry
    );

    if (productHead == NULL)
    {
        productHead = newProduct;
    }
    else
    {
        Product* temp = productHead;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newProduct;
    }

    productIndex.insert(newProduct);

    addTransaction("PRODUCT ADDED",
                    newProduct->id,
                    quantity);

    cout << "\nProduct added successfully!\n";
    cout << "Product ID: " << newProduct->id << endl;
}

// ================= UPDATE PRODUCT =================

void WarehouseSystem::updateProduct(
    int id,
    string name,
    int quantity,
    double price,
    string expiry)
{
    if (quantity < 0 || price < 0 || name.empty())
    {
        cout << "\nInvalid product details!\n";
        return;
    }

    Product* product = productIndex.search(id);

    if (product == NULL)
    {
        cout << "\nProduct not found!\n";
        return;
    }

    int difference = quantity - product->quantity;

    product->setName(name);
    product->setQuantity(quantity);
    product->setPrice(price);
    product->setExpiry(expiry);

    if (difference != 0)
    {
        addTransaction(
            "STOCK ADJUSTMENT",
            id,
            difference > 0 ? difference : -difference
        );
    }

    cout << "\nProduct updated successfully!\n";
}

// ================= DELETE PRODUCT =================

void WarehouseSystem::deleteProduct(int id)
{
    Product* product = productIndex.search(id);

    if (product == NULL)
    {
        cout << "\nProduct not found!\n";
        return;
    }

    Product* temp = productHead;
    Product* previous = NULL;

    while (temp != NULL && temp->id != id)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "\nProduct not found!\n";
        return;
    }

    addTransaction("PRODUCT DELETED",
                   id,
                   temp->quantity);

    productIndex.remove(id);

    if (previous == NULL)
    {
        productHead = temp->next;
    }
    else
    {
        previous->next = temp->next;
    }

    delete temp;

    cout << "\nProduct deleted successfully!\n";
}

// ================= SEARCH PRODUCT =================

Product* WarehouseSystem::searchProduct(int id)
{
    return productIndex.search(id);
}

// ================= DISPLAY PRODUCTS =================

void WarehouseSystem::displayProducts()
{
    if (productHead == NULL)
    {
        cout << "\nNo products available!\n";
        return;
    }

    cout << "\n========== PRODUCT INVENTORY ==========\n";

    Product* temp = productHead;

    while (temp != NULL)
    {
        cout << "\nProduct ID : " << temp->id;
        cout << "\nName       : " << temp->name;
        cout << "\nQuantity   : " << temp->quantity;
        cout << "\nPrice      : " << temp->price;
        cout << "\nExpiry     : " << temp->expiry;

        cout << "\n---------------------------------------\n";

        temp = temp->next;
    }
}

// ================= SEARCH AND DISPLAY =================

void displayProductDetails(Product* product)
{
    if (product == NULL)
    {
        cout << "\nProduct not found!\n";
        return;
    }

    cout << "\n========== PRODUCT DETAILS ==========\n";

    cout << "Product ID : " << product->getId() << endl;
    cout << "Name       : " << product->getName() << endl;
    cout << "Quantity   : " << product->getQuantity() << endl;
    cout << "Price      : " << product->getPrice() << endl;
    cout << "Expiry     : " << product->getExpiry() << endl;
}
