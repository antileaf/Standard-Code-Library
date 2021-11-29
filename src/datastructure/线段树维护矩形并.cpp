constexpr int maxn = 100005, maxm = maxn * 70;

int lc[maxm], rc[maxm], cover[maxm], sum[maxm], root, seg_cnt;
int s, t, d;

void refresh(int l, int r, int o) {
	if (cover[o])
		sum[o] = r - l + 1;
	else
		sum[o] = sum[lc[o]] + sum[rc[o]];
}

void modify(int l, int r, int &o) {
	if (!o)
		o = ++seg_cnt;

	if (s <= l && t >= r) {
		cover[o] += d;
		refresh(l, r, o);

		return;
	}
	
	int mid = (l + r) / 2;
	
	if (s <= mid)
		modify(l, mid, lc[o]);
	if (t > mid)
		modify(mid + 1, r, rc[o]);
	
	refresh(l, r, o);
}

struct modi {
	int x, l, r, d;
	
	bool operator < (const modi &o) {
		return x < o.x;
	}
} a[maxn * 2];

int main() {

	int n;
	scanf("%d", &n);

	for (int i = 1; i <= n; i++) {
		int lx, ly, rx, ry;
		scanf("%d%d%d%d", &lx, &ly, &rx, &ry);

		a[i * 2 - 1] = {lx, ly + 1, ry, 1};
		a[i * 2] = {rx, ly + 1, ry, -1};
	}

	sort(a + 1, a + n * 2 + 1);

	int last = -1;
	long long ans = 0;

	for (int i = 1; i <= n * 2; i++) {
		if (last != -1)
			ans += (long long)(a[i].x - last) * sum[1];
		last = a[i].x;

		s = a[i].l;
		t = a[i].r;
		d = a[i].d;

		modify(1, 1e9, root);
	}

	printf("%lld\n", ans);

	return 0;
}