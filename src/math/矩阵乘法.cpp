for (int i = 1; i <= n; i++)
	for (int k = 1; k <= n; k++)
		for (int j = 1; j <= n; j++)
			a[i][j] += b[i][k] * c[k][j];
// 通过改善内存访问连续性，显著提升速度
