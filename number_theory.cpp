#include <bits/stdc++.h>

const int N = 1e6+1, mod = 1e9 + 7;

set<int> divs(int n) {
    set<int> ans;
    for (int i=1 ; i*i<=n ; i++) {
        if (n%i==0) {
            ans.insert(i);
            if (n/i!=i) ans.insert(n/i);
        }
    }
    return ans;
}

int D[N];
int number_of_divs(int n) {
    for (int i=1 ; i<N ;i++) {
        for (int j=i ; j<N ;j+=i) {
            D[j]++;
        }
    }
    return D[n];
}

ll powmod(ll a,ll n) {
    a%=mod;
    ll res=1;
    while(n) {
        if(n&1)res=(res*a)%mod;
        a=(a*a)%mod;
        n>>=1;
    }
    return res;
}

int MAXN = 1e6+5;
vector<bool>isPrime(MAXN,1);vector<int>primes;
void linearSieve()
{
    isPrime[0]=isPrime[1]=0;
    for(ll i=2;i<MAXN;i++)
    {
        if(isPrime[i]) primes.push_back(i);
        for(auto it:primes)
        {
            if(i*it>=MAXN) break;
            isPrime[i*it]=0;
            if(i%it==0) break;
        }
    }
}