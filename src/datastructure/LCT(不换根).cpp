#define isroot(x) ((x) != (x) -> p -> ch[0] && (x) != (x) -> p -> ch[1]) // 判断是不是Splay的根
#define dir(x) ((x) == (x) -> p -> ch[1]) // 判断它是它父亲的左 / 右儿子

struct node { // 结点类定义
	int size; // Splay的子树大小
	node *ch[2], *p;

	node() : size(1) {}
	void refresh() {
		size = ch[0] -> size + ch[1] -> size + 1;
	} // 附加信息维护
} null[maxn];

// 在主函数开头加上这句初始化
null -> size = 0;

// 初始化结点
void initalize(node *x) {
	x -> ch[0] = x -> ch[1] = x -> p = null;
}

// Access 均摊O(\log n)
// LCT核心操作, 把结点到根的路径打通, 顺便把与重儿子的连边变成轻边
// 需要调用splay
node *access(node *x) {
	node *y = null;

	while (x != null) {
		splay(x);

		x -> ch[1] = y;
		(y = x) -> refresh();

		x = x -> p;
	}

	return y;
}

// Link 均摊O(\log n)
// 把x的父亲设为y
// 要求x必须为所在树的根节点，否则会导致后续各种莫名其妙的问题
// 需要调用splay
void link(node *x, node *y) {
	splay(x);
	x -> p = y;
}

// Cut 均摊O(\log n)
// 把x与其父亲的连边断掉
// x可以是所在树的根节点, 这时此操作没有任何实质效果
// 需要调用access和splay
void cut(node *x) {
	access(x);
	splay(x);

	x -> ch[0] -> p = null;
	x -> ch[0] = null;

	x -> refresh();
}

// Splay 均摊O(\log n)
// 需要调用旋转
void splay(node *x) {
	while (!isroot(x)) {
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

// 旋转(LCT版本) O(1)
// 平衡树基本操作
// 要求对应儿子必须存在, 否则会导致后续各种莫名其妙的问题
void rot(node *x, int d) {
	node *y = x -> ch[d ^ 1];

	y -> p = x -> p;
	if (!isroot(x))
		x -> p -> ch[dir(x)] = y;

	if ((x -> ch[d ^ 1] = y -> ch[d]) != null)
		y -> ch[d] -> p = x;
	(y -> ch[d] = x) -> p = y;

	x -> refresh();
	y -> refresh();
}
