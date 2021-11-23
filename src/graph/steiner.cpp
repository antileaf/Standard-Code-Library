constexpr int maxn = 105, inf = 0x3f3f3f3f;

int dp[maxn][(1 << 10) + 1];
int g[maxn][maxn], a[15];
bool inq[maxn];

int main() {

	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	
	memset(g, 63, sizeof(g));

	while (m--) {
		int u, v, c;
		scanf("%d%d%d", &u, &v, &c);

		g[u][v] = g[v][u] = min(g[u][v], c); // 不要忘了是双向边
	}
	
	memset(dp, 63, sizeof(dp));

	for (int i = 0; i < k; i++) {
		scanf("%d", &a[i]);

		dp[a[i]][1 << i] = 0;
	}

	for (int s = 1; s < (1 << k); s++) {
		for (int i = 1; i <= n; i++)
			for (int t = (s - 1) & s; t; (--t) &= s)
				dp[i][s] = min(dp[i][s], dp[i][t] + dp[i][s ^ t]);
		
		// SPFA
		queue<int> q;
		for (int i = 1; i <= n; i++)
			if (dp[i][s] < inf) {
				q.push(i);
				inq[i] = true;
			}
		
		while (!q.empty()) {
			int i = q.front();
			q.pop();
			inq[i] = false; // 最终结束时inq一定全0, 所以不用清空

			for (int j = 1; j <= n; j++)
				if (dp[i][s] + g[i][j] < dp[j][s]) {
					dp[j][s] = dp[i][s] + g[i][j];
					if (!inq[j]) {
						q.push(j);
						inq[j] = true;
					}
				}
		}
	}

	int ans = inf;
	for (int i = 1; i <= n; i++)
		ans = min(ans, dp[i][(1 << k) - 1]);
	
	printf("%d\n", ans);

	return 0;
}