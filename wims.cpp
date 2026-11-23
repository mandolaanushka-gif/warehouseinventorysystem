
#include "wims.h"

// ================= AVL TREE IMPLEMENTATION =================

AVLNode::AVLNode(Product* product)
{
    this->product = product;
    left = NULL;
    right = NULL;
    height = 1;
}

AVLTree::AVLTree()
{
    root = NULL;
}

static void deleteAVLNodes(AVLNode* node)
{
    if (node == NULL)
        return;

    deleteAVLNodes(node->left);
    deleteAVLNodes(node->right);
    delete node;
}

AVLTree::~AVLTree()
{
    deleteAVLNodes(root);
    root = NULL;
}

int AVLTree::getHeight(AVLNode* node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

int AVLTree::getBalance(AVLNode* node)
{
    if (node == NULL)
        return 0;

    return getHeight(node->left) -
           getHeight(node->right);
}

AVLNode* AVLTree::rotateRight(AVLNode* y)
{
    AVLNode* x = y->left;
    AVLNode* temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + max(getHeight(y->left),
                        getHeight(y->right));

    x->height = 1 + max(getHeight(x->left),
                        getHeight(x->right));

    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x)
{
    AVLNode* y = x->right;
    AVLNode* temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = 1 + max(getHeight(x->left),
                        getHeight(x->right));

    y->height = 1 + max(getHeight(y->left),
                        getHeight(y->right));

    return y;
}

AVLNode* AVLTree::insertNode(AVLNode* node,
                             Product* product)
{
    if (node == NULL)
        return new AVLNode(product);

    if (product->getId() < node->product->getId())
        node->left = insertNode(node->left, product);

    else if (product->getId() > node->product->getId())
        node->right = insertNode(node->right, product);

    else
        return node;

    node->height = 1 + max(getHeight(node->left),
                           getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 &&
        product->getId() < node->left->product->getId())
        return rotateRight(node);

    if (balance < -1 &&
        product->getId() > node->right->product->getId())
        return rotateLeft(node);

    if (balance > 1 &&
        product->getId() > node->left->product->getId())
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 &&
        product->getId() < node->right->product->getId())
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLTree::insert(Product* product)
{
    root = insertNode(root, product);
}

Product* AVLTree::searchNode(AVLNode* node, int id)
{
    if (node == NULL)
        return NULL;

    if (node->product->getId() == id)
        return node->product;

    if (id < node->product->getId())
        return searchNode(node->left, id);

    return searchNode(node->right, id);
}

Product* AVLTree::search(int id)
{
    return searchNode(root, id);
}

AVLNode* AVLTree::findMin(AVLNode* node)
{
    while (node != NULL && node->left != NULL)
        node = node->left;

    return node;
}

AVLNode* AVLTree::deleteNode(AVLNode* node, int id)
{
    if (node == NULL)
        return NULL;

    if (id < node->product->getId())
        node->left = deleteNode(node->left, id);

    else if (id > node->product->getId())
        node->right = deleteNode(node->right, id);

    else
    {
        if (node->left == NULL || node->right == NULL)
        {
            AVLNode* temp = node->left ?
                            node->left : node->right;

            delete node;
            return temp;
        }

        AVLNode* temp = findMin(node->right);

        node->product = temp->product;

        node->right = deleteNode(
            node->right,
            temp->product->getId()
        );
    }

    node->height = 1 + max(getHeight(node->left),
                           getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLTree::remove(int id)
{
    root = deleteNode(root, id);
}

void AVLTree::displayInOrder(AVLNode* node)
{
    if (node == NULL)
        return;

    displayInOrder(node->left);

    cout << "ID: " << node->product->getId()
         << " | Name: " << node->product->getName()
         << endl;

    displayInOrder(node->right);
}

void AVLTree::display()
{
    displayInOrder(root);
}

// ================= WAREHOUSE SYSTEM =================

WarehouseSystem::WarehouseSystem()
{
    productHead = NULL;
    transactionHead = NULL;

    orderFront = NULL;
    orderRear = NULL;

    nextProductId = 101;
    nextTransactionId = 1;
    nextOrderId = 1;

    // Preloaded Sample Inventory
    addProduct("Rice", 100, 50, "2027-12-30");
    addProduct("Wheat", 80, 40, "2027-11-20");
    addProduct("Sugar", 60, 45, "2027-10-15");
    addProduct("Oil", 50, 120, "2027-09-25");
    addProduct("Salt", 90, 20, "2028-01-15");
}

WarehouseSystem::~WarehouseSystem()
{
    Product* p = productHead;

    while (p != NULL)
    {
        Product* temp = p;
        p = p->next;
        delete temp;
    }

    Transaction* t = transactionHead;

    while (t != NULL)
    {
        Transaction* temp = t;
        t = t->next;
        delete temp;
    }

    Order* o = orderFront;

    while (o != NULL)
    {
        Order* temp = o;
        o = o->next;
        delete temp;
    }
}

// ================= TRANSACTION RECORD =================

void WarehouseSystem::addTransaction(
    string type, int productId, int quantity)
{
    Transaction* newTransaction =
        new Transaction(
            nextTransactionId++,
            type,
            productId,
            quantity,
            getCurrentDate()
        );

    newTransaction->next = transactionHead;
    transactionHead = newTransaction;
}

// ================= ACCESSOR FUNCTIONS =================

Product* WarehouseSystem::getProductHead()
{
    return productHead;
}

Transaction* WarehouseSystem::getTransactionHead()
{
    return transactionHead;
}

Order* WarehouseSystem::getOrderFront()
{
    return orderFront;
}
