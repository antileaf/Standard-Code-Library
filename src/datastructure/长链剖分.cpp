// 顾名思义, 长链剖分是取最深的儿子作为重儿子

// O(n)维护以深度为下标的子树信息
vector<int> G[maxn], v[maxn];
int n, p[maxn], h[maxn], son[maxn], ans[maxn];

// 原题题意: 求每个点的子树中与它距离是几的点最多, 相同的取最大深度
// 由于vector只能在后面加入元素, 为了写代码方便, 这里反过来存
// 或者开一个结构体维护倒过来的vector
void dfs(int x) {
	h[x] = 1;

	for (int y : G[x])
		if (y != p[x]){
			p[y] = x;
			dfs(y);
			
			if (h[y] > h[son[x]])
				son[x] = y;
		}

	if (!son[x]) {
		v[x].push_back(1);
		ans[x] = 0;
		return;
	}

	h[x] = h[son[x]] + 1;
	swap(v[x],v[son[x]]);

	if (v[x][ans[son[x]]] == 1)
		ans[x] = h[x] - 1;
	else 
		ans[x] = ans[son[x]];

	v[x].push_back(1);

	int mx = v[x][ans[x]];
	for (int y : G[x])
		if (y != p[x] && y != son[x]) {
			for (int j = 1; j <= h[y]; j++) {
				v[x][h[x] - j - 1] += v[y][h[y] - j];

				int t = v[x][h[x] - j - 1];
				if (t > mx || (t == mx && h[x] - j - 1 > ans[x])) {
					mx = t;
					ans[x] = h[x] - j - 1;
				}
			}

			v[y].clear();
		}
}