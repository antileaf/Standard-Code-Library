// 定理: 一个字符串本质不同的回文子串个数是O(n)的
// 注意回文树只需要开一倍结点, 另外结点编号也是一个可用的bfs序

// 全局数组定义
int val[maxn], par[maxn], go[maxn][26], last, cnt;
char s[maxn];

// 重要!在主函数最前面一定要加上以下初始化
par[0] = cnt = 1;
val[1] = -1;
// 这个初始化和广义回文树不一样, 写普通题可以用, 广义回文树就不要乱搞了

// extend函数 均摊O(1)
// 向后扩展一个字符
// 传入对应下标
void extend(int n) {
	int p = last, c = s[n] - 'a';
	while (s[n - val[p] - 1] != s[n])
		p = par[p];

	if (!go[p][c]) {
		int q = ++cnt, now = p;
		val[q] = val[p] + 2;

		do
			p = par[p];
		while (s[n - val[p] - 1] != s[n]);

		par[q] = go[p][c];
		last = go[now][c] = q;
	}
	else
		last = go[p][c];
		
	// a[last]++;
}
