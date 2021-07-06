struct node {
	int val, size;
	node *ch[2];

	node(int val) : val(val), size(1) {}

	inline void refresh() {
		size = ch[0] -> size + ch[1] -> size;
	}


} null[maxn];

node *copied(node *x) { // 如果不用可持久化的话, 直接用就行了
	return new node(*x);
}

node *merge(node *x, node *y) {
	if (x == null)
		return y;
	if (y == null)
		return x;
	
	node *z;
	if (rand() % (x -> size + y -> size) < x -> size) {
		z = copied(y);
		z -> ch[0] = merge(x, y -> ch[0]);
	}
	else {
		z = copied(x);
		z -> ch[1] = merge(x -> ch[1], y);
	}

	z -> refresh(); // 因为每次只有一边会递归到儿子, 所以z不可能取到null
	return z;
}

pair<node*, node*> split(node *x, int k) { // 左边大小为k
	if (x == null)
		return make_pair(null, null);
	
	pair<node*, node*> pi(null, null);

	if (k <= x -> ch[0] -> size) {
		pi = split(x -> ch[0], k);

		node *z = copied(x);
		z -> ch[0] = pi.second;
		z -> refresh();
		pi.second = z;
	}
	else {
		pi = split(x -> ch[1], k - x -> ch[0] -> size - 1);

		node *y = copied(x);
		y -> ch[1] = pi.first;
		y -> refresh();
		pi.first = y;
	}

	return pi;
}

// 记得初始化null
int main() {
	for (int i = 0; i <= n; i++)
		null[i].ch[0] = null[i].ch[1] = null;
	null -> size = 0;

	// do something

	return 0;
}