// 注意ISAP适用于一般稀疏图, 对于二分图或分层图情况Dinic比较优, 稠密图则HLPP更优

// 边的定义
// 这里没有记录起点和反向边, 因为反向边即为正向边xor 1, 起点即为反向边的终点
struct edge {
	int to, cap, prev;
} e[maxe * 2];

// 全局变量和数组定义
int last[maxn], cnte = 0, d[maxn], p[maxn], c[maxn], cur[maxn], q[maxn];
int n, m, s, t; // s, t一定要开成全局变量

void AddEdge(int x, int y, int z) {
	e[cnte].to = y;
	e[cnte].cap = z;
	e[cnte].prev = last[x];
	last[x] = cnte++;
}

void addedge(int x, int y, int z) {
	AddEdge(x, y, z);
	AddEdge(y, x, 0);
}

// 预处理到t的距离标号
// 在测试数据组数较少时可以省略,把所有距离标号初始化为0
void bfs() {
	memset(d, -1, sizeof(d));

	int head = 0, tail = 0;
	d[t] = 0;
	q[tail++] = t;

	while (head != tail) {
		int x = q[head++];
		c[d[x]]++;

		for (int i = last[x]; ~i; i = e[i].prev)
			if (e[i ^ 1].cap && d[e[i].to] == -1) {
				d[e[i].to] = d[x] + 1;
				q[tail++] = e[i].to;
			}
	}
}

// augment函数 O(n) 沿增广路增广一次, 返回增广的流量
int augment() {
	int a = (~0u) >> 1; // INT_MAX

	for (int x = t; x != s; x = e[p[x] ^ 1].to)
		a = min(a, e[p[x]].cap);

	for (int x = t; x != s; x = e[p[x] ^ 1].to) {
		e[p[x]].cap -= a;
		e[p[x] ^ 1].cap += a;
	}

	return a;
}

// 主过程 O(n^2 m), 返回最大流的流量
// 注意这里的n是编号最大值,在这个值不为n的时候一定要开个变量记录下来并修改代码
int ISAP() {
	bfs();

	memcpy(cur, last, sizeof(cur));

	int x = s, flow = 0;

	while (d[s] < n) {
		if (x == t) { // 如果走到了t就增广一次, 并返回s重新找增广路
			flow += augment();
			x = s;
		}

		bool ok = false;
		for (int &i = cur[x]; ~i; i = e[i].prev)
			if (e[i].cap && d[x] == d[e[i].to] + 1) {
				p[e[i].to] = i;
				x = e[i].to;

				ok = true;
				break;
			}
		
		if (!ok) { // 修改距离标号
			int tmp = n - 1;
			for (int i = last[x]; ~i; i = e[i].prev)
				if (e[i].cap)
					tmp = min(tmp, d[e[i].to] + 1);

			if (!--c[d[x]])
				break; // gap优化, 一定要加上

			c[d[x] = tmp]++;
			cur[x] = last[x];

			if(x != s)
				x = e[p[x] ^ 1].to;
		}
	}
	return flow;
}

// 重要! main函数最前面一定要加上如下初始化
memset(last, -1, sizeof(last));