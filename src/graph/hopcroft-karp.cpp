vector<int> G[maxn];

int girl[maxn], boy[maxn]; // girl: 左边匹配右边  boy: 右边匹配左边

bool vis[maxn]; // 右半的点是否已被访问
int dx[maxn], dy[maxn];
int q[maxn];

bool bfs(int n) {
	memset(dx, -1, sizeof(int) * (n + 1));
	memset(dy, -1, sizeof(int) * (n + 1));
	
	int head = 0, tail = 0;
	for (int i = 1; i <= n; i++)
		if (!girl[i]) {
			q[tail++] = i;
			dx[i] = 0;
		}
	
	bool flag = false;
	
	while (head != tail) {
		int x = q[head++];

		for (auto y : G[x])
			if (dy[y] == -1) {
				dy[y] = dx[x] + 1;
				
				if (boy[y]) {
					if (dx[boy[y]] == -1) {
						dx[boy[y]] = dy[y] + 1;
						q[tail++] = boy[y];
					}
				}
				else
					flag = true;
			}
	}

	return flag;
}

bool dfs(int x) {
	for (int y : G[x])
		if (!vis[y] && dy[y] == dx[x] + 1) {
			vis[y] = true;

			if (boy[y] && !dfs(boy[y]))
				continue;
			
			girl[x] = y;
			boy[y] = x;
			return true;
		}
	
	return false;
}

int hopcroft_karp(int n) {
	int ans = 0;
	
	for (int x = 1; x <= n; x++) // 先贪心求出一组初始匹配, 当然不写贪心也行
		for (int y : G[x])
			if (!boy[y]) {
				girl[x] = y;
				boy[y] = x;
				ans++;
				break;
			}
	
	while (bfs(n)) {
		memset(vis, 0, sizeof(bool) * (n + 1));

		for (int x = 1; x <= n; x++)
			if (!girl[x])
				ans += dfs(x);
	}

	return ans;
}