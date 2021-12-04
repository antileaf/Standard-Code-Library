vector<pair<int, int> > q[maxn];
int lca[maxn];

void dfs(int x) {
	dfn[x] = ++tim; // 其实求LCA是用不到DFS序的, 但是一般其他步骤要用
	ufs[x] = x;

	for (auto pi : q[x]) {
		int y = pi.first, i = pi.second;
		if (dfn[y])
			lca[i] = findufs(y);
	}

	for (int y : G[x])
		if (y != p[x]) {
			p[y] = x;
			dfs(y);
		}
	
	ufs[x] = p[x];
}