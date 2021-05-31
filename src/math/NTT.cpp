constexpr int p = 998244353, g = 3; // p为模数, g为p的任意一个原根

void NTT(int *A, int n, int tp) { // n为变换长度, tp为1或-1, 表示正/逆变换
	for (int i = 1, j = 0, k; i < n - 1; i++) { // O(n)旋转算法, 原理是模拟加1
		k = n;
		do
			j ^= (k >>= 1);
		while (j < k);

		if(i < j)
			swap(A[i], A[j]);
	}

	for (int k = 2; k <= n; k <<= 1) {
		int wn = qpow(g, (tp > 0 ? (p - 1) / k : (p - 1) / k * (long long)(p - 2) % (p - 1)));
		for (int i = 0; i < n; i += k) {
			int w = 1;
			for (int j = 0; j < (k >> 1); j++, w = (long long)w * wn % p){
				int a = A[i + j], b = (long long)w * A[i + j + (k >> 1)] % p;
				A[i + j] = (a + b) % p;
				A[i + j + (k >> 1)] = (a - b + p) % p;
			} // 更好的写法是预处理单位根的次幂
		}
	}

	if (tp < 0) {
		int inv = qpow(n, p - 2); // 如果能预处理逆元更好
		for (int i = 0; i < n; i++)
			A[i] = (long long)A[i] * inv % p;
	}
}
