int q[maxn], ans[maxn]; // q是要代入的各个系数, ans是求出的值
int tg[25][maxn * 2], tf[25][maxn]; // 辅助数组, tg是预处理乘积,
// tf是项数越来越少的f, tf[0]就是原来的函数

void pretreat(int l, int r, int k) { // 预处理
	static int A[maxn], B[maxn];

	int *g = tg[k] + l * 2;

	if (r - l + 1 <= 1) { // 小范围暴力
		g[0] = 1;
		
		for (int i = l; i <= r; i++) {
			for (int j = i - l + 1; j; j--) {
				g[j] = (g[j - 1] - (long long)g[j] * q[i]) % p;
				if (g[j] < 0)
					g[j] += p;
			}
			g[0] = (long long)g[0] * (p - q[i]) % p;
		}

		reverse(g, g + r - l + 2);

		return;
	}

	int mid = (l + r) / 2;

	pretreat(l, mid, k + 1);
	pretreat(mid + 1, r, k + 1);
	
	int N = 1;
	while (N <= r - l + 1)
		N *= 2;
	
	int *gl = tg[k + 1] + l * 2, *gr = tg[k + 1] + (mid + 1) * 2;
	
	memset(A, 0, sizeof(int) * N);
	memset(B, 0, sizeof(int) * N);

	memcpy(A, gl, sizeof(int) * (mid - l + 2));
	memcpy(B, gr, sizeof(int) * (r - mid + 1));

	NTT(A, N, 1);
	NTT(B, N, 1);

	for (int i = 0; i < N; i++)
		A[i] = (long long)A[i] * B[i] % p;
	
	NTT(A, N, -1);

	for (int i = 0; i <= r - l + 1; i++)
		g[i] = A[i];
}

void solve(int l, int r, int k) { // 主过程
	static int a[maxn], b[maxn];

	int *f = tf[k];

	if (l == r) {
		ans[l] = f[0];
		return;
	}

	int mid = (l + r) / 2;
	int *ff = tf[k + 1], *gl = tg[k + 1] + l * 2, *gr = tg[k + 1] + (mid + 1) * 2;

	int N = 1;
	while (N < r - l + 2)
		N *= 2;
	
	memcpy(a, f, sizeof(int) * (r - l + 2));
	memcpy(b, gr, sizeof(int) * (r - mid + 1));
	reverse(b, b + r - mid + 1);

	NTT(a, N, 1);
	NTT(b, N, 1);
	for (int i = 0; i < N; i++)
		b[i] = (long long)a[i] * b[i] % p;
	
	reverse(b + 1, b + N);
	NTT(b, N, 1);
	int n_inv = qpow(N, p - 2);
	for (int i = 0; i < N; i++)
		b[i] = (long long)b[i] * n_inv % p;
	
	for (int i = 0; i < mid - l + 2; i++)
		ff[i] = b[i + r - mid];
	
	memset(a, 0, sizeof(int) * N);
	memset(b, 0, sizeof(int) * N);

	solve(l, mid, k + 1);

	memset(ff, 0, sizeof(int) * (mid - l + 2));

	memcpy(a, f, sizeof(int) * (r - l + 2));
	memcpy(b, gl, sizeof(int) * (mid - l + 2));
	reverse(b, b + mid - l + 2);

	NTT(a, N, 1);
	NTT(b, N, 1);
	for (int i = 0; i < N; i++)
		b[i] = (long long)a[i] * b[i] % p;

	reverse(b + 1, b + N);
	NTT(b, N, 1);
	for (int i = 0; i < N; i++)
		b[i] = (long long)b[i] * n_inv % p;

	for (int i = 0; i < r - mid + 1; i++)
		ff[i] = b[i + mid - l + 1];

	memset(a, 0, sizeof(int) * N);
	memset(b, 0, sizeof(int) * N);

	solve(mid + 1, r, k + 1);

	memset(gl, 0, sizeof(int) * (mid - l + 2));
	memset(gr, 0, sizeof(int) * (r - mid + 1));
	memset(ff, 0, sizeof(int) * (r - mid + 1));
}

// f < x^n, m个询问, 0-based
void get_value(int *f, int *x, int *a, int n, int m) {
	static int c[maxn], d[maxn];

	if (m <= n)
		m = n + 1;
	if (n < m - 1)
		n = m - 1; // 补零

	memcpy(q, x, sizeof(int) * m);

	pretreat(0, m - 1, 0);

	int N = 1;
	while (N < m)
		N *= 2;
	
	get_inv(tg[0], c, N);
	
	fill(c + m, c + N, 0);
	reverse(c, c + m);

	memcpy(d, f, sizeof(int) * m);

	NTT(c, N * 2, 1);
	NTT(d, N * 2, 1);
	for (int i = 0; i < N * 2; i++)
		c[i] = (long long)c[i] * d[i] % p;
	NTT(c, N * 2, -1);

	for (int i = 0; i < m; i++)
		tf[0][i] = c[i + n];

	solve(0, m - 1, 0);

	if (a) // 如果a是nullptr, 代表不复制答案, 直接用ans数组
		memcpy(a, ans, sizeof(int) * m);
}