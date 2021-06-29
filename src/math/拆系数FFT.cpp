constexpr int maxn = 262200, p = 23333333, M = 4830; // M取值要使得结果不超过10^14

// 需要开的数组
struct Complex {
	// 内容略
} w[maxn], w_inv[maxn], A[maxn], B[maxn], C[maxn], D[maxn], F[maxn], G[maxn], H[maxn];

// 主函数(当然更多时候包装一下比较好)
// 需要调用FFT初始化, FFT
int main() {
	scanf("%d", &n);

	int N = 1;
	while (N < n * 2)
		N *= 2;

	for (int i = 0, x; i < n; i++) {
		scanf("%d", &x);
		A[i] = x / M;
		B[i] = x % M;
	}

	for (int i = 0, x; i < n; i++) {
		scanf("%d", &x);
		C[i] = x / M;
		D[i] = x % M;
	}

	FFT_init(N);

	FFT(A, N, 1);
	FFT(B, N, 1);
	FFT(C, N, 1);
	FFT(D, N, 1);

	for (int i = 0; i < N; i++) {
		F[i] = A[i] * C[i];
		G[i] = A[i] * D[i] + B[i] * C[i];
		H[i] = B[i] * D[i];
	}

	FFT(F, N, -1);
	FFT(G, N, -1);
	FFT(H, N, -1);

	for (int i = 0; i < n; i++)
		printf("%d ", (int)((M * M * ((long long)(F[i].a + 0.5) % p) % p + M * ((long long)(G[i].a + 0.5) % p) % p + (long long)(H[i].a + 0.5) % p) % p));
	
	return 0;
}
