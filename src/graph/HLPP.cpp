constexpr int maxn = 1205, maxe = 120005;

struct edge {
	int to, cap, prev;
} e[maxe * 2];

int n, m, s, t;
int last[maxn], cnte;
int h[maxn], gap[maxn * 2];
long long ex[maxn]; // 多余流量
bool inq[maxn];

struct cmp {
	bool operator() (int x, int y) const {
		return h[x] < h[y];
	}
};

priority_queue<int, vector<int>, cmp> heap;

void adde(int x, int y, int z) {
	e[cnte].to = y;
	e[cnte].cap = z;
	e[cnte].prev = last[x];
	last[x] = cnte++;
}

void addedge(int x, int y, int z) {
	adde(x, y, z);
	adde(y, x, 0);
}

bool bfs() {
	static int q[maxn];

	fill(h, h + n + 1, 2 * n); // 如果没有全局的n, 记得改这里
	int head = 0, tail = 0;
	q[tail++] = t;
	h[t] = 0;
	
	while (head < tail) {
		int x = q[head++];
		for (int i = last[x]; ~i; i = e[i].prev)
			if (e[i ^ 1].cap && h[e[i].to] > h[x] + 1) {
				h[e[i].to] = h[x] + 1;
				q[tail++] = e[i].to;
			}
	}

	return h[s] < 2 * n;
}

void push(int x) {
	for (int i = last[x]; ~i; i = e[i].prev)
		if (e[i].cap && h[x] == h[e[i].to] + 1) {
			int d = min(ex[x], (long long)e[i].cap);

			e[i].cap -= d;
			e[i ^ 1].cap += d;
			ex[x] -= d;
			ex[e[i].to] += d;

			if (e[i].to != s && e[i].to != t && !inq[e[i].to]) {
				heap.push(e[i].to);
				inq[e[i].to] = true;
			}

			if (!ex[x])
				break;
		}
}

void relabel(int x) {
	h[x] = 2 * n;
	
	for (int i = last[x]; ~i; i = e[i].prev)
		if (e[i].cap)
			h[x] = min(h[x], h[e[i].to] + 1);
}

long long hlpp() {
	if (!bfs())
		return 0;
	
	// memset(gap, 0, sizeof(int) * 2 * n);
	h[s] = n;

	for (int i = 1; i <= n; i++)
		gap[h[i]]++;
	
	for (int i = last[s]; ~i; i = e[i].prev)
		if (e[i].cap) {
			int d = e[i].cap;

			e[i].cap -= d;
			e[i ^ 1].cap += d;
			ex[s] -= d;
			ex[e[i].to] += d;

			if (e[i].to != s && e[i].to != t && !inq[e[i].to]) {
					heap.push(e[i].to);
					inq[e[i].to] = true;
			}
		}
	
	while (!heap.empty()) {
		int x = heap.top();
		heap.pop();
		inq[x] = false;

		push(x);
		if (ex[x]) {
			if (!--gap[h[x]]) { // gap
				for (int i = 1; i <= n; i++)
				 	if (i != s && i != t && h[i] > h[x])
					 	h[i] = n + 1;
			}

			relabel(x);
			++gap[h[x]];
			heap.push(x);
			inq[x] = true;
		}	
	}

	return ex[t];
}

//记得初始化
memset(last, -1, sizeof(last));