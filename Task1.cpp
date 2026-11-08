#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct HNode {
    char ch;
    long long freq;
    int order;
    HNode* left;
    HNode* right;

    HNode(char c, long long f, int o) {
        ch = c;
        freq = f;
        order = o;
        left = nullptr;
        right = nullptr;
    }
};

bool higher(HNode* a, HNode* b) {
    if (a->freq != b->freq)
        return a->freq < b->freq;

    return a->order < b->order;
}

class MinHeap {
    vector<HNode*> a;

    void up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;

            if (higher(a[i], a[p])) {
                swap(a[i], a[p]);
                i = p;
            }
            else
                break;
        }
    }

    void down(int i) {
        int n = a.size();

        while (true) {
            int l = 2 * i + 1;
            int r = l + 1;
            int m = i;

            if (l < n && higher(a[l], a[m]))
                m = l;

            if (r < n && higher(a[r], a[m]))
                m = r;

            if (m == i)
                break;

            swap(a[i], a[m]);
            i = m;
        }
    }

public:
    size_t size() const {
        return a.size();
    }

    void push(HNode* x) {
        a.push_back(x);
        up(a.size() - 1);
    }
    HNode* pop() {
        HNode* top = a[0];

        a[0] = a.back();
        a.pop_back();

        if (!a.empty())
            down(0);

        return top;
    }
};

void preorder(HNode* n, string code) {
    if (n == nullptr)
        return;

    if (n->left == nullptr && n->right == nullptr) {
        cout << n->ch << ": ";

        if (code.empty())
            cout << "0";
        else
            cout << code;

        cout << endl;
        return;
    }

    preorder(n->left, code + "0");
    preorder(n->right, code + "1");
}

void freeTree(HNode* n) {
    if (n == nullptr)
        return;

    freeTree(n->left);
    freeTree(n->right);

    delete n;
}

int main() {
    int n;

    cout << "Enter number of characters: ";
    cin >> n;

    string S;
    cout << "Enter characters: ";
    cin >> S;

    if (n <= 0 || S.length() != n) {
        cout << "Invalid input!";
        return 0;
    }
    vector<int> f(n);

    cout << "Enter frequencies: ";
    for (int i = 0; i < n; i++) {
        cin >> f[i];

        if (f[i] <= 0) {
            cout << "Invalid frequency!";
            return 0;
        }
    }

    MinHeap heap;

    for (int i = 0; i < n; i++) {
        heap.push(new HNode(S[i], f[i], i));
    }

    int counter = n;

    while (heap.size() > 1) {
        HNode* x = heap.pop();
        HNode* y = heap.pop();

        HNode* z = new HNode('#', x->freq + y->freq, counter++);

        z->left = x;
        z->right = y;

        heap.push(z);
    }
    HNode* root = heap.pop();
    cout << "\nHuffman Codes (Preorder):\n";

    preorder(root, "");

    freeTree(root);
    return 0;
}
