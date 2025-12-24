#include <bits/stdc++.h>

using namespace std;

vector<int> lazy;
vector<int> tree;
vector<int> A;
int n;

int init_tree(int idx, int l, int r) {
    if (l == r) {
        return tree[idx] = A[l];
    } else {
        int mid = l + (r-l)/2;
        int left_res = init_tree(idx * 2 + 1, l, mid);
        int right_res = init_tree(idx * 2 + 2, mid + 1, r);
        return tree[idx] = left_res + right_res;
    }
}

int query(int l, int r, int c_l, int c_r, int idx) {

	if (lazy[idx] != 0) {
		tree[idx] += lazy[idx] * (c_r - c_l + 1);

		if (c_l != c_r) {
			lazy[idx * 2 + 1] += lazy[idx];
			lazy[idx * 2 + 2] += lazy[idx];
		}

		lazy[idx] = 0;
	}

	if (c_l >= l && c_r <= r) return tree[idx];

	if (c_r < l || c_l > r) return 0;

	int mid = c_l + (c_r - c_l)/2;
	int left_res = query(l, r, c_l, mid, idx * 2 + 1);
	int right_res = query(l, r, mid + 1, c_r, idx * 2 + 2);
	return left_res + right_res;
}

void update(int l, int r, int c_l, int c_r, int idx, int val) {
	if (c_l >= l && c_r <= r) {
		lazy[idx] += val;
		return;
	}

	if (c_r < l || c_l > r) return;

	tree[idx] += val * (min(r, c_r) - max(l, c_l) + 1);
	int mid = c_l + (c_r - c_l)/2;

	update(l, r, c_l, mid, idx * 2 + 1, val);
	update(l, r, mid + 1, c_r, idx * 2 + 2, val);
}

int main() {
	cin.tie(0);
	ios::sync_with_stdio(false);

	int q;
	cin>>n>>q;
	tree.resize(4 * n, 0);
	lazy.resize(4 * n, 0);
	A.resize(n);

	for (int i = 0; i < n; i++) {
		cin>>A[i];
	}

	init_tree(0, 0, n-1);
	for (int i = 0; i < q; i++) {
		int l, r;
		cin>>l>>r;
		cout<<query(l, r, 0, n-1, 0)<<"\n";

		int u_l, u_r, val;
		cin>>u_l>>u_r>>val;
		update(u_l, u_r, 0, n-1, 0, val);
	}

}
