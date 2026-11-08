
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;

    ListNode(int v) {
        val = v;
        next = nullptr;
    }
};

// Min Heap
class MinHeap {
    vector<ListNode*> a;

    void up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;

            if (a[i]->val < a[p]->val) {
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

            if (l < n && a[l]->val < a[m]->val)
                m = l;

            if (r < n && a[r]->val < a[m]->val)
                m = r;

            if (m == i)
                break;

            swap(a[i], a[m]);
            i = m;
        }
    }

public:
    bool empty() const {
        return a.empty();
    }

    void push(ListNode* x) {
        a.push_back(x);
        up(a.size() - 1);
    }

    ListNode* pop() {
        if (empty())
            return nullptr;

        ListNode* top = a[0];

        a[0] = a.back();
        a.pop_back();

        if (!empty())
            down(0);

        return top;
    }
};

// Merge K sorted linked lists
ListNode* mergeKLists(vector<ListNode*>& lists) {
    MinHeap heap;

    for (ListNode* head : lists) {
        if (head != nullptr)
            heap.push(head);
    }

    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!heap.empty()) {
        ListNode* smallest = heap.pop();

        tail->next = smallest;
        tail = smallest;

        if (smallest->next != nullptr)
            heap.push(smallest->next);
    }

    tail->next = nullptr;

    return dummy.next;
}

// Build linked list from user input
ListNode* build(int n) {
    ListNode dummy(0);
    ListNode* t = &dummy;

    int x, prev = 0;

    for (int i = 0; i < n; i++) {
        if (!(cin >> x)) {
            cout << "Invalid input! Enter integers only.\n";
            return nullptr;
        }

        // Check ascending order
        if (i > 0 && x < prev) {
            cout << "Invalid input! List must be sorted.\n";
            return nullptr;
        }

        t->next = new ListNode(x);
        t = t->next;

        prev = x;
    }

    return dummy.next;
}

// Print linked list
void print(ListNode* h) {
    if (h == nullptr) {
        cout << "Empty list";
    }

    while (h != nullptr) {
        cout << h->val;

        if (h->next != nullptr)
            cout << " -> ";

        h = h->next;
    }

    cout << endl;
}

// Delete linked list
void freeList(ListNode* h) {
    while (h != nullptr) {
        ListNode* t = h;
        h = h->next;
        delete t;
    }
}

int main() {
    int k;

    cout << "Enter number of linked lists: ";

    if (!(cin >> k) || k <= 0) {
        cout << "Invalid number of lists!";
        return 0;
    }

    vector<ListNode*> lists;
    vector<ListNode*> allocated;

    for (int i = 0; i < k; i++) {
        int n;

        cout << "\nEnter number of nodes in list "
            << i + 1 << ": ";

        if (!(cin >> n) || n < 0) {
            cout << "Invalid number of nodes!";
            for (ListNode* h : allocated)
                freeList(h);
            return 0;
        }

        cout << "Enter " << n << " sorted elements: ";

        ListNode* head = build(n);

        if (n > 0 && head == nullptr) {
            for (ListNode* h : allocated)
                freeList(h);
            return 0;
        }

        lists.push_back(head);
        allocated.push_back(head);
    }

    ListNode* merged = mergeKLists(lists);

    cout << "\nMerged sorted linked list:\n";
    print(merged);

    freeList(merged);

    return 0;
}
