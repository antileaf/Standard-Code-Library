int dfn[maxn], low[maxn], tim = 0;
vector<int> G[maxn], scc[maxn];
int sccid[maxn], scc_cnt = 0, stk[maxn];
bool instk[maxn];

void dfs(int x) {
	dfn[x] = low[x] = ++tim;

	stk[++stk[0]] = x;
	instk[x] = true;

	for (int y : G[x]) {
		if (!dfn[y]) {
			dfs(y);
			low[x] = min(low[x], low[y]);
		}
		else if (instk[y])
			low[x] = min(low[x], dfn[y]);
	}

	if (dfn[x] == low[x]) {
		scc_cnt++;

		int u;
		do {
			u = stk[stk[0]--];
			instk[u] = false;
			sccid[u] = scc_cnt;
			scc[scc_cnt].push_back(u);
		} while (u != x);
	}
}

void tarjan(int n) {
	for (int i = 1; i <= n; i++)
		if (!dfn[i])
			dfs(i);
}