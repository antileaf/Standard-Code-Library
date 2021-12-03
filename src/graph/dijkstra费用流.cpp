struct edge {
	int to, cap, prev, w;
} e[maxe * 2];

int last[maxn], cnte;

long long d[maxn], h[maxn];
int p[maxn];

bool vis[maxn];
int s, t;

void Adde(int x, int y, int z, int w) {
	e[cnte].to = y;
	e[cnte].cap = z;
	e[cnte].w = w;
	e[cnte].prev = last[x];
	last[x] = cnte++;
}

void addedge(int x, int y, int z, int w) {
	Adde(x, y, z, w);
	Adde(y, x, 0, -w);
}

void dijkstra() {
	memset(d, 63, sizeof(d));
	memset(vis, 0, sizeof(vis));
	
	priority_queue<pair<long long, int> > heap;

	d[s] = 0;
	heap.push(make_pair(0ll, s));
	
	while (!heap.empty()) {
		int x = heap.top().second;
		heap.pop();

		if (vis[x])
			continue;
		
		vis[x] = true;
		for (int i = last[x]; ~i; i = e[i].prev)
			if (e[i].cap > 0 && d[e[i].to] > d[x] + e[i].w + h[x] - h[e[i].to]) {
				d[e[i].to] = d[x] + e[i].w + h[x] - h[e[i].to];
				p[e[i].to] = i;
				heap.push(make_pair(-d[e[i].to], e[i].to));
			}
	}
}

pair<long long, long long> mcmf() {
	/*
	spfa();
	for (int i = 1; i <= t; i++)
		h[i] = d[i];
	// 如果初始有负权就像这样跑一遍SPFA预处理
	*/

	long long flow = 0, cost = 0;

	while (dijkstra(), d[t] < 0x3f3f3f3f) {
		for (int i = 1; i <= t; i++)
			h[i] += d[i];

		int a = 0x3f3f3f3f;
		
		for (int x = t; x != s; x = e[p[x] ^ 1].to)
			a = min(a, e[p[x]].cap);
		
		flow += a;
		cost += (long long)a * h[t];

		for (int x = t; x != s; x = e[p[x] ^ 1].to) {
			e[p[x]].cap -= a;
			e[p[x] ^ 1].cap += a;
		}

	}

	return make_pair(flow, cost);
}

// 记得初始化
memset(last, -1, sizeof(last));