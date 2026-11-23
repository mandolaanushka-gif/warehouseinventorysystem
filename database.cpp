
#include "database.h"
#include <sqlite3.h>

// ================= CONSTRUCTOR =================

Database::Database()
{
    db = NULL;
}

// ================= DESTRUCTOR =================

Database::~Database()
{
    close();
}

// ================= CONNECT =================

bool Database::connect(string filename)
{
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
    {
        cout << "\nDatabase connection failed!\n";

        if (db != NULL)
            cout << sqlite3_errmsg(db) << endl;

        close();
        return false;
    }

    sqlite3_exec(db, "PRAGMA foreign_keys = ON;",
                 NULL, NULL, NULL);

    cout << "\nDatabase connected successfully!\n";
    return true;
}

// ================= CLOSE =================

void Database::close()
{
    if (db != NULL)
    {
        sqlite3_close(db);
        db = NULL;
    }
}

// ================= STATUS =================

bool Database::isConnected()
{
    return db != NULL;
}

// ================= CREATE TABLES =================

bool Database::createTables()
{
    if (!isConnected())
        return false;

    const char* sql =
        "CREATE TABLE IF NOT EXISTS Products ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "quantity INTEGER NOT NULL CHECK(quantity >= 0),"
        "price REAL NOT NULL CHECK(price >= 0),"
        "expiry TEXT);"

        "CREATE TABLE IF NOT EXISTS Transactions ("
        "transaction_id INTEGER PRIMARY KEY,"
        "type TEXT NOT NULL,"
        "product_id INTEGER,"
        "quantity INTEGER,"
        "date TEXT);"

        "CREATE TABLE IF NOT EXISTS Orders ("
        "order_id INTEGER PRIMARY KEY,"
        "product_id INTEGER,"
        "quantity INTEGER,"
        "status TEXT,"
        "date TEXT);"

        "CREATE INDEX IF NOT EXISTS idx_product_name "
        "ON Products(name);"

        "CREATE INDEX IF NOT EXISTS idx_transaction_product "
        "ON Transactions(product_id);"

        "CREATE INDEX IF NOT EXISTS idx_order_product "
        "ON Orders(product_id);";

    char* errorMessage = NULL;

    if (sqlite3_exec(db, sql, NULL, NULL,
                     &errorMessage) != SQLITE_OK)
    {
        cout << "\nTable creation error: "
             << errorMessage << endl;

        sqlite3_free(errorMessage);
        return false;
    }

    cout << "\nDatabase tables created successfully!\n";
    return true;
}

// ================= SAVE ALL DATA =================

bool Database::saveAll(WarehouseSystem& system)
{
    const char* transactionSQL = nullptr;
const char* orderSQL = nullptr;
    if (!isConnected())
        return false;

    char* errorMessage = NULL;

    if (sqlite3_exec(db, "BEGIN TRANSACTION;",
                     NULL, NULL, &errorMessage) != SQLITE_OK)
    {
        if (errorMessage != NULL)
            sqlite3_free(errorMessage);

        return false;
    }

    const char* clearSQL =
        "DELETE FROM Products;"
        "DELETE FROM Transactions;"
        "DELETE FROM Orders;";

    if (sqlite3_exec(db, clearSQL, NULL, NULL,
                     &errorMessage) != SQLITE_OK)
    {
        cout << sqlite3_errmsg(db) << endl;

        if (errorMessage != NULL)
            sqlite3_free(errorMessage);

        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return false;
    }

    sqlite3_stmt* statement = NULL;

    // SAVE PRODUCTS

    const char* productSQL =
        "INSERT INTO Products "
        "(id, name, quantity, price, expiry) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, productSQL, -1,
                           &statement, NULL) != SQLITE_OK)
        goto save_error;

    for (Product* p = system.productHead;
         p != NULL; p = p->next)
    {
        sqlite3_bind_int(statement, 1, p->id);

        sqlite3_bind_text(statement, 2,
            p->name.c_str(), -1, SQLITE_TRANSIENT);

        sqlite3_bind_int(statement, 3, p->quantity);

        sqlite3_bind_double(statement, 4, p->price);

        sqlite3_bind_text(statement, 5,
            p->expiry.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(statement) != SQLITE_DONE)
            goto save_error;

        sqlite3_reset(statement);
        sqlite3_clear_bindings(statement);
    }

    sqlite3_finalize(statement);
    statement = NULL;

    // SAVE TRANSACTIONS

    transactionSQL =
        "INSERT INTO Transactions "
        "(transaction_id, type, product_id, quantity, date) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, transactionSQL, -1,
                           &statement, NULL) != SQLITE_OK)
        goto save_error;

    for (Transaction* t = system.transactionHead;
         t != NULL; t = t->next)
    {
        sqlite3_bind_int(statement, 1, t->transactionId);

        sqlite3_bind_text(statement, 2,
            t->type.c_str(), -1, SQLITE_TRANSIENT);

        sqlite3_bind_int(statement, 3, t->productId);

        sqlite3_bind_int(statement, 4, t->quantity);

        sqlite3_bind_text(statement, 5,
            t->date.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(statement) != SQLITE_DONE)
            goto save_error;

        sqlite3_reset(statement);
        sqlite3_clear_bindings(statement);
    }

    sqlite3_finalize(statement);
    statement = NULL;

    // SAVE ORDERS

    orderSQL =
        "INSERT INTO Orders "
        "(order_id, product_id, quantity, status, date) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, orderSQL, -1,
                           &statement, NULL) != SQLITE_OK)
        goto save_error;

    for (Order* o = system.orderFront;
         o != NULL; o = o->next)
    {
        sqlite3_bind_int(statement, 1, o->orderId);

        sqlite3_bind_int(statement, 2, o->productId);

        sqlite3_bind_int(statement, 3, o->quantity);

        sqlite3_bind_text(statement, 4,
            o->status.c_str(), -1, SQLITE_TRANSIENT);

        sqlite3_bind_text(statement, 5,
            o->date.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(statement) != SQLITE_DONE)
            goto save_error;

        sqlite3_reset(statement);
        sqlite3_clear_bindings(statement);
    }

    sqlite3_finalize(statement);
    statement = NULL;

    if (sqlite3_exec(db, "COMMIT;", NULL, NULL,
                     &errorMessage) != SQLITE_OK)
    {
        cout << sqlite3_errmsg(db) << endl;

        if (errorMessage != NULL)
            sqlite3_free(errorMessage);

        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return false;
    }

    cout << "\nAll data saved successfully!\n";
    return true;

