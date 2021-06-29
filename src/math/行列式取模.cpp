int p;

int Gauss(int A[maxn][maxn], int n) {
	int det = 1;

	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++)
			while (A[j][i]) {
				int t = (p - A[i][i] / A[j][i]) % p;
				for (int k = i; k <= n; k++)
					A[i][k] = (A[i][k] + (long long)A[j][k] * t) % p;
				
				swap(A[i], A[j]);
				det = (p - det) % p; // 交换一次之后行列式取负
			}

			if (!A[i][i])
				return 0;

			det = (long long)det * A[i][i] % p;
	}

	return det;
}