for (int i = 0; i <= k; i++)
	r[i] = f(i);
for (int j = 0; j < k; j++)
	for (int i = k; i > j; i--)
		r[i] -= r[i - 1];
