// Blossom 带花树 O(nm)
// By ysf
// 通过题目：UOJ#79 一般图最大匹配

// 带花树通常比高斯消元快很多，但在只需要求最大匹配大小的时候并没有高斯消元好写
// 当然输出方案要方便很多

// 全局数组与变量定义
vector<int> G[maxn];
int girl[maxn], f[maxn], t[maxn], p[maxn], vis[maxn], tim, q[maxn], head, tail;
int n, m;


// 封装好的主过程 O(nm)
int blossom() {
	int ans = 0;

	for (int i = 1; i <= n; i++)
		if (!girl[i])
			ans += bfs(i);
			
	return ans;
}


// bfs找增广路 O(m)
bool bfs(int s) {
	memset(t, 0, sizeof(t));
	memset(p, 0, sizeof(p));

	for (int i = 1; i <= n; i++)
		f[i] = i; // 并查集
	
	head = tail = 0;
	q[tail++] = s;
	t[s] = 1;

	while (head != tail){
		int x = q[head++];
		for (int y : G[x]){
			if (findroot(y) == findroot(x) || t[y] == 2)
				continue;

			if (!t[y]){
				t[y] = 2;
				p[y] = x;

				if (!girl[y]){
					for (int u = y, t; u; u = t) {
						t = girl[p[u]];
						girl[p[u]] = u;
						girl[u] = p[u];
					}
					return true;
				}
				t[girl[y]] = 1;
				q[tail++] = girl[y];
			}
			else if (t[y] == 1) {
				int z = LCA(x, y);
				shrink(x, y, z);
				shrink(y, x, z);
			}
		}
	}

	return false;
}

//缩奇环 O(n)
void shrink(int x, int y, int z) {
	while (findroot(x) != z){
		p[x] = y;
		y = girl[x];

		if (t[y] == 2) {
			t[y] = 1;
			q[tail++] = y;
		}

		if(findroot(x) == x)
			f[x] = z;
		if(findroot(y) == y)
			f[y] = z;

		x = p[y];
	}
}

//暴力找LCA O(n)
int LCA(int x, int y) {
	tim++;
	while (true) {
		if (x) {
			x = findroot(x);

			if (vis[x] == tim)
				return x;
			else {
				vis[x] = tim;
				x = p[girl[x]];
			}
		}
		swap(x, y);
	}
}

//并查集的查找 O(1)
int findroot(int x) {
	return x == f[x] ? x : (f[x] = findroot(f[x]));
}