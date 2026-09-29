#include <bits/stdc++.h>
#define ll long long
#define int ll
using namespace std;

struct BIT { // 1-indexed
    int n;
    vector<long long> b;

    BIT(int _n) {
        n = _n;
        while (n < _n) n <<= 1; // added to use lower bound
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

    // get first pref >= sum
    int lower_bound(int sum) {
        if(sum > b[n])
            return -1;

        int skip = 0;
        for (int step = n; step > 0; step >>= 1) {
            if(b[skip + step] < sum) {
                sum -= b[skip + step];
                skip += step;
            }
        }
        return skip + 1;
    }
    int operator[](int idx) {
        return lower_bound(idx);
    }
};


//Multiset using BIT
struct MultiSet {
    int n;
    vector<int> b;

    MultiSet(int _n) {
        n = 1;
        while (n < _n) n <<= 1;
        b.assign(n + 1, 0);
    }

    int get(int idx) {
        int ans = 0;
        while (idx > 0) {
            ans += b[idx];
            idx -= idx & -idx;
        }
        return ans;
    }

    int get(int l, int r) {
        return get(r) - get(l - 1);
    }

    void insert(int val, int cnt = 1) {
        while (val <= n) {
            b[val] += cnt;
            val += val & -val;
        }
    }
    void erase(int val, int cnt = 1) {
        int tot = get(val, val);
        cnt = min(cnt, tot);
        insert(val, -cnt);
    }
    int size() {
        return b[n];
    }

    // if sum >= tot array sum, it will return the next power of two + 1
    int lower_bound(int sum) {
        if(sum > b[n])
            return -1;

        int skip = 0;
        for (int step = n; step > 0; step >>= 1) {
            if(b[skip + step] < sum) {
                sum -= b[skip + step];
                skip += step;
            }
        }
        return skip + 1;
    }

    int operator[](int idx) {
        return lower_bound(idx);
    }
} ms(1e6);