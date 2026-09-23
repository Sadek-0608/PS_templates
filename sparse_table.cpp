#include <bits/stdc++.h>
#define ll long long
#define int ll
using namespace std;

struct SparceTable {
    vector<vector<int>> data;
    vector<int> logs;

    int merge(int &lf, int &ri) {
        return min(lf, ri);// change the operation, must be "op(x, x) = x"
    }

    SparceTable(vector<int> &arr) {
        int n = arr.size();

        logs.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i)
            logs[i] = logs[i / 2] + 1;

        data.assign(logs[n] + 1, vector<int>(n));
        data[0] = arr;

        for (int i = 1; i <= logs[n]; ++i) {
            int len = 1 << i;
            for (int j = 0; j + len <= n; ++j)
                data[i][j] = merge(data[i - 1][j], data[i - 1][j + (len >> 1)]);
        }
    }

    int get(int l, int r) {
        int len = r - l + 1;
        int level = logs[len];
        return merge(data[level][l], data[level][r - (1 << level) + 1]);
    }
};