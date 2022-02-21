// 多项式取模, 余数输出到C, 商输出到D
void get_mod(int *A, int *B, int *C, int *D, int n, int m) {
	static int b[maxn], d[maxn];
	static bool flag = false;

	if (n < m) {
		memcpy(C, A, sizeof(int) * n);
		
		if (D)
			memset(D, 0, sizeof(int) * m);

		return;
	}

	get_div(A, B, d, n, m);
	
	if (D) { // D是商, 可以选择不要
		for (int i = 0; i < n - m + 1; i++)
			D[i] = d[i];
	}

	int N = 1;
	while (N < n)
		N *= 2;
	
	if (!flag) {
		memcpy(b, B, sizeof(int) * m);
		NTT(b, N, 1);

		flag = true;
	}

	NTT(d, N, 1);

	for (int i = 0; i < N; i++)
		d[i] = (long long)d[i] * b[i] % p;
	
	NTT(d, N, -1);

	for (int i = 0; i < m - 1; i++)
		C[i] = (A[i] - d[i] + p) % p;
	
	// memset(b, 0, sizeof(int) * N);
	memset(d, 0, sizeof(int) * N);
}

// g < x^n, f是输出答案的数组
void pow_mod(long long k, int *g, int n, int *f) {
	static int a[maxn], t[maxn];

	memset(f, 0, sizeof(int) * (n * 2));

	f[0] = a[1] = 1;

	int N = 1;
	while (N < n * 2 - 1)
		N *= 2;

	while (k) {
		NTT(a, N, 1);
		
		if (k & 1) {
			memcpy(t, f, sizeof(int) * N);

			NTT(t, N, 1);
			for (int i = 0; i < N; i++)
				t[i] = (long long)t[i] * a[i] % p;
			NTT(t, N, -1);
			
			get_mod(t, g, f, NULL, n * 2 - 1, n);
		}

		for (int i = 0; i < N; i++)
			a[i] = (long long)a[i] * a[i] % p;
		NTT(a, N, -1);

		memcpy(t, a, sizeof(int) * (n * 2 - 1));
		get_mod(t, g, a, NULL, n * 2 - 1, n);
		fill(a + n - 1, a + N, 0);

		k >>= 1;
	}
	
	memset(a, 0, sizeof(int) * (n * 2));
}

// $f_n = \sum_{i = 1} ^ m  f_{n - i} a_i$
// f是0~m-1项的初值
int linear_recurrence(long long n, int m, int *f, int *a) {
	static int g[maxn], c[maxn];

	memset(g, 0, sizeof(int) * (m * 2 + 1));

	for (int i = 0; i < m; i++)
		g[i] = (p - a[m - i]) % p;
	g[m] = 1;

	pow_mod(n, g, m + 1, c);

	int ans = 0;
	for (int i = 0; i < m; i++)
		ans = (ans + (long long)c[i] * f[i]) % p;
	
	return ans;
}