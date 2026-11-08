#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;

enum Criterion { MAXIMUM, MINIMUM, SUM };

struct Node {
    int key, cnt, h;
    Node* l;
    Node* r;

    Node(int k) {
        key = k;
        cnt = 1;
        h = 1;
        l = nullptr;
        r = nullptr;
    }
};

class AVLTree {
    Node* root = nullptr;

    int height(Node* n) {
        return n ? n->h : 0;
    }

    int balance(Node* n) {
        return n ? height(n->l) - height(n->r) : 0;
    }

    void update(Node* n) {
        n->h = 1 + max(height(n->l), height(n->r));
    }

    Node* rotateRight(Node* y) {
        Node* x = y->l;
        y->l = x->r;
        x->r = y;

        update(y);
        update(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->r;
        x->r = y->l;
        y->l = x;

        update(x);
        update(y);

        return y;
    }

    Node* rebalance(Node* n) {
        update(n);

        int bf = balance(n);

        if (bf > 1) {
            if (balance(n->l) < 0) {
                cout << "   Left-Right rotation at "
                    << n->key << endl;
                n->l = rotateLeft(n->l);
            }
            else {
                cout << "   Left-Left rotation at "
                    << n->key << endl;
            }

            return rotateRight(n);
        }

        if (bf < -1) {
            if (balance(n->r) > 0) {
                cout << "   Right-Left rotation at "
                    << n->key << endl;
                n->r = rotateRight(n->r);
            }
            else {
                cout << "   Right-Right rotation at "
                    << n->key << endl;
            }

            return rotateLeft(n);
        }

        return n;
    }

    Node* insert(Node* n, int key) {
        if (!n)
            return new Node(key);

        if (key < n->key)
            n->l = insert(n->l, key);

        else if (key > n->key)
            n->r = insert(n->r, key);
        else {
            n->cnt++;
            return n;
        }

        return rebalance(n);
    }

    Node* remove(Node* n, int key) {
        if (!n)
            return nullptr;

        if (key < n->key)
            n->l = remove(n->l, key);

        else if (key > n->key)
            n->r = remove(n->r, key);

        else {
            if (n->cnt > 1) {
                n->cnt--;
                return n;
            }

            if (!n->l || !n->r) {
                Node* child = n->l ? n->l : n->r;
                delete n;
                return child;
            }

            Node* s = n->r;

            while (s->l)
                s = s->l;

            n->key = s->key;
            n->cnt = s->cnt;

            s->cnt = 1;

            n->r = remove(n->r, s->key);
        }

        return rebalance(n);
    }

    void largest(Node* n, int k, vector<int>& out) {
        if (!n || (int)out.size() >= k)
            return;

        largest(n->r, k, out);

        for (int i = 0; i < n->cnt && (int)out.size() < k; i++)
            out.push_back(n->key);

        largest(n->l, k, out);
    }

    void smallest(Node* n, int k, vector<int>& out) {
        if (!n || (int)out.size() >= k)
            return;

        smallest(n->l, k, out);

        for (int i = 0; i < n->cnt && (int)out.size() < k; i++)
            out.push_back(n->key);

        smallest(n->r, k, out);
    }

    void print(Node* n, int depth) {
        if (!n)
            return;

        print(n->r, depth + 1);

        cout << string(depth * 6, ' ') << n->key;

        if (n->cnt > 1)
            cout << "(x" << n->cnt << ")";

        cout << " [h=" << n->h
            << ", bf=" << balance(n) << "]\n";

        print(n->l, depth + 1);
    }

    int countNodes(Node* n) {
        if (!n)
            return 0;

        return n->cnt + countNodes(n->l) + countNodes(n->r);
    }

    void destroy(Node* n) {
        if (!n)
            return;

        destroy(n->l);
        destroy(n->r);
        delete n;
    }

public:
    ~AVLTree() {
        destroy(root);
    }

    int size() {
        return countNodes(root);
    }

    void printTree() {
        if (!root)
            cout << "(Empty tree)\n";
        else
            print(root, 0);

        cout << endl;
    }

    void insert(int key) {
        cout << "\nInserting " << key << ":\n";

        root = insert(root, key);

        cout << "Tree after insertion:\n";
        printTree();
    }

    vector<int> topK(Criterion c, int k) {
        vector<int> out;

        if (c == MINIMUM)
            smallest(root, k, out);
        else
            largest(root, k, out);

        return out;
    }

    void queryTopK(Criterion c, int k) {
        vector<int> res = topK(c, k);

        cout << "\nTop " << k << " elements: ";

        for (int x : res)
            cout << x << " ";

        if (c == SUM) {
            long long sum = 0;

            for (int x : res)
                sum += x;

            cout << "\nSum = " << sum;
        }

        cout << endl;
    }

    void deleteTopK(Criterion c, int k) {
        vector<int> victims = topK(c, k);

        if (victims.empty()) {
            cout << "Tree is empty!\n";
            return;
        }

        cout << "\nElements to delete: ";

        for (int x : victims)
            cout << x << " ";

        cout << endl;

        for (int x : victims) {
            cout << "\nDeleting " << x << ":\n";

            root = remove(root, x);

            cout << "Tree after deletion:\n";
            printTree();
        }
    }
};

// Read integer safely
bool readInt(int& x) {
    if (cin >> x)
        return true;

    cout << "Invalid input! Please enter an integer.\n";
    return false;
}

// Select criterion
bool getCriterion(Criterion& c) {
    int choice;

    cout << "\nSelect criterion:\n";
    cout << "1. MAXIMUM (k largest)\n";
    cout << "2. MINIMUM (k smallest)\n";
    cout << "3. SUM (sum of k largest)\n";
    cout << "Enter choice: ";

    if (!readInt(choice))
        return false;

    if (choice < 1 || choice > 3) {
        cout << "Invalid criterion!\n";
        return false;
    }

    c = (Criterion)(choice - 1);

    return true;
}

int main() {
    AVLTree t;

    int choice;

    while (true) {
        cout << "\n========== AVL TREE MENU ==========\n";
        cout << "1. Insert multiple elements\n";
        cout << "2. Query top K elements\n";
        cout << "3. Delete top K elements\n";
        cout << "4. Display tree\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        if (!readInt(choice))
            return 0;

        if (choice == 1) {
            int n;

            cout << "How many elements do you want to insert? ";

            if (!readInt(n))
                return 0;

            if (n <= 0 || n > 100000) {
                cout << "Invalid number of elements!\n";
                continue;
            }

            cout << "Enter " << n << " elements:\n";

            vector<int> values(n);

            bool valid = true;

            for (int i = 0; i < n; i++) {
                if (!readInt(values[i])) {
                    valid = false;
                    break;
                }
            }

            if (!valid)
                return 0;

            for (int x : values)
                t.insert(x);
        }

        else if (choice == 2 || choice == 3) {
            if (t.size() == 0) {
                cout << "Tree is empty! Insert elements first.\n";
                continue;
            }

            Criterion c;

            if (!getCriterion(c))
                continue;

            int k;

            cout << "Enter value of K: ";

            if (!readInt(k))
                return 0;

            if (k <= 0) {
                cout << "K must be greater than zero!\n";
                continue;
            }

            if (k > t.size()) {
                cout << "K cannot exceed the total number of "
                    << "elements (" << t.size() << ").\n";
                continue;
            }

            if (choice == 2)
                t.queryTopK(c, k);

            else
                t.deleteTopK(c, k);
        }

        else if (choice == 4) {
            cout << "\nCurrent AVL Tree:\n";
            t.printTree();
        }

        else if (choice == 5) {
            cout << "Program terminated.\n";
            break;
        }

        else {
            cout << "Invalid menu choice! Try again.\n";
        }
    }

    return 0;
}
