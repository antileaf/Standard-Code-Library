void solve(int l,int r) {
	if (l == r)
		return;

	int mid = (l + r) / 2;
	
	solve(l, mid);

	int N = 1;
	while (N <= r - l + 1)
		N *= 2;

	for (int i = l; i <= mid; i++)
		B[i - l] = (long long)A[i] * fac_inv[i] % p;
	fill(B + mid - l + 1, B + N, 0);
	for (int i = 0; i < N; i++)
		C[i] = fac_inv[i];

	NTT(B, N, 1);
	NTT(C, N, 1);

	for (int i = 0; i < N; i++)
		B[i] = (long long)B[i] * C[i] % p;

	NTT(B, N, -1);

	for (int i = mid + 1; i <= r; i++)
		A[i] = (A[i] + B[i - l] * 2 % p * (long long)fac[i] % p) % p;

	solve(mid + 1, r);
}