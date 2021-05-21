void solve(int l, int r) {
	if (r <= m)
		return;

	if (r - l == 1) {
		if (l == m)
			f[l] = a[m];
		else
			f[l] = (long long)f[l] * inv[l - m] % p;
		
		for (int i = l, t = (long long)l * f[l] % p; i <= n; i += l)
			g[i] = (g[i] + t) % p;

		return;
	}

	int mid = (l + r) / 2;

	solve(l, mid);

	if (l == 0) {
		for (int i = 1; i < mid; i++) {
			A[i] = f[i];
			B[i] = (c[i] + g[i]) % p;
		}
		NTT(A, r, 1);
		NTT(B, r, 1);
		for (int i = 0; i < r; i++)
			A[i] = (long long)A[i] * B[i] % p;
		NTT(A, r, -1);

		for (int i = mid; i < r; i++)
			f[i] = (f[i] + A[i]) % p;
	}
	else {
		for (int i = 0; i < r - l; i++)
			A[i] = f[i];
		for (int i = l; i < mid; i++)
			B[i - l] = (c[i] + g[i]) % p;
		NTT(A, r - l, 1);
		NTT(B, r - l, 1);
		for (int i = 0; i < r - l; i++)
			A[i] = (long long)A[i] * B[i] %p;
		NTT(A, r - l, -1);

		for (int i = mid; i < r; i++)
			f[i] = (f[i] + A[i - l]) % p;
		
		memset(A, 0, sizeof(int) * (r - l));
		memset(B, 0, sizeof(int) * (r - l));

		for (int i = l; i < mid; i++)
			A[i - l] = f[i];
		for (int i = 0; i < r - l; i++)
			B[i] = (c[i] + g[i]) % p;
		NTT(A, r - l, 1);
		NTT(B, r - l, 1);
		for (int i = 0; i < r - l; i++)
			A[i] = (long long)A[i] * B[i] % p;
		NTT(A, r - l, -1);
		
		for (int i = mid; i < r; i++)
			f[i] = (f[i] + A[i - l]) % p;
	}

	memset(A, 0, sizeof(int) * (r - l));
	memset(B, 0, sizeof(int) * (r - l));

	solve(mid, r);
}
