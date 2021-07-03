vector<int> G[maxn], bcc[maxn];
int dfn[maxn], low[maxn], tim = 0, bccid[maxn], bcc_cnt = 0;
bool iscut[maxn];

pair<int, int> stk[maxn];
int stk_cnt = 0;

void dfs(int x, int pr) {
	int child = 0;
	dfn[x] = low[x] = ++tim;

	for (int y : G[x]) {
		if (!dfn[y]) {
			stk[++stk_cnt] = make_pair(x, y);
			child++;
			dfs(y, x);
			low[x] = min(low[x], low[y]);

			if (low[y] >= dfn[x]) {
				iscut[x] = true;
				bcc_cnt++;

				while (true) {
					auto pi = stk[stk_cnt--];
					
					if (bccid[pi.first] != bcc_cnt) {
						bcc[bcc_cnt].push_back(pi.first);
						bccid[pi.first] = bcc_cnt;
					}
					if (bccid[pi.second] != bcc_cnt) {
						bcc[bcc_cnt].push_back(pi.second);
						bccid[pi.second] = bcc_cnt;
					}

					if (pi.first == x && pi.second == y)
						break;
				}
			}
		}
		else if (dfn[y] < dfn[x] && y != pr) {
			stk[++stk_cnt] = make_pair(x, y);
			low[x] = min(low[x], dfn[y]);
		}
	}

	if (!pr && child == 1)
		iscut[x] = false;
}

void Tarjan(int n) {
	for (int i = 1; i <= n; i++)
		if (!dfn[i])
			dfs(i, 0);
}