save_error:

    cout << "\nDatabase save error: "
         << sqlite3_errmsg(db) << endl;

    if (statement != NULL)
        sqlite3_finalize(statement);

    sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);

    return false;
}

// ================= LOAD ALL DATA =================

bool Database::loadAll(WarehouseSystem& system)
{
    if (!isConnected())
        return false;

    // CLEAR OLD PRODUCTS AND AVL TREE

    Product* p = system.productHead;

    while (p != NULL)
    {
        Product* next = p->next;

        system.productIndex.remove(p->id);

        delete p;
        p = next;
    }

    system.productHead = NULL;

    // CLEAR TRANSACTIONS

    Transaction* t = system.transactionHead;

    while (t != NULL)
    {
        Transaction* next = t->next;
        delete t;
        t = next;
    }

    system.transactionHead = NULL;

    // CLEAR ORDERS

    Order* o = system.orderFront;

    while (o != NULL)
    {
        Order* next = o->next;
        delete o;
        o = next;
    }

    system.orderFront = NULL;
    system.orderRear = NULL;

    system.nextProductId = 101;
    system.nextTransactionId = 1;
    system.nextOrderId = 1;

    sqlite3_stmt* statement = NULL;

    // LOAD PRODUCTS

    const char* productSQL =
        "SELECT id, name, quantity, price, expiry "
        "FROM Products ORDER BY id ASC;";

    if (sqlite3_prepare_v2(db, productSQL, -1,
                           &statement, NULL) != SQLITE_OK)
        return false;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(statement, 0);

        const char* name =
            (const char*)sqlite3_column_text(statement, 1);

        int quantity = sqlite3_column_int(statement, 2);

        double price = sqlite3_column_double(statement, 3);

        const char* expiry =
            (const char*)sqlite3_column_text(statement, 4);

        Product* newProduct = new Product(
            id,
            name ? name : "",
            quantity,
            price,
            expiry ? expiry : ""
        );

        if (system.productHead == NULL)
        {
            system.productHead = newProduct;
        }
        else
        {
            Product* temp = system.productHead;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newProduct;
        }

        system.productIndex.insert(newProduct);

        if (id >= system.nextProductId)
            system.nextProductId = id + 1;
    }

    sqlite3_finalize(statement);
    statement = NULL;

    // LOAD TRANSACTIONS

    const char* transactionSQL =
        "SELECT transaction_id, type, product_id, quantity, date "
        "FROM Transactions ORDER BY transaction_id DESC;";

    if (sqlite3_prepare_v2(db, transactionSQL, -1,
                           &statement, NULL) != SQLITE_OK)
        return false;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(statement, 0);

        const char* type =
            (const char*)sqlite3_column_text(statement, 1);

        int productId = sqlite3_column_int(statement, 2);

        int quantity = sqlite3_column_int(statement, 3);

        const char* date =
            (const char*)sqlite3_column_text(statement, 4);

        Transaction* newTransaction = new Transaction(
            id,
            type ? type : "",
            productId,
            quantity,
            date ? date : ""
        );

        if (system.transactionHead == NULL)
        {
            system.transactionHead = newTransaction;
        }
        else
        {
            Transaction* temp = system.transactionHead;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newTransaction;
        }

        if (id >= system.nextTransactionId)
            system.nextTransactionId = id + 1;
    }

    sqlite3_finalize(statement);
    statement = NULL;

    // LOAD ORDERS

    const char* orderSQL =
        "SELECT order_id, product_id, quantity, status, date "
        "FROM Orders ORDER BY order_id ASC;";

    if (sqlite3_prepare_v2(db, orderSQL, -1,
                           &statement, NULL) != SQLITE_OK)
        return false;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(statement, 0);

        int productId = sqlite3_column_int(statement, 1);

        int quantity = sqlite3_column_int(statement, 2);

        const char* status =
            (const char*)sqlite3_column_text(statement, 3);

        const char* date =
            (const char*)sqlite3_column_text(statement, 4);

        Order* newOrder = new Order(
            id,
            productId,
            quantity,
            status ? status : "",
            date ? date : ""
        );

        if (system.orderRear == NULL)
        {
            system.orderFront = newOrder;
            system.orderRear = newOrder;
        }
        else
        {
            system.orderRear->next = newOrder;
            system.orderRear = newOrder;
        }

        if (id >= system.nextOrderId)
            system.nextOrderId = id + 1;
    }

    sqlite3_finalize(statement);

    cout << "\nAll data loaded successfully!\n";

    return true;
}
