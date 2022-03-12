#include <bits/stdc++.h>
 
using namespace std;
 
constexpr int maxn = 200005, maxm = maxn * 17 * 15;
 
int mx[maxm][2], lc[maxm], rc[maxm], seg_cnt;
int root[maxn];
 
int s, t, d;
 
void modify_seg(int l, int r, int &o) {
	int u = o;
	o = ++seg_cnt;
 
	mx[o][0] = max(mx[u][0], t);
	mx[o][1] = max(mx[u][1], d);
 
	if (l == r)
		return;
 
	lc[o] = lc[u];
	rc[o] = rc[u];
 
	int mid = (l + r) / 2;
	if (s <= mid)
		modify_seg(l, mid, lc[o]);
	else
		modify_seg(mid + 1, r, rc[o]);
}
 
int query_seg(int l, int r, int o, int k) {
	if (s <= l && t >= r)
		return mx[o][k];
	
	int mid = (l + r) / 2, ans = 0;
 
	if (s <= mid)
		ans = max(ans, query_seg(l, mid, lc[o], k));
	if (t > mid)
		ans = max(ans, query_seg(mid + 1, r, rc[o], k));
		
	return ans;
}
 
int N;
 
void modify(int pos, int u, int v, int &rt) {
	s = pos;
	t = u;
	d = v;
 
	modify_seg(1, N, rt);
}
 
int query(int l, int r, int rt) {
	s = l;
	t = r;
	int ans = query_seg(1, N, rt, 0);
 
	s = 1;
	t = l;
	return max(ans, query_seg(1, N, rt, 1) - l);
}
 
struct node {
	int size, l, r, id, tim;
	node *ch[2], *p;
	bool tag;
 
	node() = default;
 
	void apply(int v) {
		tim = v;
		tag = true;
	}
 
	void pushdown() {
		if (tag) {
			ch[0] -> tim = ch[1] -> tim = tim;
			ch[0] -> tag = ch[1] -> tag = true;
 
			tag = false;
		}
	}
 
	void update() {
		size = ch[0] -> size + ch[1] -> size + 1;
		l = (ch[0] -> l ? ch[0] -> l : id);
		r = (ch[1] -> r ? ch[1] -> r : id);
	}
} null[maxn];
 
inline bool isroot(node *x) {
	return x != x -> p -> ch[0] && x != x -> p -> ch[1];
}
 
inline bool dir(node *x) {
	return x == x -> p -> ch[1];
}
 
void init(node *x, int i) {
	*x = node();
	x -> ch[0] = x -> ch[1] = x -> p = null;
	x -> size = 1;
	x -> id = x -> l = x -> r = i;
}
 
void rot(node *x, int d) {
	node *y = x -> ch[d ^ 1];
 
	y -> p = x -> p;
	if (!isroot(x))
		x -> p -> ch[dir(x)] = y;
	
	if ((x -> ch[d ^ 1] = y -> ch[d]) != null)
		y -> ch[d] -> p = x;
	(y -> ch[d] = x) -> p = y;
 
	x -> update();
	y -> update();
}
 
void splay(node *x) {
	x -> pushdown();
 
	while (!isroot(x)) {
		if (!isroot(x -> p))
			x -> p -> p -> pushdown();
		x -> p -> pushdown();
		x -> pushdown();
 
		if (isroot(x -> p)) {
			rot(x -> p, dir(x) ^ 1);
			break;
		}
 
		if (dir(x) == dir(x -> p))
			rot(x -> p -> p, dir(x -> p) ^ 1);
		else
			rot(x -> p, dir(x) ^ 1);
		
		rot(x -> p, dir(x) ^ 1);
	}
}
 
void splay(node *x, node *rt) {
	x -> pushdown();
 
	while (x -> p != rt) {
		if (x -> p -> p != rt)
			x -> p -> p -> pushdown();
		x -> p -> pushdown();
		x -> pushdown();
 
		if (x -> p -> p == rt) {
			rot(x -> p, dir(x) ^ 1);
			break;
		}
 
		if (dir(x) == dir(x -> p))
			rot(x -> p -> p, dir(x -> p) ^ 1);
		else
			rot(x -> p, dir(x) ^ 1);
		
		rot(x -> p, dir(x) ^ 1);
	}
}
 
int val[maxn], par[maxn], go[maxn][26], sam_cnt, sam_last;
 
node *access(node *x, int r) {
	root[r] = root[r - 1];
 
	node *y = null;
 
	while (x != null) {
		splay(x);
 
		x -> pushdown();
 
		x -> ch[1] = null;
		x -> update();
 
		if (x -> tim && val[x -> r]) { // last time visited
			int right = x -> tim, left = right - val[x -> r] + 1;
			modify(left, val[x -> r], right + 1, root[r]);
		}
 
		x -> apply(r);
		x -> pushdown();
 
		x -> ch[1] = y;
		(y = x) -> update();
 
		x = x -> p;
	}
 
	return y;
}
 
void new_leaf(node *x, node *par) {
	x -> p = par;
}
 
void new_node(node *x, node *y, node *par) {
	splay(y);
 
	if (isroot(y) && y -> p == par) {
		assert(y -> ch[0] == null);
		
		y -> ch[0] = x;
		x -> p = y;
		y -> update();
	}
	else {
		splay(par, y);
 
		assert(y -> ch[0] == par);
		assert(par -> ch[1] == null);
		par -> ch[1] = x;
		x -> p = par;
 
		par -> update();
		y -> update();
	}
 
	x -> tim = y -> tim;
}
 
void extend(int c) {
	int p = sam_last, np = ++sam_cnt;
	val[np] = val[p] + 1;
 
	init(null + np, np);
 
	while (p && !go[p][c]) {
		go[p][c] = np;
		p = par[p];
	}
 
	if (!p) {
		par[np] = 1;
		new_leaf(null + np, null + par[np]);
	}
	else {
		int q = go[p][c];
 
		if (val[q] == val[p] + 1) {
			par[np] = q;
			new_leaf(null + np, null + par[np]);
		}
		else {
			int nq = ++sam_cnt;
			val[nq] = val[p] + 1;
			memcpy(go[nq], go[q], sizeof(go[q]));
 
			init(null + nq, nq);
 
			new_node(null + nq, null + q, null + par[q]);
			new_leaf(null + np, null + nq);
 
			par[nq] = par[q];
			par[np] = par[q] = nq;
 
			while (p && go[p][c] == q) {
				go[p][c] = nq;
				p = par[p];
			}
		}
	}
 
	sam_last = np;
}
 
char str[maxn];
 
int main() {
 
	init(null, 0);
	
	sam_last = sam_cnt = 1;
	init(null + 1, 1);
 
	int n, m;
	scanf("%s%d", str + 1, &m);
	n = strlen(str + 1);
	N = n + m;
 
	for (int i = 1; i <= n; i++) {
		extend(str[i] - 'a');
		access(null + sam_last, i);
	}
 
	int tmp = 0;
	
	while (m--) {
		int op;
		scanf("%d", &op);
 
		if (op == 1) {
			scanf(" %c", &str[++n]);
			
			str[n] = (str[n] - 'a' + tmp) % 26 + 'a';
 
			extend(str[n] - 'a');
			access(null + sam_last, n);
		}
		else {
			int l, r;
			scanf("%d%d", &l, &r);
 
			l = (l - 1 + tmp) % n + 1;
			r = (r - 1 + tmp) % n + 1;
 
			printf("%d\n", tmp = query(l, r, root[r]));
		}
	}
 
	return 0;
}