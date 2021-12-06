#include <bits/stdc++.h>

using namespace std;

constexpr int maxn = 100005, maxm = 262155, inf = 0x3f3f3f3f;

struct binary_heap {
	priority_queue<int> q, t;

	binary_heap() {}

	void push(int x) {
		q.push(x);
	}
	
	void erase(int x) {
		t.push(x);
	}

	int top() {
		while (!t.empty() && q.top() == t.top()) {
			q.pop();
			t.pop();
		}

		return q.top();
	}
} heap;

int pool[maxm][2][2], (*pt)[2][2] = pool;

void merge(int a[2][2], int b[2][2]) {
	static int c[2][2];
	memset(c, 0, sizeof(c));

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				if (!(j && k))
					for (int t = 0; t < 2; t++)
						c[i][t] = max(c[i][t], a[i][j] + b[k][t]);
	
	memcpy(a, c, sizeof(c));
}

vector<pair<int, int> > tw;

struct seg_tree {
	int (*tr)[2][2], n;

	int s, d[2];

	seg_tree() {}
	
	void update(int o) {
		memcpy(tr[o], tr[o * 2], sizeof(int) * 4);
		merge(tr[o], tr[o * 2 + 1]);
	}

	void build(int l, int r, int o) {
		if (l == r) {
			tr[o][0][0] = tw[l - 1].first;
			tr[o][0][1] = tr[o][1][0] = -inf;
			tr[o][1][1] = tw[l - 1].second;

			return;
		}

		int mid = (l + r) / 2;

		build(l, mid, o * 2);
		build(mid + 1, r, o * 2 + 1);

		update(o);
	}

	void modify(int l, int r, int o) {
		if (l == r) {
			tr[o][0][0] = d[0];
			tr[o][0][1] = tr[o][1][0] = -inf;
			tr[o][1][1] = d[1];
			
			return;
		}

		int mid = (l + r) / 2;
		
		if (s <= mid)
			modify(l, mid, o * 2);
		else
			modify(mid + 1, r, o * 2 + 1);
		
		update(o);
	}

	int getval() {
		int ans = 0;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				ans = max(ans, tr[1][i][j]);
		
		return ans;
	}

	pair<int, int> getpair() {
		int ans[2] = {0};
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				ans[i] = max(ans[i], tr[1][i][j]);
		
		return make_pair(ans[0], ans[1]);
	}

	void build(int len) {
		n = len;
		int N = 1;
		while (N < n * 2)
			N *= 2;
		
		tr = pt;
		pt += N;

		build(1, n, 1);
	}

	void modify(int x, int dat[2]) {
		s = x;
		for (int i = 0; i < 2; i++)
			d[i] = dat[i];
		modify(1, n, 1);
	}
} seg[maxn];

vector<int> G[maxn];

int p[maxn], d[maxn], sz[maxn], son[maxn], top[maxn];
int dp[maxn][2], dptr[maxn][2], w[maxn];

void dfs1(int x) {
	d[x] = d[p[x]] + 1;
	sz[x] = 1;

	for (int y : G[x])
		if (y != p[x]) {
			p[y] = x;
			dfs1(y);

			if (sz[y] > sz[son[x]])
				son[x] = y;
			
			sz[x] += sz[y];
		}
}

void dfs2(int x) {
	if (x == son[p[x]])
		top[x] = top[p[x]];
	else
		top[x] = x;
		
	for (int y : G[x])
		if (y != p[x])
			dfs2(y);

	dp[x][1] = w[x];
	for (int y : G[x])
		if (y != p[x] && y != son[x]) {
			dp[x][1] += dptr[y][0];
			dp[x][0] += max(dptr[y][0], dptr[y][1]);
		}
	
	if (top[x] == x) {
		tw.clear();

		for (int u = x; u; u = son[u])
			tw.push_back(make_pair(dp[u][0], dp[u][1]));
		
		seg[x].build((int)tw.size());

		tie(dptr[x][0], dptr[x][1]) = seg[x].getpair();

		heap.push(seg[x].getval());
	}
}

void modify(int x, int dat) {
	dp[x][1] -= w[x];
	dp[x][1] += (w[x] = dat);

	while (x) {
		if (p[top[x]]) {
			dp[p[top[x]]][0] -= max(dptr[top[x]][0], dptr[top[x]][1]);
			dp[p[top[x]]][1] -= dptr[top[x]][0];
		}

		heap.erase(seg[top[x]].getval());
		seg[top[x]].modify(d[x] - d[top[x]] + 1, dp[x]);
		heap.push(seg[top[x]].getval());

		tie(dptr[top[x]][0], dptr[top[x]][1]) = seg[top[x]].getpair();

		if (p[top[x]]) {
			dp[p[top[x]]][0] += max(dptr[top[x]][0], dptr[top[x]][1]);
			dp[p[top[x]]][1] += dptr[top[x]][0];
		}

		x = p[top[x]];
	}
}

int main() {

	int n, m;
	scanf("%d%d", &n, &m);

	for (int i = 1; i <= n; i++)
		scanf("%d", &w[i]);

	for (int i = 1; i < n; i++) {
		int x, y;
		scanf("%d%d", &x, &y);

		G[x].push_back(y);
		G[y].push_back(x);
	}

	dfs1(1);
	dfs2(1);

	while (m--) {
		int x, dat;
		scanf("%d%d", &x, &dat);

		modify(x, dat);

		printf("%d\n", heap.top());
	}

	return 0;
}