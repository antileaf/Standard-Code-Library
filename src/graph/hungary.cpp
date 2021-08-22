vector<int> G[maxn];

int girl[maxn], boy[maxn]; // 男孩在左边, 女孩在右边
bool vis[maxn];

bool dfs(int x) {
	for (int y : G[x])
		if (!vis[y]) {
			vis[y] = true;

			if (!boy[y] || dfs(boy[y])) {
				girl[x] = y;
				boy[y] = x;

				return true;
			}
		}
	
	return false;
}

int hungary() {
	int ans = 0;
	
	for (int i = 1; i <= n; i++)
		if (!girl[i]) {
			memset(vis, 0, sizeof(vis));
			ans += dfs(i);
		}
	
	return ans;
}