vector<pair<int, int> > seg[(1 << 22) + 5];

int s, t;
pair<int, int> d;

void add(int l, int r, int o) {
	if (s > t)
		return;

	if (s <= l && t >= r) {
		seg[o].push_back(d);
		return;
	}

	int mid = (l + r) / 2;
	
	if (s <= mid)
		add(l, mid, o * 2);
	if (t > mid)
		add(mid + 1, r, o * 2 + 1);
}

int ufs[maxn], sz[maxn], stk[maxn], top;

int findufs(int x) {
	while (ufs[x] != x)
		x = ufs[x];
	
	return ufs[x];
}

void link(int x, int y) {
	x = findufs(x);
	y = findufs(y);

	if (x == y)
		return;
	
	if (sz[x] < sz[y])
		swap(x, y);
	
	ufs[y] = x;
	sz[x] += sz[y];
	stk[++top] = y;
}

int ans[maxm];

void solve(int l, int r, int o) {
	int tmp = top;

	for (auto pi : seg[o])
		link(pi.first, pi.second);
	
	if (l == r)
		ans[l] = top;
	else {
		int mid = (l + r) / 2;

		solve(l, mid, o * 2);
		solve(mid + 1, r, o * 2 + 1);
	}

	for (int i = top; i > tmp; i--) {
		int x = stk[i];

		sz[ufs[x]] -= sz[x];
		ufs[x] = x;
	}

	top = tmp;
}

map<pair<int, int>, int> mp;