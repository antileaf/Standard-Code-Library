#include <bits/stdc++.h>

using namespace std;

constexpr int maxn = 100005, maxk = 49;
constexpr double alpha = .75;

mt19937 rnd(23333333);

struct node {
	int key, size, p;
	node *ch[2];

	node() {}

	node(int key) : key(key), size(1), p(rnd()) {}

	inline void update() {
		size = ch[0] -> size + ch[1] -> size + 1;
	}
} null[maxn * maxk], *pt = null;

vector<node*> pool;

node *newnode(int val) {
	node *x;

	if (!pool.empty()) {
		x = pool.back();
		pool.pop_back();
	}
	else
		x = ++pt;
	
	*x = node(val);
	x -> ch[0] = x -> ch[1] = null;

	return x;
}

void rot(node *&x, int d) {
	node *y = x -> ch[d ^ 1];
	x -> ch[d ^ 1] = y -> ch[d];
	y -> ch[d] = x;

	x -> update();
	(x = y) -> update();
}

void insert(node *&o, int x) {
	if (o == null) {
		o = newnode(x);
		return;
	}

	int d = (x > o -> key);

	insert(o -> ch[d], x);
	o -> update();

	if (o -> ch[d] -> p < o -> p)
		rot(o, d ^ 1);
}

int get_order(node *o, int x) {
	int ans = 0;

	while (o != null) {
		int d = (x > o -> key);

		if (d)
			ans += o -> ch[0] -> size + 1;
		
		o = o -> ch[d];
	}

	return ans;
}

void destroy(node *x) {
	if (x == null)
		return;
	
	pool.push_back(x);
	destroy(x -> ch[0]);
	destroy(x -> ch[1]);
}

struct my_tree { // 封装了一下, 如果不卡内存直接换成PBDS就好了
	node *rt;
	
	my_tree() : rt(null) {}

	void clear() {
		::destroy(rt);
		rt = null;
	}

	void insert(int x) {
		::insert(rt, x);
	}

	int order_of_key(int x) { // less than x
		return ::get_order(rt, x);
	}
} tr[maxn], tre[maxn][maxk];

vector<pair<int, int> > G[maxn];

int p[maxn], depth[maxn], d[maxn][maxk], rid[maxn][maxk];
int sz[maxn], siz[maxn][maxk], q[maxn];
bool vis[maxn];

int w[maxn];

void destroy(int o) {
	int head = 0, tail = 0;
	q[tail++] = o;
	vis[o] = false;

	while (head != tail) {
		int x = q[head++];
		tr[x].clear();

		for (int i = depth[o]; i <= depth[x]; i++) {
			tre[x][i].clear();
			d[x][i] = rid[x][i] = siz[x][i] = 0;
		}

		for (auto pi : G[x]) {
			int y = pi.first;

			if (vis[y] && depth[y] >= depth[o]) {
				vis[y] = false;
				q[tail++] = y;
			}
		}
	}
}

int getcenter(int o, int s) {
	int head = 0, tail = 0;
	q[tail++] = o;

	while (head != tail) {
		int x = q[head++];
		sz[x] = 1;

		for (auto pi : G[x]) {
			int y = pi.first;

			if (!vis[y] && y != p[x]) {
				p[y] = x;
				q[tail++] = y;
			}
		}
	}

	for (int i = s - 1; i; i--)
		sz[p[q[i]]] += sz[q[i]];
		
	int x = o;
	while (true) {
		bool ok = false;

		for (auto pi : G[x]) {
			int y = pi.first;
			if (!vis[y] && y != p[x] && sz[y] * 2 > s) {
				x = y;
				ok = true;
				break;
			}
		}

		if (!ok)
			break;
	}

	return x;
}

void getdis(int st, int o, int k) {
	int head = 0, tail = 0;
	q[tail++] = st;

	while (head != tail) {
		int x = q[head++];
		sz[x] = 1;
		rid[x][k] = st;

		tr[o].insert(d[x][k] - w[x]);
		tre[st][k].insert(d[x][k] - w[x]);
		
		for (auto pi : G[x]) {
			int y = pi.first, val = pi.second;

			if (!vis[y] && y != p[x]) {
				p[y] = x;
				d[y][k] = d[x][k] + val;
				q[tail++] = y;
			}
		}
	}

	for (int i = tail - 1; i; i--)
		sz[p[q[i]]] += sz[q[i]];

	siz[st][k] = sz[st];
}

void rebuild(int x, int s, int pr) {
	x = getcenter(x, s);
	vis[x] = true;
	p[x] = pr;
	depth[x] = depth[pr] + 1;
	sz[x] = s;

	tr[x].insert(-w[x]);

	for (auto pi : G[x]) {
		int y = pi.first, val = pi.second;

		if (!vis[y]) {
			p[y] = x;
			d[y][depth[x]] = val;
			getdis(y, x, depth[x]);
		}
	}

	for (auto pi : G[x]) {
		int y = pi.first;

		if (!vis[y])
			rebuild(y, sz[y], x);
	}
}

long long add_node(int x, int nw) { // nw是边权
	depth[x] = depth[p[x]] + 1;
	sz[x] = 1;
	vis[x] = true;
	
	tr[x].insert(-w[x]);
	
	long long tmp = 0;
	int goat = 0; // 替罪羊

	for (int u = p[x], k = depth[x] - 1; u; u = p[u], k--) {
		d[x][k] = d[p[x]][k] + nw;
		rid[x][k] = (rid[p[x]][k] ? rid[p[x]][k] : x);
		
		tmp += tr[u].order_of_key(w[x] - d[x][k] + 1);
		tmp -= tre[rid[x][k]][k].order_of_key(w[x] - d[x][k] + 1);

		tr[u].insert(d[x][k] - w[x]);
		tre[rid[x][k]][k].insert(d[x][k] - w[x]);
		
		sz[u]++;
		siz[rid[x][k]][k]++;

		if (siz[rid[x][k]][k] > sz[u] * alpha + 5)
			goat = u;
	}

	if (goat) {
		destroy(goat);
		rebuild(goat, sz[goat], p[goat]);
	}

	return tmp;
}

int main() {

	null -> ch[0] = null -> ch[1] = null;
	null -> size = 0;

	int n;
	scanf("%*d%d", &n);
	
	scanf("%*d%*d%d", &w[1]);
	vis[1] = true;
	sz[1] = 1;
	tr[1].insert(-w[1]);

	printf("0\n");

	long long ans = 0;

	for (int i = 2; i <= n; i++) {
		int nw;
		scanf("%d%d%d", &p[i], &nw, &w[i]);

		p[i] ^= (ans % 1000000000);

		G[i].push_back(make_pair(p[i], nw));
		G[p[i]].push_back(make_pair(i, nw));

		ans += add_node(i, nw);

		printf("%lld\n", ans);
	}
	
	return 0;
}
