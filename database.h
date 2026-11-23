
#ifndef DATABASE_H
#define DATABASE_H

#include "wims.h"

// SQLite forward declaration
struct sqlite3;

// ================= DATABASE CLASS =================

class Database
{
private:
    sqlite3* db;

public:

    // Constructor and Destructor
    Database();
    ~Database();

    // Database Connection
    bool connect(string filename);

    void close();

    // Table Creation
    bool createTables();

    // Save Data
    bool saveAll(WarehouseSystem& system);

    // Load Data
    bool loadAll(WarehouseSystem& system);

    // Connection Status
    bool isConnected();
};

#endif
