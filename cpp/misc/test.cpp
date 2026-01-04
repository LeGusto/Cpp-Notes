#include<bits/stdc++.h>

using namespace std;


long long solve(int l, int r) {
    [[maybe_unused]]int n = l + r;
    int m1 = r;
    int m2 = l - 1;
    int m3 = n - (l - 1);
    int m4 = n - r;
    set<int> edges;
    edges.insert(m1); edges.insert(m2); edges.insert(m3); edges.insert(m4);
    return (edges.size() * (long long)n) / 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int t = 1;
    // cin>>t;
    while (t--) {
        long long l, r;
        cin>>l>>r;
        long long ans1 = solve(l, r);
        cout<<ans1<<"\n";
        // long long ans2 = solve2(l, r);
        // comp(ans1, ans2);
        // cout<<"====\n";
        // solve2(l, r);
        // cout<<"-----\n";
    }
}

/*
5
2 2
2 3
2 4
2 5
2 6

5
3 3
3 4
3 5
3 6
3 7

5
4 4
4 5
4 6
4 7
4 8

7
2 2
3 2
4 2
5 2
6 2
7 2
8 2

7
3 3
4 3
5 3
6 3
7 3
8 3
9 3


10
8 5
2 3
45 20
5 52
6 9
10 11
12 13
25 32
20 21
80 50
*/