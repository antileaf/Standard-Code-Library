constexpr int p = 998244353; // p为模数

int ntt_n, omega[maxn], omega_inv[maxn]; // ntt_n要在主函数里初始化

void NTT_init(int n) {
	int wn = qpow(3, (p - 1) / n); // 这里的3代表模数的任意一个原根

	omega[0] = omega_inv[0] = 1;

	for (int i = 1; i < n; i++)
		omega_inv[n - i] = omega[i] = (long long)omega[i - 1] * wn % p;
}

void NTT(int *a, int n, int tp) { // n为变换长度, tp为1或-1, 表示正/逆变换

	for (int i = 1, j = 0, k; i < n - 1; i++) { // O(n)旋转算法, 原理是模拟加1
		k = n;
		do
			j ^= (k >>= 1);
		while (j < k);

		if (i < j)
			swap(a[i], a[j]);
	}

	for (int k = 2, m = ntt_n / 2; k <= n; k *= 2, m /= 2)
		for (int i = 0; i < n; i += k)
			for (int j = 0; j < k / 2; j++) {
				int w = (tp > 0 ? omega : omega_inv)[m * j];

				int u = a[i + j], v = (long long)w * a[i + j + k / 2] % p;
				a[i + j] = u + v;
				if (a[i + j] >= p)
					a[i + j] -= p;

				a[i + j + k / 2] = u - v;
				if (a[i + j + k / 2] < 0)
					a[i + j + k / 2] += p;
			}

	if (tp < 0) {
		int inv = qpow(n, p - 2);
		for (int i = 0; i < n; i++)
			a[i] = (long long)a[i] * inv % p;
	}
}