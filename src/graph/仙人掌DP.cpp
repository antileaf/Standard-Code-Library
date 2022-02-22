struct edge {
	int to, w, prev;
} e[maxn * 2];

vector<pair<int, int> > v[maxn];
vector<long long> d[maxn];
stack<int> stk;

int p[maxn];
bool vis[maxn], vise[maxn * 2];
int last[maxn], cnte;

long long f[maxn], g[maxn], sum[maxn];
int n, m, cnt;

void addedge(int x, int y, int w) {
	v[x].push_back(make_pair(y, w));
}

void dfs(int x) {

	vis[x] = true;
	
	for (int i = last[x]; ~i; i = e[i].prev) {
		if (vise[i ^ 1])
			continue;
			
		int y = e[i].to, w = e[i].w;
		
		vise[i] = true;
		
		if (!vis[y]) {
			stk.push(i);
			p[y] = x;
			dfs(y);
			
			if (!stk.empty() && stk.top() == i) {
				stk.pop();
				addedge(x, y, w);
			}
		}
		
		else {
			cnt++;
			
			long long tmp = w;
			while (!stk.empty()) {
				int i = stk.top();
				stk.pop();
				
				int yy = e[i].to, ww = e[i].w;
				
				addedge(cnt, yy, 0);
				
				d[cnt].push_back(tmp);
				
				tmp += ww;
				
				if (e[i ^ 1].to == y)
					break;
			}
			
			addedge(y, cnt, 0);
			
			sum[cnt] = tmp;
		}
	}
}
​
void dp(int x) {
	
	for (auto o : v[x]) {
		int y = o.first, w = o.second;
		dp(y);
	}
	
	if (x <= n) {
		for (auto o : v[x]) {
			int y = o.first, w = o.second;
			
			f[x] += 2 * w + f[y];
		}
		
		g[x] = f[x];
		
		for (auto o : v[x]) {
			int y = o.first, w = o.second;
			
			g[x] = min(g[x], f[x] - f[y] - 2 * w + g[y] + w);
		}
	}
	else {
		f[x] = sum[x];
		for (auto o : v[x]) {
			int y = o.first;
			
			f[x] += f[y];
		}
		
		g[x] = f[x];
		
		for (int i = 0; i < (int)v[x].size(); i++) {
			int y = v[x][i].first;
			
			g[x] = min(g[x], f[x] - f[y] + g[y] + min(d[x][i], sum[x] - d[x][i]));
		}
	}
}
