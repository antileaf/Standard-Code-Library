// KM Weighted Bio-Graph Matching KM二分图最大权匹配
// By AntiLeaf
// O(n^3)

const long long INF = 0x3f3f3f3f3f3f3f3f;

long long w[maxn][maxn], lx[maxn], ly[maxn], slack[maxn];
// 边权 顶标 slack
// 如果要求最大权完美匹配就把不存在的边设为-INF 否则所有边对0取max

bool visx[maxn], visy[maxn];

int boy[maxn], girl[maxn], p[maxn], q[maxn], head, tail; // p : pre

int n, m, N, e;

// 增广
bool check(int y) {
	visy[y] = true;
	
	if (boy[y]) {
		visx[boy[y]] = true;
		q[tail++] = boy[y];
		return false;
	}

	while (y) {
		boy[y] = p[y];
		swap(y, girl[p[y]]);
	}

	return true;
}

// bfs每个点
void bfs(int x) {
	memset(q, 0, sizeof(q));
	head = tail = 0;
	
	q[tail++] = x;
	visx[x] = true;

	while (true) {
		while (head != tail) {
			int x = q[head++];

			for (int y = 1; y <= N; y++)
				if (!visy[y]) {
					long long d = lx[x] + ly[y] - w[x][y];

					if (d < slack[y]) {
						p[y] = x;
						slack[y] = d;

						if (!slack[y] && check(y))
							return;
					}
				}
		}

		long long d = INF;
		for (int i = 1; i <= N; i++)
			if (!visy[i])
				d = min(d, slack[i]);

		for (int i = 1; i <= N; i++) {
			if (visx[i])
				lx[i] -= d;
				
			if (visy[i])
				ly[i] += d;
			else
				slack[i] -= d;
		}

		for (int i = 1; i <= N; i++)
			if (!visy[i] && !slack[i] && check(i))
				return;
	}
}

// 主过程
long long KM() {
	for (int i = 1; i <= N; i++) {
		// lx[i] = 0;
		ly[i] = -INF;
		// boy[i] = girl[i] = -1;

		for (int j = 1; j <= N; j++)
			ly[i] = max(ly[i], w[j][i]);
	}

	for (int i = 1; i <= N; i++) {
		memset(slack, 0x3f, sizeof(slack));
		memset(visx, 0, sizeof(visx));
		memset(visy, 0, sizeof(visy));
		bfs(i);
	}

	long long ans = 0;
	for (int i = 1; i <= N; i++)
		ans += w[i][girl[i]];
	return ans;
}

// 为了方便贴上主函数
int main() {

	scanf("%d%d%d", &n, &m, &e);
	N = max(n, m);
	
	while (e--) {
		int x, y, c;
		scanf("%d%d%d", &x, &y, &c);
		w[x][y] = max(c, 0);
	}

	printf("%lld\n", KM());

	for (int i = 1; i <= n; i++) {
		if (i > 1)
			printf(" ");
		printf("%d", w[i][girl[i]] > 0 ? girl[i] : 0);
	}
	printf("\n");

	return 0;
}