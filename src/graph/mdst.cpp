#include <bits/stdc++.h>

using namespace std;

constexpr int maxn = 505;
constexpr long long inf = 0x3f3f3f3f3f3f3f3fll;

int g[maxn][maxn], id[maxn][maxn], pr[maxn]; // g是邻接矩阵
long long f[maxn][maxn], d[maxn];
bool vis[maxn];

vector<pair<int, int>> minimum_diameter_spanning_tree(int n) { // 1-based
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			g[i][j] *= 2; // 输入的边权都要乘2

	memset(f, 63, sizeof(f));
	
	for (int i = 1; i <= n; i++)
		f[i][i] = 0;

	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			if (g[i][j])
				f[i][j] = g[i][j];
	
	for (int k = 1; k <= n; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++)
			id[i][j] = j; // 距离i第j近的点
		
		sort(id[i] + 1, id[i] + n + 1, [&i] (int x, int y) {
			return f[i][x] < f[i][y];
		});
	}

	int o = 0;
	long long ansv = inf; // vertex

	for (int i = 1; i <= n; i++)
		if (f[i][id[i][n]] * 2 < ansv) {
			ansv = f[i][id[i][n]] * 2;
			o = i;
		}
	
	int u = 0, v = 0;
	long long disu = -inf, disv = -inf, anse = inf;
	
	for (int x = 1; x <= n; x++)
		for (int y = 1; y <= n; y++)
			if (g[x][y]) { // 如果g[x][y] = 0说明没有边
				int w = g[x][y];

				for (int i = n - 1, j = n; i; i--)
					if (f[y][id[x][i]] > f[y][id[x][j]]) {
						long long tmp = f[x][id[x][i]] + f[y][id[x][j]] + w;
						if (tmp < anse) {
							anse = tmp;
							u = x;
							v = y;
							
							disu = tmp / 2 - f[x][id[x][i]];
							disv = w - disu;
						}
						j = i;
					}
			}
	
	printf("%lld\n", min(ansv, anse) / 2); // 直径

	memset(d, 63, sizeof(d));
	
	if (ansv <= anse)
		d[o] = 0;
	else {
		d[u] = disu;
		d[v] = disv;
	}

	for (int k = 1; k <= n; k++) { // Dijkstra
		int x = 0;
		for (int i = 1; i <= n; i++)
			if (!vis[i] && d[i] < d[x])
				x = i;
		
		vis[x] = true;
		for (int y = 1; y <= n; y++)
			if (g[x][y] && !vis[y]) {
				if (d[y] > d[x] + g[x][y]) {
					d[y] = d[x] + g[x][y];
					pr[y] = x;
				}
				else if (d[y] == d[x] + g[x][y] && d[pr[y]] < d[x])
					pr[y] = x;
			}
	}

	vector<pair<int, int>> vec;
	for (int i = 1; i <= n; i++)
		if (pr[i])
			vec.emplace_back(i, pr[i]);
	
	if (ansv > anse)
		vec.emplace_back(u, v);
	
	return vec;
}

int main() {

	int n, m;
	scanf("%d%d", &n, &m);

	while (m--) {
		int x, y, z;
		scanf("%d%d%d", &x, &y, &z);

		g[x][y] = g[y][x] = z; // 无向图
	}

	auto vec = minimum_diameter_spanning_tree(n);
	for (auto [x, y] : vec)
		printf("%d %d\n", x, y);

	return 0;
}