// b.size() = n + 1
vector<poly> bostan_mori_comp(int n, const poly& a, vector<poly> b) {
	if (!n) { // a 的长度一定是最初的 n + 1
		int n0 = (int)a.size(), m = (int)b[0].size();

		b[0].insert(b[0].begin(), 1);
		b[0].resize(get_ntt_n(n0));
		poly b_inv = poly_inv(b[0]);
		b_inv.resize(a.size());

		poly t = poly_auto_mul(a, b_inv);
		poly res = poly(m);
		for (int i = 0; i < n0; i++)
			res[i + m - n0] = t[i];

		return {res};
	}

	int ntt_n = get_ntt_n(n * 2 + 1);
	int m = (int)b[0].size();
	
	vector<poly> q = b;
	dft_2d(b, ntt_n, m * 2);
	
	vector<poly> c(ntt_n);
	for (int i = 0; i < ntt_n; i++) // Q(-x, y) 的 DFT 直接从 Q(x, y) 的 DFT 转化过来就行了
		c[i] = b[(i + ntt_n / 2) % ntt_n];

	b.resize(ntt_n / 2);

	for (int i = 0; i < ntt_n / 2; i++)
		for (int j = 0; j < m * 2; j++)
			b[i][j] = (ll)b[i][j] * c[i][j] % p;
	
	idft_2d(b, n / 2 + 1, [] (int i) { return true; });

	for (int i = 0; i <= n / 2; i++) {
		for (int j = m * 2; j; j--)
			b[i][j] = b[i][j - 1];
		b[i][0] = 0;
		
		for (int j = 0; j < m; j++)
			b[i][j] = (b[i][j] + q[i * 2][j] * 2 % p) % p;
	}

	b.resize(n / 2 + 1);

	vector<poly> res = bostan_mori_comp(n / 2, a, move(b));

	vector<poly> t = res;
	dft_2d(t, ntt_n / 2, m * 2);

	for (int i = 0; i < ntt_n; i++) // 因为 V(x^2, y) 只有 x^2k 项，所以只需做一半
		for (int j = 0; j < m * 2; j++) // DFT 的后一半一定和前一半一样
			c[i][j] = (ll)t[i % (ntt_n / 2)][j] * c[i][j] % p;
	
	idft_2d(c, n + 1, [] (int i) { return true; });

	t = vector<poly>(n + 1);
	for (int i = 0; i <= n; i++) {
		t[i].resize(m);

		for (int j = 0; j < m; j++)
			t[i][j] = c[i][j + m - 1];

		if (i % 2 == 0)
			for (int j = 0; j < m; j++)
				t[i][j] = (t[i][j] + res[i / 2][j + m]) % p;
	}
	
	return t;
}

// 求 F(G(x)), 长度要相同
poly poly_composition(const poly& f, const poly& g) {
	int n = (int)f.size() - 1;

	poly a(n + 1);
	for (int i = 0; i <= n; i++)
		a[i] = f[n - i];

	vector<poly> b(n + 1);
	for (int i = 0; i <= n; i++)
		b[i] = {(p - g[i]) % p};
	
	vector<poly> t = bostan_mori_comp(n, a, b);
	
	poly res(n + 1);
	for (int i = 0; i <= n; i++)
		res[i] = t[i][0];
	return res;
}
