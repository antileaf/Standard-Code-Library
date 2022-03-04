#include <bits/stdc++.h>

using namespace std;

constexpr int maxn = (1 << 20) + 5;

int cnt[maxn][2], mul[maxn][2];
bool rev[maxn];
long long sum[maxn];

int now;

void build(int l, int r, int o) {
	cnt[o][0] = r - l + 1;

	if (l == r)
		return;
	
	int mid = (l + r) / 2;
	
	build(l, mid, o * 2);
	build(mid + 1, r, o * 2 + 1);
}

void apply(int o, bool flip, long long w0, long long w1) {
	sum[o] += w0 * cnt[o][0] + w1 * cnt[o][1];

	if (flip)
		swap(cnt[o][0], cnt[o][1]);

	if (rev[o])
		swap(w0, w1);
	
	mul[o][0] += w0;
	mul[o][1] += w1;
	rev[o] ^= flip;
}

void pushdown(int o) {
	if (!mul[o][0] && !mul[o][1] && !rev[o])
		return;
	
	apply(o * 2, rev[o], mul[o][0], mul[o][1]);
	apply(o * 2 + 1, rev[o], mul[o][0], mul[o][1]);

	mul[o][0] = mul[o][1] = 0;
	rev[o] = false;
}

void update(int o) {
	cnt[o][0] = cnt[o * 2][0] + cnt[o * 2 + 1][0];
	cnt[o][1] = cnt[o * 2][1] + cnt[o * 2 + 1][1];

	sum[o] = sum[o * 2] + sum[o * 2 + 1];
}

int s, t;

void modify(int l, int r, int o) {
	if (s <= l && t >= r) {
		apply(o, true, 0, 0);
		return;
	}

	int mid = (l + r) / 2;
	pushdown(o);

	if (s <= mid)
		modify(l, mid, o * 2);
	if (t > mid)
		modify(mid + 1, r, o * 2 + 1);
		
	update(o);
}

long long query(int l, int r, int o) {
	if (s <= l && t >= r)
		return sum[o];
	
	int mid = (l + r) / 2;
	pushdown(o);

	long long ans = 0;
	if (s <= mid)
		ans += query(l, mid, o * 2);
	if (t > mid)
		ans += query(mid + 1, r, o * 2 + 1);
	
	return ans;
}

vector<pair<int, int> > vec[maxn]; // pos, id

long long ans[maxn];
int a[maxn], last[maxn];

int main() {

	int n;
	scanf("%d", &n);

	build(1, n, 1);

	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	
	int m;
	scanf("%d", &m);

	for (int i = 1; i <= m; i++) {
		int l, r;
		scanf("%d%d", &l, &r);

		vec[r].emplace_back(l, i);
	}

	for (int i = 1; i <= n; i++) {
		s = last[a[i]] + 1;
		t = now = i;

		modify(1, n, 1);
		apply(1, false, 0, 1);

		for (auto [l, k] : vec[i]) {
			s = l;
			ans[k] = query(1, n, 1);
		}

		last[a[i]] = i;
	}

	for (int i = 1; i <= m; i++)
		printf("%lld\n", ans[i]);

	return 0;
}