#include <bits/stdc++.h>
#define ll long long
#define int ll
using namespace std;

struct BIT { // 1-indexed
    int n;
    vector<long long> b;

    BIT(int _n) {
        n = _n;
        b.assign(n + 1, 0);
    }

    void add(int idx, int v) { // arr[idx] += v
        while (idx <= n) {
            b[idx] += v;
            idx += idx & -idx;
        }
    }

    long long get(int idx) { // from 1 to idx
        long long ans = 0;
        while (idx > 0) {
            ans += b[idx];
            idx -= idx & -idx;
        }
        return ans;
    }

    long long get(int l, int r) { // from l ro r
        return get(r) - get(l - 1);
    }

    void set(int idx, int v) { // arr[idx] = v
        int old = get(idx, idx);
        cout << old << '\n';
        add(idx, -old + v);
    }
};