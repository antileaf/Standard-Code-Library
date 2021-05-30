// A为输入, C为输出, n为所需长度且必须是2^k
// 多项式求逆, 要求A常数项不为0
void get_inv(int *A, int *C, int n) {
	static int B[maxn];

	memset(C, 0, sizeof(int) * (n * 2));
	C[0] = qpow(A[0],p - 2); // 一般常数项都是1, 直接赋值为1就可以

	for (int k = 2; k <= n; k <<= 1) {
		memcpy(B, A, sizeof(int) * k);
		memset(B + k, 0, sizeof(int) * k);

		NTT(B, k * 2, 1);
		NTT(C,k * 2, 1);
		
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

	memset(C+n,0,sizeof(int)*n);
}

// 多项式exp, 要求A没有常数项
// 常数很大且总代码较长, 一般来说最好替换为分治FFT
// 分治FFT依据: 设G(x) = exp F(x), 则有 g_i = \sum_{k=1}^{i-1} f_{i-k} * k * g_k
void get_exp(int *A, int *C, int n) {
	static int B[maxn];

	memset(C, 0, sizeof(int) * (n * 2));
	C[0] = 1;

	for (int k = 2; k <= n; k <<= 1) {
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
void getdiv(int *A, int *B, int *C, int n, int m) {
	static int f[maxn], g[maxn], gi[maxn];

	assert(n >= m);

	int N = 1;
	while (N < (n - m + 1))
		N <<= 1;
	
	memset(f, 0, sizeof(int) * N);
	memset(g, 0, sizeof(int) * N);
	// memset(gi, 0, sizeof(int) * N);

	for (int i = 0; i < n - m + 1; i++)
		f[i] = A[n - i - 1];
	for (int i = 0; i < m && i < n - m + 1; i++)
		g[i] = B[m - i - 1];
	
	getinv(g, gi, N);

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
void getmod(int *A, int *B, int *C, int *D, int n, int m) {
	static int b[maxn], d[maxn];

	assert(n >= m);

	getdiv(A, B, d, n, m);
	
	if (D) { // D是商，可以选择不要
		for (int i = 0; i < n - m + 1; i++)
			D[i] = d[i];
	}

	int N = 1;
	while (N < n + m - 1)
		N <<= 1;
	
	memcpy(b, B, sizeof(int) * N);

	NTT(b, N, 1);
	NTT(d, N, 1);

	for (int i = 0; i < N; i++)
		b[i] = (long long)d[i] * b[i] % p;
	
	NTT(b, N, -1);

	for (int i = 0; i < m; i++)
		C[i] = (A[i] - b[i] + p) % p;
}