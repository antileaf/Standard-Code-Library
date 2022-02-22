// 这个东西虽然只需要抄板子但还是极其难写, 常数极其巨大, 没必要的时候就不要用
// 如果维护子树最小值就需要套一个可删除的堆来维护, 复杂度会变成O(n\log^2 n)
// 注意由于这道题与边权有关, 需要边权拆点变点权

// 宏定义
#define isroot(x) ((x) -> p == null || ((x) != (x) -> p -> ch[0] && (x) != (x) -> p -> ch[1]))
#define dir(x) ((x) == (x) -> p -> ch[1])

// 节点类定义
struct node { // 以维护子树中黑点到根距离和为例
	int w, chain_cnt, tree_cnt;
	long long sum, suml, sumr, tree_sum; // 由于换根需要子树反转, 需要维护两个方向的信息
	bool rev, col;
	node *ch[2], *p;

	node() : w(0), chain_cnt(0), tree_cnt(0),sum(0),suml(0), sumr(0),
		tree_sum(0), rev(false), col(false) {}

	inline void pushdown() {
		if(!rev)
			return;

		ch[0]->rev ^= true;
		ch[1]->rev ^= true;
		swap(ch[0], ch[1]);
		swap(suml, sumr);

		rev = false;
	}

	inline void refresh() { // 如果不想这样特判就pushdown一下
		// pushdown();

		sum = ch[0] -> sum + ch[1] -> sum + w;
		suml = (ch[0] -> rev ? ch[0] -> sumr : ch[0] -> suml) + (ch[1] -> rev ? ch[1] -> sumr : ch[1] -> suml) + (tree_cnt + ch[1] -> chain_cnt) * (ch[0] -> sum + w) + tree_sum;
		sumr = (ch[0] -> rev ? ch[0] -> suml : ch[0] -> sumr) + (ch[1] -> rev ? ch[1] -> suml : ch[1] -> sumr) + (tree_cnt + ch[0] -> chain_cnt) * (ch[1] -> sum + w) + tree_sum;
		chain_cnt = ch[0] -> chain_cnt + ch[1] -> chain_cnt + tree_cnt;
	}
} null[maxn * 2]; // 如果没有边权变点权就不用乘2了

// 封装构造函数
node *newnode(int w) {
	node *x = nodes.front(); // 因为有删边加边, 可以用一个队列维护可用结点
	nodes.pop();
	initalize(x);
	x -> w = w;
	x -> refresh();
	return x;
}

// 封装初始化函数
// 记得在进行操作之前对所有结点调用一遍
inline void initalize(node *x) {
	*x = node();
	x -> ch[0] = x -> ch[1] = x -> p = null;
}

// 注意一下在Access的同时更新子树信息的方法
node *access(node *x) {
	node *y = null;

	while (x != null) {
		splay(x);

		x -> tree_cnt += x -> ch[1] -> chain_cnt - y -> chain_cnt;
		x -> tree_sum += (x -> ch[1] -> rev ? x -> ch[1] -> sumr : x -> ch[1] -> suml) - y -> suml;
		x -> ch[1] = y;

		(y = x) -> refresh();
		x = x -> p;
	}

	return y;
}

// 找到一个点所在连通块的根
// 对比原版没有变化
node *getroot(node *x) {
	x = access(x);
	
	while (x -> pushdown(), x -> ch[0] != null)
		x = x -> ch[0];
	splay(x);

	return x;
}

// 换根, 同样没有变化
void makeroot(node *x) {
	access(x);
	splay(x);
	x -> rev ^= true;
	x -> pushdown();
}

// 连接两个点
// !!! 注意这里必须把两者都变成根, 因为只能修改根结点
void link(node *x, node *y) {
	makeroot(x);
	makeroot(y);

	x -> p = y;
	y -> tree_cnt += x -> chain_cnt;
	y -> tree_sum += x -> suml;
	y -> refresh();
}

// 删除一条边
// 对比原版没有变化
void cut(node *x, node *y) {
	makeroot(x);
	access(y);
	splay(y);

	y -> ch[0] -> p = null;
	y -> ch[0] = null;
	y -> refresh();
}

// 修改/询问一个点, 这里以询问为例
// 如果是修改就在换根之后搞一些操作
long long query(node *x) {
	makeroot(x);
	return x -> suml;
}

// Splay函数
// 对比原版没有变化
void splay(node *x) {
	x -> pushdown();

	while (!isroot(x)) {
		if (!isroot(x -> p))
			x -> p -> p -> pushdown();
		x -> p -> pushdown();
		x -> pushdown();

		if (isroot(x -> p)) {
			rot(x -> p, dir(x) ^ 1);
			break;
		}

		if (dir(x) == dir(x -> p))
			rot(x -> p -> p, dir(x -> p) ^ 1);
		else
			rot(x -> p, dir(x) ^ 1);

		rot(x -> p, dir(x) ^ 1);
	}
}

// 旋转函数
// 对比原版没有变化
void rot(node *x, int d) {
	node *y = x -> ch[d ^ 1];
	
	if ((x -> ch[d^1] = y -> ch[d]) != null)
		y -> ch[d] -> p = x;

	y -> p = x -> p;
	if (!isroot(x))
		x -> p -> ch[dir(x)] = y;

	(y -> ch[d] = x) -> p = y;
	
	x -> refresh();
	y -> refresh();
}