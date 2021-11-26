#include <bits/stdc++.h>

using namespace std;

constexpr int maxn = 100005, B = 314;

struct Q {
	int l, r, d, id;

	Q() = default;

	Q(int l, int r, int d, int id) : l(l), r(r), d(d), id(id) {}

	friend bool operator < (const Q &a, const Q &b) {
		if (a.d != b.d)
			return a.d < b.d;
		
		return a.r < b.r;
	}
} q[maxn]; // 结构体可以复用, d既可以作为左端点块编号, 也可以作为二次离线处理的倍数

int global_n, bid[maxn], L[maxn], R[maxn], cntb;

int sa[maxn], sb[maxn];

void addp(int x) { // sqrt(n)修改 O(1)查询
	for (int k = bid[x]; k <= cntb; k++)
		sb[k]++;
	
	for (int i = x; i <= R[bid[x]]; i++)
		sa[i]++;
}

int queryp(int x) {
	if (!x)
		return 0;

	return sa[x] + sb[bid[x] - 1];
}

void adds(int x) {
	for (int k = 1; k <= bid[x]; k++)
		sb[k]++;
	
	for (int i = L[bid[x]]; i <= x; i++)
		sa[i]++;
}

int querys(int x) {
	if (x > global_n)
		return 0; // 为了防止越界就判一下
	return sa[x] + sb[bid[x] + 1];
}

vector<Q> vp[maxn], vs[maxn]; // prefix, suffix

long long fp[maxn], fs[maxn]; // prefix, suffix

int a[maxn], b[maxn];

long long ta[maxn], ans[maxn];

int main() {

	int n, m;
	scanf("%d%d", &n, &m);

	global_n = n;

	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	
	memcpy(b, a, sizeof(int) * (n + 1));
	sort(b + 1, b + n + 1);

	for (int i = 1; i <= n; i++)
		a[i] = lower_bound(b + 1, b + n + 1, a[i]) - b;
	
	for (int i = 1; i <= n; i++) {
		bid[i] = (i - 1) / B + 1;
		
		if (!L[bid[i]])
			L[bid[i]] = i;

		R[bid[i]] = i;
		cntb = bid[i];
	}

	for (int i = 1; i <= m; i++) {
		scanf("%d%d", &q[i].l, &q[i].r);

		q[i].d = bid[q[i].l];
		q[i].id = i;
	}

	sort(q + 1, q + m + 1);

	int l = 2, r = 1; // l, r是上一个询问的端点

	for (int i = 1; i <= m; i++) {
		int s = q[i].l, t = q[i].r; // s, t是当前要调整到的端点

		if (s < l)
			vs[r + 1].push_back(Q(s, l - 1, 1, i));
		else if (s > l)
			vs[r + 1].push_back(Q(l, s - 1, -1, i));
		
		l = s;

		if (t > r)
			vp[l - 1].push_back(Q(r + 1, t, 1, i));
		else if (t < r)
			vp[l - 1].push_back(Q(t + 1, r, -1, i));
		
		r = t;
	}

	for (int i = 1; i <= n; i++) { // 第一遍正着处理, 解决关于前缀的询问
		fp[i] = fp[i - 1] + querys(a[i] + 1);

		adds(a[i]);

		for (auto q : vp[i]) {
			long long tmp = 0;
			for (int k = q.l; k <= q.r; k++)
				tmp += querys(a[k] + 1);
			
			ta[q.id] -= q.d * tmp;
		}
	}

	memset(sa, 0, sizeof(sa));
	memset(sb, 0, sizeof(sb));

	for (int i = n; i; i--) { // 第二遍倒着处理, 解决关于后缀的询问
		fs[i] = fs[i + 1] + queryp(a[i] - 1);

		addp(a[i]);

		for (auto q : vs[i]) {
			long long tmp = 0;
			for (int k = q.l; k <= q.r; k++)
				tmp += queryp(a[k] - 1);

			ta[q.id] -= q.d * tmp;
		}
	}

	l = 2;
	r = 1;

	for (int i = 1; i <= m; i++) { // 求出fs和fp之后再加上这部分的贡献
		int s = q[i].l, t = q[i].r;

		ta[i] += fs[s] - fs[l];
		ta[i] += fp[t] - fp[r];

		l = s;
		r = t;

		ta[i] += ta[i - 1]; // 因为算出来的是相邻两个询问之间的贡献, 所以要前缀和
		ans[q[i].id] = ta[i];
	}

	for (int i = 1; i <= m; i++)
		printf("%lld\n", ans[i]);

	return 0;
}