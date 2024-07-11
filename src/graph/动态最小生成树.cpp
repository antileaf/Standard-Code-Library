struct Edge { int u, v, w; } edges[MAXN];

int vis[MAXN], stamp = 0; // 因为不好清空，用时间戳标记

struct Modify { int i, new_w; } op[MAXN]; // 方便起见，只有修改边权一种操作

int ufs[MAXN], ufs_size[MAXN]; // 因为要撤销，不方便存 rank，所以用 size

void ufs_init(int n) {
	for (int i = 1; i <= n; i++) {
		ufs[i] = i;
		ufs_size[i] = 1;
	}
}

int findufs(int x) { while (ufs[x] != x) x = ufs[x]; return x; }

void undo(const vector<int>& stk) {
	for (int i = (int)stk.size() - 1; ~i; i--) {
		int x = stk[i]; ufs_size[ufs[x]] -= ufs_size[x];
		ufs[x] = x; } }

bool link(int u, int v, vector<int>& stk) {
	int x = findufs(u), y = findufs(v);

	if (x != y) {
		if (ufs_size[x] > ufs_size[y])
			swap(x, y);
		
		ufs[x] = y;
		ufs_size[y] += ufs_size[x];
		stk.push_back(x);
		return true;
	}
	return false;
}

// Returns { unused, used }. e should be already sorted.
pair<vector<int>, vector<int>> kruskal(const vector<int>& e, const vector<int>& must) {
	vector<int> stk, used[2];

	for (int i : must)
		link(edges[i].u, edges[i].v, stk);

	for (int i : e)
		used[link(edges[i].u, edges[i].v, stk)].push_back(i);
	
	undo(stk);
	
	return {used[0], used[1]};
}

ll ans[MAXN];

void solve(int l, int r, vector<int> e, ll sum) {
	if (l == r) {
		edges[op[l].i].w = op[l].new_w;

		sort(e.begin(), e.end(), [] (int i, int j) { return edges[i].w < edges[j].w; });
		for (int i : kruskal(e, {}).second)
			sum += edges[i].w;

		// Did not check if the graph is connected.
		ans[l] = sum;

		return;
	}

	sort(e.begin(), e.end(), [] (int i, int j) { return edges[i].w < edges[j].w; });

	stamp++;
	vector<int> mod;
	for (int t = l; t <= r; t++)
		if (vis[op[t].i] < stamp) {
			vis[op[t].i] = stamp;
			mod.push_back(op[t].i);
		}
	
	vector<int> notmod;
	for (int i : e)
		if (vis[i] < stamp)
			notmod.push_back(i);
	
	// Reduction, unused edges are dismissed.
	notmod = get<1>(kruskal(notmod, {}));
	
	// Contraction, used edges are sure to be in MST.
	auto [notsure, must] = kruskal(notmod, mod);

	vector<int> stk;
	for (int i : must) {
		assert(link(edges[i].u, edges[i].v, stk));
		sum += edges[i].w;
	}

	for (int i : mod)
		notsure.push_back(i);
	
	int mid = (l + r) / 2;
	solve(l, mid, notsure, sum);
	solve(mid + 1, r, notsure, sum);

	undo(stk);
}

int main() { ufs_init(n); }
