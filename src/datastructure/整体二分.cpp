int op[maxn], ql[maxn], qr[maxn]; // 1: modify  2: query
long long qk[maxn]; // 修改和询问可以一起存

int ans[maxn];

void solve(int l, int r, vector<int> v) { // 如果想卡常可以用数组, 然后只需要传一个数组的l, r； 递归的时候类似归并反过来, 开两个辅助数组, 处理完再复制回去即可
	if (v.empty())
		return;

	if (l == r) {
		for (int i : v)
			if (op[i] == 2)
				ans[i] = l;
		
		return;
	}

	int mid = (l + r) / 2;
	
	vector<int> vl, vr;

	for (int i : v) {
		if (op[i] == 1) {
			if (qk[i] <= mid)
				vl.push_back(i);
			else {
				update(ql[i], qr[i], 1); // update是区间加
				vr.push_back(i);
			}
		}
		else {
			long long tmp = query(ql[i], qr[i]);

			if (qk[i] <= tmp) // 因为是k大数查询
				vr.push_back(i);
			else {
				qk[i] -= tmp;
				vl.push_back(i);
			}
		}
	}

	for (int i : vr)
		if (op[i] == 1)
			update(ql[i], qr[i], -1);

	v.clear();

	solve(l, mid, vl);
	solve(mid + 1, r, vr);
}

int main() {
	int n, m;
	scanf("%d%d", &n, &m);

	M = 1;
	while (M < n + 2)
		M *= 2;
	
	for (int i = 1; i <= m; i++)
		scanf("%d%d%d%lld", &op[i], &ql[i], &qr[i], &qk[i]);
	
	vector<int> v;
	for (int i = 1; i <= m; i++)
		v.push_back(i);

	solve(1, 1e9, v);

	for (int i = 1; i <= m; i++)
		if (op[i] == 2)
			printf("%d\n", ans[i]);

	return 0;
}