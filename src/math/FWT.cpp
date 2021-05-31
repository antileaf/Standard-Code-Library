// 注意FWT常数比较小, 这点与FFT/NTT不同
// 以下代码均以模质数情况为例, 其中n为变换长度, tp表示正/逆变换

// 按位或版本
void FWT_or(int *A, int n, int tp) {
	for (int k = 2; k <= n; k *= 2)
		for (int i = 0; i < n; i += k)
			for (int j = 0; j < k / 2; j++) {
				if (tp > 0)
					A[i + j + k / 2] = (A[i + j + k / 2] + A[i + j]) % p;
				else
					A[i + j + k / 2] = (A[i + j + k / 2] - A[i + j] + p)%p;
			}
}

// 按位与版本
void FWT_and(int *A, int n, int tp) {
	for (int k = 2; k <= n; k *= 2)
		for (int i = 0; i < n; i += k)
			for (int j = 0; j < k / 2; j++) {
				if (tp > 0)
					A[i + j] = (A[i + j] + A[i + j + k / 2]) % p;
				else
					A[i + j] = (A[i + j] - A[i + j + k / 2] + p) % p;
			}
}

// 按位异或版本
void FWT_xor(int *A, int n, int tp) {
	for (int k = 2; k <= n; k *= 2)
		for (int i = 0; i < n; i += k)
			for (int j = 0; j < k / 2; j++) {
				int a = A[i + j], b = A[i + j + k / 2];
				A[i + j] = (a + b) % p;
				A[i + j + k / 2] = (a - b + p) % p;
			}
	
	if (tp < 0) {
		int inv = qpow(n % p, p - 2); // n的逆元, 在不取模时需要用每层除以2代替
		for (int i = 0; i < n; i++)
			A[i] = A[i] * inv % p;
	}
}