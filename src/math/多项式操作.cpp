// A为输入, C为输出, n为所需长度且必须是2^k
// 多项式求逆, 要求A常数项不为0
void get_inv(int *A, int *C, int n) {
	static int B[maxn];

	memset(C, 0, sizeof(int) * (n * 2));
	C[0] = qpow(A[0], p - 2); // 一般常数项都是1, 直接赋值为1就可以

	for (int k = 2; k <= n; k *= 2) {
		memcpy(B, A, sizeof(int) * k);
		memset(B + k, 0, sizeof(int) * k);

		NTT(B, k * 2, 1);
		NTT(C, k * 2, 1);
		
		for (int i = 0; i < k * 2; i++) {
			C[i] = (2 - (long long)B[i] * C[i]) % p * C[i] % p;
			if (C[i] < 0)
				C[i] += p;
		}
		
		NTT(C, k * 2, -1);

		memset(C + k, 0, sizeof(int) * k);
	}
}

// 开根
void get_sqrt(int *A, int *C, int n) {
	static int B[maxn], D[maxn];
	
	memset(C, 0, sizeof(int) * (n * 2));
	C[0] = 1; // 如果不是1就要考虑二次剩余

	for (int k = 2; k <= n; k *= 2) {
		memcpy(B, A, sizeof(int) * k);
		memset(B + k, 0, sizeof(int) * k);
		
		get_inv(C, D, k);

		NTT(B, k * 2, 1);
		NTT(D, k * 2, 1);

		for (int i = 0; i < k * 2; i++)
			B[i] = (long long)B[i] * D[i]%p;
			
		NTT(B, k * 2, -1);

		for (int i = 0; i < k; i++)
			C[i] = (long long)(C[i] + B[i]) * inv_2 % p;//inv_2是2的逆元
	}
}

// 求导
void get_derivative(int *A, int *C, int n) {
	for (int i = 1; i < n; i++)
		C[i - 1] = (long long)A[i] * i % p;
	
	C[n - 1] = 0;
}

// 不定积分, 最好预处理逆元
void get_integrate(int *A, int *C, int n) {
	for (int i = 1; i < n; i++)
		C[i] = (long long)A[i - 1] * qpow(i, p - 2) % p;
	
	C[0] = 0; // 不定积分没有常数项
}

// 多项式ln, 要求A常数项不为0
void get_ln(int *A, int *C, int n) { // 通常情况下A常数项都是1
	static int B[maxn];

	get_derivative(A, B, n);
	memset(B + n, 0, sizeof(int) * n);

	get_inv(A, C, n);

	NTT(B, n * 2, 1);
	NTT(C, n * 2, 1);

	for (int i = 0;i < n * 2; i++)
		B[i] = (long long)B[i] * C[i] % p;

	NTT(B, n * 2, -1);

	get_integrate(B, C, n);

	memset(C + n, 0, sizeof(int) * n);
}

// 多项式exp, 要求A没有常数项
// 常数很大且总代码较长, 一般来说最好替换为分治FFT
// 分治FFT依据: 设$G(x) = \exp F(x)$, 则有 $g_i = \sum_{k=1}^{i-1} f_{i-k} * k * g_k$
void get_exp(int *A, int *C, int n) {
	static int B[maxn];

	memset(C, 0, sizeof(int) * (n * 2));
	C[0] = 1;

	for (int k = 2; k <= n; k *= 2) {
		get_ln(C, B, k);

		for (int i = 0; i < k; i++) {
			B[i] = A[i] - B[i];
			if (B[i] < 0)
				B[i] += p;
		}
		(++B[0]) %= p;

		NTT(B, k * 2, 1);
		NTT(C, k * 2, 1);

		for (int i = 0; i < k * 2; i++)
			C[i] = (long long)C[i] * B[i] % p;

		NTT(C, k * 2, -1);

		memset(C + k, 0, sizeof(int) * k);
	}
}

// 多项式k次幂, 在A常数项不为1时需要转化
// 常数较大且总代码较长, 在时间要求不高时最好替换为暴力快速幂
void get_pow(int *A, int *C, int n, int k) {
	static int B[maxn];

	get_ln(A, B, n);

	for (int i = 0; i < n; i++)
		B[i] = (long long)B[i] * k % p;

	get_exp(B, C, n);
}

