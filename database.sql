
-- =====================================
-- WIMS DATABASE SCHEMA
-- Warehouse Inventory Management System
-- =====================================

PRAGMA foreign_keys = ON;

-- ================= PRODUCTS TABLE =================

CREATE TABLE IF NOT EXISTS Products
(
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    quantity INTEGER NOT NULL CHECK(quantity >= 0),
    price REAL NOT NULL CHECK(price >= 0),
    expiry TEXT
);

-- ================= TRANSACTIONS TABLE =================

CREATE TABLE IF NOT EXISTS Transactions
(
    transaction_id INTEGER PRIMARY KEY,
    type TEXT NOT NULL,
    product_id INTEGER,
    quantity INTEGER,
    date TEXT
);

-- ================= ORDERS TABLE =================

CREATE TABLE IF NOT EXISTS Orders
(
    order_id INTEGER PRIMARY KEY,
    product_id INTEGER,
    quantity INTEGER,
    status TEXT,
    date TEXT
);

-- ================= INDEXES =================

CREATE INDEX IF NOT EXISTS idx_product_name
ON Products(name);

CREATE INDEX IF NOT EXISTS idx_transaction_product
ON Transactions(product_id);

CREATE INDEX IF NOT EXISTS idx_order_product
ON Orders(product_id);

-- ================= END =================
