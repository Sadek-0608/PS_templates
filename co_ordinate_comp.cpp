#include <bits\stdc++.h>

using namespace std;

vector<int> comp(vector<int> comp) {
    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());
    auto get_id=[&](int val) {
        return lower_bound(comp.begin(), comp.end(), val) - comp.begin();
    };
    return comp;
}