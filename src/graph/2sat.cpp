bool vis[maxn];
int stk[maxn], top;

// 主函数
for (int i = 0; i < n; i += 2)
	if (!vis[i] && !vis[i ^ 1]) {
			top = 0;
			if (!dfs(i)) {
				while (top)
					vis[stk[top--]] = false;

				if (!dfs(i + 1)) {
					bad = true;
					break;
				}
			}
		}
// 最后stk中的所有元素就是选中的值

// dfs
bool dfs(int x) {
	if (vis[x ^ 1])
		return false;

	if (vis[x])
		return true;

	vis[x] = true;
	stk[++top] = x;

	for (int i = 0; i < (int)G[x].size(); i++)
		if (!dfs(G[x][i]))
			return false;

	return true;
}