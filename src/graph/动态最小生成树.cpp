// 动态最小生成树的离线算法比较容易,而在线算法通常极为复杂
// 一个跑得比较快的离线做法是对时间分治,在每层分治时找出一定在/不在MST上的边,只带着不确定边继续递归
// 简单起见,找确定边的过程用Kruskal算法实现,过程中的两种重要操作如下:
// - Reduction:待修改边标为+INF,跑MST后把非树边删掉,减少无用边
// - Contraction:待修改边标为-INF,跑MST后缩除待修改边之外的所有MST边,计算必须边
// 每轮分治需要Reduction-Contraction,借此减少不确定边,从而保证复杂度
// 复杂度证明:假设当前区间有k条待修改边,n和m表示点数和边数,那么最坏情况下R-C的效果为(n, m) -> (n, n + k - 1) -> (k + 1, 2k)


// 全局结构体与数组定义
struct edge { //边的定义
	int u, v, w, id; // id表示边在原图中的编号
	bool vis; // 在Kruskal时用,记录这条边是否是树边
	bool operator < (const edge &e) const { return w < e.w; }
} e[20][maxn], t[maxn]; // 为了便于回滚,在每层分治存一个副本


// 用于存储修改的结构体,表示第id条边的权值从u修改为v
struct A {
	int id, u, v;
} a[maxn];


int id[20][maxn]; // 每条边在当前图中的编号
int p[maxn], size[maxn], stk[maxn], top; // p和size是并查集数组,stk是用来撤销的栈
int n, m, q; // 点数,边数,修改数


// 方便起见,附上可能需要用到的预处理代码
for (int i = 1; i <= n; i++) { // 并查集初始化
	p[i] = i;
	size[i] = 1;
}

for (int i = 1; i <= m; i++) { // 读入与预标号
	scanf("%d%d%d", &e[0][i].u, &e[0][i].v, &e[0][i].w);
	e[0][i].id = i;
	id[0][i] = i;
}

for (int i = 1; i <= q; i++) { // 预处理出调用数组
	scanf("%d%d", &a[i].id, &a[i].v);
	a[i].u = e[0][a[i].id].w;
	e[0][a[i].id].w = a[i].v;
}

for(int i = q; i; i--)
	e[0][a[i].id].w = a[i].u;

CDQ(1, q, 0, m, 0); // 这是调用方法


// 分治主过程 O(nlog^2n)
// 需要调用Reduction和Contraction
void CDQ(int l, int r, int d, int m, long long ans) { // CDQ分治
	if (l == r) { // 区间长度已减小到1,输出答案,退出
		e[d][id[d][a[l].id]].w = a[l].v;
		printf("%lld\n", ans + Kruskal(m, e[d]));
		e[d][id[d][a[l].id]].w=a[l].u;
		return;
	}

	int tmp = top;

	Reduction(l, r, d, m);
	ans += Contraction(l, r, d, m); // R-C

	int mid = (l + r) / 2;

	copy(e[d] + 1, e[d] + m + 1, e[d + 1] + 1);
	for (int i = 1; i <= m; i++)
		id[d + 1][e[d][i].id] = i; // 准备好下一层要用的数组
		
	CDQ(l, mid, d + 1, m, ans);

	for (int i = l; i <= mid; i++)
		e[d][id[d][a[i].id]].w = a[i].v; // 进行左边的修改

	copy(e[d] + 1, e[d] + m + 1, e[d + 1] + 1);
	for (int i = 1; i <= m; i++)
		id[d + 1][e[d][i].id] = i; // 重新准备下一层要用的数组

	CDQ(mid + 1, r, d + 1, m, ans);

	for (int i = top; i > tmp; i--)
		cut(stk[i]);//撤销所有操作
	top = tmp;
}


// Reduction(减少无用边):待修改边标为+INF,跑MST后把非树边删掉,减少无用边
// 需要调用Kruskal
void Reduction(int l, int r, int d, int &m) {
	for (int i = l; i <= r; i++)
		e[d][id[d][a[i].id]].w = INF;//待修改的边标为INF

	Kruskal(m, e[d]);

	copy(e[d] + 1, e[d] + m + 1, t + 1);

	int cnt = 0;
	for (int i = 1; i <= m; i++)
		if (t[i].w == INF || t[i].vis){ // 非树边扔掉
			id[d][t[i].id] = ++cnt; // 给边重新编号
			e[d][cnt] = t[i];
		}

	for (int i = r; i >= l; i--)
		e[d][id[d][a[i].id]].w = a[i].u; // 把待修改的边改回去

	m=cnt;
}


// Contraction(缩必须边):待修改边标为-INF,跑MST后缩除待修改边之外的所有树边
// 返回缩掉的边的总权值
// 需要调用Kruskal
long long Contraction(int l, int r, int d, int &m) {
	long long ans = 0;

	for (int i = l; i <= r; i++)
		e[d][id[d][a[i].id]].w = -INF; // 待修改边标为-INF

	Kruskal(m, e[d]);
	copy(e[d] + 1, e[d] + m + 1, t + 1);

	int cnt = 0;
	for (int i = 1; i <= m ; i++) {

		if (t[i].w != -INF && t[i].vis) { // 必须边
			ans += t[i].w;
			mergeset(t[i].u, t[i].v);
		}
		else { // 不确定边
			id[d][t[i].id]=++cnt;
			e[d][cnt]=t[i];
		}
	}

	for (int i = r ; i >= l; i--) {
		e[d][id[d][a[i].id]].w = a[i].u; // 把待修改的边改回去
		e[d][id[d][a[i].id]].vis = false;
	}

	m = cnt;

	return ans;
}


// Kruskal算法 O(mlogn)
// 方便起见,这里直接沿用进行过缩点的并查集,在过程结束后撤销即可
long long Kruskal(int m, edge *e) {
	int tmp = top;
	long long ans = 0;

	sort(e + 1, e + m + 1); // 比较函数在结构体中定义过了

	for (int i = 1; i <= m; i++) {
		if (findroot(e[i].u) != findroot(e[i].v)) {
			e[i].vis = true;
			ans += e[i].w;
			mergeset(e[i].u, e[i].v);
		}
		else
			e[i].vis = false;
	}

	for(int i = top; i > tmp; i--)
		cut(stk[i]); // 撤销所有操作
	top = tmp;

	return ans;
}


// 以下是并查集相关函数
int findroot(int x) { // 因为需要撤销,不写路径压缩
	while (p[x] != x)
		x = p[x];

	return x;
}

void mergeset(int x, int y) { // 按size合并,如果想跑得更快就写一个按秩合并
	x = findroot(x); // 但是按秩合并要再开一个栈记录合并之前的秩
	y = findroot(y);

	if (x == y)
		return;

	if (size[x] > size[y])
		swap(x, y);

	p[x] = y;
	size[y] += size[x];
	stk[++top] = x;
}

void cut(int x) { // 并查集撤销
	int y = x;

	do
		size[y = p[y]] -= size[x];
	while (p[y]! = y);

	p[x] = x;
}