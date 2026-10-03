#include <bits\stdc++.h>

using namespace std;

struct Node {
    long long sum;
    Node () : sum(0){}
    Node (int x) : sum(x){}
};

struct SegTree {
    int tree_size;
    vector<Node> SegData;
    SegTree(int n) {
        tree_size = 1;
        while (tree_size < n) tree_size <<= 1;
        SegData.assign(2 * tree_size, Node());
    }

    Node merge(const Node &lf, const Node &ri) {
        Node ans = Node();
        ans.sum = lf.sum + ri.sum;
        return ans;
    }

    void build(const vector<int> &arr, int node, int lx, int rx) {
        if(rx - lx == 1) {
            if(lx < arr.size())
                SegData[node] = Node(arr[lx]);
            return;
        }
        int mid = (lx + rx) >> 1;
        build(arr, 2 * node + 1, lx, mid);
        build(arr, 2 * node + 2, mid, rx);
        SegData[node] = merge(SegData[2 * node + 1], SegData[2 * node + 2]);
    }
    void build(const vector<int> &arr) {
        build(arr, 0, 0, tree_size);
    }

    void set(int idx, int val, int node, int lx, int rx) {
        if(rx - lx == 1) {
            SegData[node] = Node(val);
            return;
        }

        int mid = (lx + rx) >> 1;
        if(idx < mid)
            set(idx, val, 2 * node + 1, lx, mid);
        else
            set(idx, val, 2 * node + 2, mid, rx);
        SegData[node] = merge(SegData[2 * node + 1], SegData[2 * node + 2]);
    }
    void set(int idx, int val) {
        set(idx, val, 0, 0, tree_size);
    }

    Node get_range(int l, int r, int node, int lx, int rx) {
        if(lx >= r || rx <= l)
            return Node();
        if(lx >= l && rx <= r)
            return SegData[node];

        int mid = (lx + rx) >> 1;
        Node lf = get_range(l, r, 2 * node + 1, lx, mid);
        Node ri = get_range(l, r, 2 * node + 2, mid, rx);
        return merge(lf, ri);
    }
    long long get_range(int l, int r) { // R not included
        return get_range(l, r, 0, 0, tree_size).sum;
    }

    int find_first(int l, int r, int x, int node, int lx, int rx) {
        if(lx >= r || rx <= l)
            return -1;
        if(SegData[node].sum < x)
            return -1;
        if(rx - lx == 1)
            return lx;

        int mid = (lx + rx) >> 1;
        int ans = find_first(l, r, x, 2 * node + 1, lx, mid);
        if(ans == -1)
            ans = find_first(l, r, x, 2 * node + 2, mid, rx);
        return ans;
    }
    int find_first(int l, int r, int x) {
        return find_first(l, r, x, 0, 0, tree_size);
    }
};
