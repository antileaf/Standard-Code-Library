#define dir(x) ((x) == (x) -> p -> ch[1])

struct node {
	int size;
	bool rev;
	node *ch[2],*p;

	node() : size(1), rev(false) {}

	void pushdown() {
		if(!rev)
			return;

		ch[0] -> rev ^= true;
		ch[1] -> rev ^= true;
		swap(ch[0], ch[1]);

		rev=false;
	}

	void refresh() {
		size = ch[0] -> size + ch[1] -> size + 1;
	}
} null[maxn], *root = null;

void rot(node *x, int d) {
	node *y = x -> ch[d ^ 1];

	if ((x -> ch[d ^ 1] = y -> ch[d]) != null)
		y -> ch[d] -> p = x;
	((y -> p = x -> p) != null ? x -> p -> ch[dir(x)] : root) = y;
	(y -> ch[d] = x) -> p = y;

	x -> refresh();
	y -> refresh();
}

void splay(node *x, node *t) {
	while (x -> p != t) {
		if (x -> p -> p == t) {
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

node *kth(int k, node *o) {
	int d;
	k++; // 因为最左边有一个哨兵

	while (o != null) {
		o -> pushdown();

		if (k == o -> ch[0] -> size + 1)
			return o;

		if ((d = k > o -> ch[0] -> size))
			k -= o -> ch[0] -> size + 1;
		o = o -> ch[d];
	}

	return null;
}

void reverse(int l, int r) {
	splay(kth(l - 1));
	splay(kth(r + 1), root);

	root -> ch[1] -> ch[0] -> rev ^= true;
}

int n, m;

int main() {
	null -> size = 0;
	null -> ch[0] = null -> ch[1] = null -> p = null;

	scanf("%d%d", &n, &m);
	root = null + n + 1;
	root -> ch[0] = root -> ch[1] = root -> p = null;

	for (int i = 1; i <= n; i++) {
		null[i].ch[1] = null[i].p = null;
		null[i].ch[0] = root;
		root -> p = null + i;
		(root = null + i) -> refresh();
	}

	null[n + 2].ch[1] = null[n + 2].p = null;
	null[n + 2].ch[0] = root; // 这里直接建成一条链的, 如果想减少常数也可以递归建一个平衡的树
	root -> p = null + n + 2; // 总之记得建两个哨兵, 这样splay起来不需要特判
	(root = null + n + 2) -> refresh();

	// Do something

	return 0;
}