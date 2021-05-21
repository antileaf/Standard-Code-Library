// 在线求一个点的第k祖先 O(n\log n)-O(1)
// 理论基础: 任意一个点x的k级祖先y所在长链长度一定>=k

// 全局数组定义
vector<int> G[maxn], v[maxn];
int d[maxn], mxd[maxn], son[maxn], top[maxn], len[maxn];
int f[19][maxn], log_tbl[maxn];

// 在主函数中两遍dfs之后加上如下预处理
log_tbl[0] = -1;
for (int i = 1; i <= n; i++)
	log_tbl[i] = log_tbl[i / 2] + 1;
for (int j = 1; (1 << j) < n; j++)
	for (int i = 1; i <= n; i++)
		f[j][i] = f[j - 1][f[j - 1][i]];

// 第一遍dfs, 用于计算深度和找出重儿子
void dfs1(int x) {
	mxd[x] = d[x];

	for (int y : G[x])
		if (y != f[0][x]){
			f[0][y] = x;
			d[y] = d[x] + 1;

			dfs1(y);

			mxd[x] = max(mxd[x], mxd[y]);
			if (mxd[y] > mxd[son[x]])
				son[x] = y;
		}
}

// 第二遍dfs, 用于进行剖分和预处理梯子剖分(每条链向上延伸一倍)数组
void dfs2(int x) {
	top[x] = (x == son[f[0][x]] ? top[f[0][x]] : x);

	for (int y : G[x])
		if (y != f[0][x])
			dfs2(y);

	if (top[x] == x) {
		int u = x;
		while (top[son[u]] == x)
			u = son[u];

		len[x] = d[u] - d[x];
		for (int i = 0; i < len[x]; i++, u = f[0][u])
			v[x].push_back(u);
		
		u = x;
		for (int i = 0; i < len[x] && u; i++, u = f[0][u])
			v[x].push_back(u);
	}
}

// 在线询问x的k级祖先 O(1)
// 不存在时返回0
int query(int x, int k) {
	if (!k)
		return x;
	if (k > d[x])
		return 0;

	x = f[log_tbl[k]][x];
	k ^= 1 << log_tbl[k];
	return v[top[x]][d[top[x]] + len[top[x]] - d[x] + k];
}