// 多项式除法, A / B, 结果输出在C
// A的次数为n, B的次数为m
void get_div(int *A, int *B, int *C, int n, int m) {
	static int f[maxn], g[maxn], gi[maxn];

	if (n < m) {
		memset(C, 0, sizeof(int) * m);
		return;
	}

	int N = 1;
	while (N < (n - m + 1))
		N *= 2;
	
	memset(f, 0, sizeof(int) * N * 2);
	memset(g, 0, sizeof(int) * N * 2);
	// memset(gi, 0, sizeof(int) * N);

	for (int i = 0; i < n - m + 1; i++)
		f[i] = A[n - i - 1];
	for (int i = 0; i < m && i < n - m + 1; i++)
		g[i] = B[m - i - 1];
	
	get_inv(g, gi, N);

	for (int i = n - m + 1; i < N; i++)
		gi[i] = 0;
	
	NTT(f, N * 2, 1);
	NTT(gi, N * 2, 1);
	
	for (int i = 0; i < N * 2; i++)
		f[i] = (long long)f[i] * gi[i] % p;
	
	NTT(f, N * 2, -1);

	for (int i = 0; i < n - m + 1; i++)
		C[i] = f[n - m - i];
}

// 多项式取模, 余数输出到C, 商输出到D
void get_mod(int *A, int *B, int *C, int *D, int n, int m) {
	static int b[maxn], d[maxn];

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
	
	memcpy(b, B, sizeof(int) * m);

	NTT(b, N, 1);
	NTT(d, N, 1);

	for (int i = 0; i < N; i++)
		b[i] = (long long)d[i] * b[i] % p;
	
	NTT(b, N, -1);

	for (int i = 0; i < m - 1; i++)
		C[i] = (A[i] - b[i] + p) % p;
	
	memset(b, 0, sizeof(int) * N);
	memset(d, 0, sizeof(int) * N);
}

// 多点求值要用的数组
int q[maxn], ans[maxn]; // q是要代入的各个系数, ans是求出的值
int tg[25][maxn * 2], tf[25][maxn]; // 辅助数组, tg是预处理乘积
// tf是项数越来越少的f, tf[0]就是原来的函数

void pretreat(int l, int r, int k) { // 多点求值预处理
	static int A[maxn], B[maxn];

	int *g = tg[k] + l * 2;

	if (r - l + 1 <= 200) { // 小范围暴力, 能跑得快点
		g[0] = 1;
		
		for (int i = l; i <= r; i++) {
			for (int j = i - l + 1; j; j--) {
				g[j] = (g[j - 1] - (long long)g[j] * q[i]) % p;
				if (g[j] < 0)
					g[j] += p;
			}
			g[0] = (long long)g[0] * (p - q[i]) % p;
		}

		return;
	}

	int mid = (l + r) / 2;

	pretreat(l, mid, k + 1);
	pretreat(mid + 1, r, k + 1);

	if (!k)
		return;
	
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

void solve(int l, int r, int k) { // 多项式多点求值主过程
	int *f = tf[k];

	if (r - l + 1 <= 200) {
		for (int i = l; i <= r; i++) {
			int x = q[i];

			for (int j = r - l; ~j; j--)
				ans[i] = ((long long)ans[i] * x + f[j]) % p;
		}

		return;
	}

	int mid = (l + r) / 2;
	int *ff = tf[k + 1], *gl = tg[k + 1] + l * 2, *gr = tg[k + 1] + (mid + 1) * 2;

	get_mod(f, gl, ff, nullptr, r - l + 1, mid - l + 2);
	solve(l, mid, k + 1);

	memset(gl, 0, sizeof(int) * (mid - l + 2));
	memset(ff, 0, sizeof(int) * (mid - l + 1));

	get_mod(f, gr, ff, nullptr, r - l + 1, r - mid + 1);
	solve(mid + 1, r, k + 1);

	memset(gr, 0, sizeof(int) * (r - mid + 1));
	memset(ff, 0, sizeof(int) * (r - mid));
}

// f < x^n, m个询问, 询问是0-based, 当然改成1-based也很简单
void get_value(int *f, int *x, int *a, int n, int m) {
	if (m <= n)
		m = n + 1;
	if (n < m - 1)
		n = m - 1; // 补零方便处理

	memcpy(tf[0], f, sizeof(int) * n);
	memcpy(q, x, sizeof(int) * m);

	pretreat(0, m - 1, 0);
	solve(0, m - 1, 0);

	if (a) // 如果a是nullptr, 代表不复制答案, 直接用ans数组
		memcpy(a, ans, sizeof(int) * m);
}