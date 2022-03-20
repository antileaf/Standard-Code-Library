int ex[maxn], id[maxn];

int main() {

	memset(last, -1, sizeof(last));

	int n, m, src, sink;
	scanf("%d%d%d%d", &n, &m, &src, &sink);
	s = n + 1;
	t = n + 2;

	while (m--) {
		int x, y, b, c;
		scanf("%d%d%d%d", &x, &y, &b, &c);

		addedge(x, y, c - b);
		
		ex[y] += b;
		ex[x] -= b;
	}
	
	for (int i = 1; i <= n; i++) {
		id[i] = cnte;
		
		if (ex[i] >= 0)
			addedge(s, i, ex[i]);
		else
			addedge(i, t, -ex[i]);
	}

	addedge(sink, src, (~0u) >> 1);
	
	Dinic();

	if (any_of(id + 1, id + n + 1, [] (int i) {return (bool)e[i].cap;}))
		printf("please go home to sleep\n");
	else {
		int flow = e[cnte - 1].cap;
		e[cnte - 1].cap = e[cnte - 2].cap = 0;
		s = src;
		t = sink;

		printf("%d\n", flow + Dinic());
	}

	return 0;
}