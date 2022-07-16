vector<int> G[maxn], C[maxn], v[maxn]; // C是边的编号
int cur[maxn];
bool vis[maxn * 2];

vector<pair<int, int> > vec;

int d[maxn];

void dfs(int x) {
	bool bad = false;

	while (!bad) {
		bad = true;

		for (int &i = cur[x]; i < (int)G[x].size(); i++)
			if (!vis[C[x][i]]) {
				vis[C[x][i]] = true;
				vec.emplace_back(x, i);
				x = G[x][i];
				bad = false;

				break;
			}
	}
}