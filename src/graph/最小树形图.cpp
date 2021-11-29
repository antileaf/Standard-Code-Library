constexpr int maxn = 105, maxe = 10005, inf = 0x3f3f3f3f;

struct edge {
	int u, v, w;
} e[maxe];

int mn[maxn], pr[maxn], ufs[maxn], vis[maxn];
bool alive[maxn];

int edmonds(int n, int m, int rt) {
	for (int i = 1; i <= n; i++)
		alive[i] = true;
	
	int ans = 0;

	while (true) {
		memset(mn, 63, sizeof(int) * (n + 1));
		memset(pr, 0, sizeof(int) * (n + 1));
		memset(ufs, 0, sizeof(int) * (n + 1));
		memset(vis, 0, sizeof(int) * (n + 1));

		mn[rt] = 0;

		for (int i = 1; i <= m; i++)
			if (e[i].u != e[i].v && e[i].w < mn[e[i].v]) {
				mn[e[i].v] = e[i].w;
				pr[e[i].v] = e[i].u;
			}
		
		for (int i = 1; i <= n; i++)
			if (alive[i]) {
				if (mn[i] >= inf)
					return -1; // 不存在最小树形图
				
				ans += mn[i];
			}
		
		bool flag = false;
		
		for (int i = 1; i <= n; i++) {
			if (!alive[i])
				continue;

			int x = i;
			while (x && !vis[x]) {
				vis[x] = i;
				x = pr[x];
			}

			if (x && vis[x] == i) {
				flag = true;
				for (int u = x; !ufs[u]; u = pr[u])
					ufs[u] = x;
			}
		}

		for (int i = 1; i <= m; i++) {
			e[i].w -= mn[e[i].v];

			if (ufs[e[i].u])
				e[i].u = ufs[e[i].u];
			if (ufs[e[i].v]) 
				e[i].v = ufs[e[i].v];
		}

		if (!flag)
			return ans;

		for (int i = 1; i <= n; i++)
			if (ufs[i] && i != ufs[i])
				alive[i] = false;
	}
}