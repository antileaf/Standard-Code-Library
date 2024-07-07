constexpr int maxn = 20005, maxm = 200005;
 
struct edge {
	int to, prev, cap, w;
} e[maxm * 2];
 
int last[maxn], cnte, d[maxn], p[maxn]; // 记得把last初始化成-1, 不然会死循环
bool inq[maxn];
 
void spfa(int s) {
 
	memset(d, -63, sizeof(d));
	memset(p, -1, sizeof(p));
 
	queue<int> q;
	
	q.push(s);
	d[s] = 0;
 
	while (!q.empty()) {
		int x = q.front();
		q.pop();
		inq[x] = false;
 
		for (int i = last[x]; ~i; i = e[i].prev)
			if (e[i].cap) {
				int y = e[i].to;
 
				if (d[x] + e[i].w > d[y]) {
					p[y] = i;
					d[y] = d[x] + e[i].w;
					if (!inq[y]) {
						q.push(y);
						inq[y] = true;
					}
				}
			}
	}
}
 
int mcmf(int s, int t) {
	int ans = 0;
 
	while (spfa(s), d[t] > 0) {
		int flow = 0x3f3f3f3f;
		for (int x = t; x != s; x = e[p[x] ^ 1].to) 
			flow = min(flow, e[p[x]].cap);
		
		ans += flow * d[t];
 
		for (int x = t; x != s; x = e[p[x] ^ 1].to) {
			e[p[x]].cap -= flow;
			e[p[x] ^ 1].cap += flow;
		}
	}
 
	return ans;
}
 
void add(int x, int y, int c, int w) {
	e[cnte].to = y;
	e[cnte].cap = c;
	e[cnte].w = w;
 
	e[cnte].prev = last[x];
	last[x] = cnte++;
}
 
void addedge(int x, int y, int c, int w) {
	add(x, y, c, w);
	add(y, x, 0, -w);
}
