void Gauss_Jordan(int A[][maxn], int n) {
	for (int i = 1; i <= n; i++) {
		int ii = i;
		for (int j = i + 1; j <= n; j++)
			if (fabs(A[j][i]) > fabs(A[ii][i]))
				ii = j;

		if (ii != i) // 这里没有判是否无解，如果有可能无解的话要判一下
			for (int j = i; j <= n + 1; j++)
				swap(A[i][j], A[ii][j]);
		
		for (int j = 1; j <= n; j++)
			if (j != i) // 消成对角
				for (int k = n + 1; k >= i; k--)
					A[j][k] -= A[j][i] / A[i][i] * A[i][k];
	}
}
