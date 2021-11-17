int tim; // tim实际上就是当前的右端点

node *access(node *x) {
	node *y = null;

	while (x != null) {
		splay(x);

		x -> ch[1] = null;
		x -> refresh();

		if (x -> val) // val记录的是上次访问时间, 也就是right集合最大值
			update(x -> val - val[x -> r] + 1, x -> val - val[par[x -> l]], -1);
		
		x -> val = tim;
		x -> lazy = true;

		update(x -> val - val[x -> r] + 1, x -> val - val[par[x -> l]], 1);

		x -> ch[1] = y;

		(y = x) -> refresh();

		x = x -> p;
	}

	return y;
}

// 以下是main函数中的用法
for (int i = 1; i <= n; i++) {
	tim++;
	access(null + id[i]);

	if (i >= m) // 例题询问长度是固定的, 如果不固定的话就按照右端点离线即可
		ans[i - m + 1] = query(i - m + 1, i);
}