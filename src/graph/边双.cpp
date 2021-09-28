int u[maxe], v[maxe];
vector<int> G[maxn]; // 存的是边的编号

int stk[maxn], top, dfn[maxn], low[maxn], tim, bcc_cnt;
vector<int> bcc[maxn];

bool isbridge[maxe];

void dfs(int x, int pr) { // 这里pr是入边的编号
	dfn[x] = low[x] = ++tim;
	stk[++top] = x;

	for (int i : G[x]) {
		int y = (u[i] == x ? v[i] : u[i]);

		if (!dfn[y]) {
			dfs(y, i);
			low[x] = min(low[x], low[y]);

			if (low[y] > dfn[x])
				bridge[i] = true;
		}
		else if (i != pr)
			low[x] = min(low[x], dfn[y]);
	}

	if (dfn[x] == low[x]) {
		bcc_cnt++;
		int y;
		do {
			y = stk[top--];
			bcc[bcc_cnt].push_back(y);
		} while (y != x);
	}
}