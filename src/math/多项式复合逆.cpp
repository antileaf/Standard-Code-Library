// 对二元多项式做 DFT
void dft_2d(vector<poly>& a, int ntt_n, int ntt_m) {
	int n = (int)a.size();
	
	a.resize(ntt_n);
	for (int i = 0; i < ntt_n; i++) {
		a[i].resize(ntt_m);

		if (i < n)
			ntt(a[i].data(), ntt_m, 1);
	}

	for (int j = 0; j < ntt_m; j++) {
		poly t(ntt_n);

		for (int i = 0; i < n; i++)
			t[i] = a[i][j];
		
		ntt(t.data(), ntt_n, 1);

		for (int i = 0; i < ntt_n; i++)
			a[i][j] = t[i];
	}
}

// 对二元多项式做 DFT，只保留前 n 行中 filter = true 的行
void idft_2d(vector<poly>& a, int n, function<bool(int)> filter) {
	int ntt_n = (int)a.size(), m = (int)a[0].size();

	for (int j = 0; j < m; j++) {
		poly t(ntt_n);
		for (int i = 0; i < ntt_n; i++)
			t[i] = a[i][j];
		
		ntt(t.data(), ntt_n, -1);

		for (int i = 0; i < n; i++)
			a[i][j] = t[i];
	}

	a.resize(n);
	for (int i = 0; i < n; i++)
		if (filter(i))
			ntt(a[i].data(), m, -1);
}

// 对所有 $k \in [1, n]$，求 $[x^n] f^k(x)$
// 注意这里 n 是最高次数，即 f 的长度为 n+1
poly bostan_mori(const poly& f) {
	int n = (int)f.size() - 1;
	vector<poly> a(n + 1), b(n + 1);

	for (int i = 0; i <= n; i++) {
		a[i] = {0};
		b[i] = {(p - f[i]) % p};
	}
	
	bool a00 = true;
	int m = 1;
	while (n) {
		vector<poly> ac(a), bc(b);
		for (int i = 0; i <= n; i++)
			for (int j = 0; j < m; j++) {
				if (a00) {
					int d = i % 2 ? p - b[i][j] : b[i][j];
					(a[i][j] += d) %= p;
				}
				
				if (i % 2 == 0)
					b[i][j] = b[i][j] * 2 % p;
				else
					b[i][j] = 0;
			}
		
		int ntt_n = get_ntt_n(n * 2 + 1);
		
		dft_2d(ac, ntt_n, m * 2);
		dft_2d(bc, ntt_n, m * 2);

		for (int i = 0; i < ntt_n; i++)
			for (int j = 0; j < m * 2; j++) // Q(-x, y) 的 DFT 直接从 Q(x, y) 的 DFT 转化过来就行了
				ac[i][j] = (ll)ac[i][j] * bc[(i + ntt_n / 2) & (ntt_n - 1)][j] % p;
		
		for (int i = 0; i < ntt_n / 2; i++) // 因为 Q(x, y) Q(-x, y) 只有 x^2k 项，所以只需做一半
			for (int j = 0; j < m * 2; j++) // DFT 的后一半一定和前一半一样
				bc[i][j] = (ll)bc[i][j] * bc[i + ntt_n / 2][j] % p;
		
		bc.resize(ntt_n / 2);
		
		idft_2d(ac, n + 1, [n](int i) { return i % 2 == n % 2; });
		idft_2d(bc, n / 2 + 1, [](int i) { return true; });

		for (int i = 0; i <= n; i++)
			if (i % 2 == n % 2) {
				a[i].resize(m * 2);

				for (int j = 1; j < m * 2; j++)
					(a[i][j] += ac[i][j - 1]) %= p;
				
				a[i / 2].swap(a[i]);
			}

		for (int i = 0; i <= n / 2; i++) {
			b[i].swap(b[i * 2]);
			b[i].resize(m * 2);

			for (int j = 1; j < m * 2; j++)
				(b[i][j] += bc[i][j - 1]) %= p;
		}

		a00 &= !(n % 2);
		n /= 2;
		m *= 2;

		a.resize(n + 1);
		b.resize(n + 1);
	}
	
	for (int i = (int)f.size() - 1; i; i--) {
		a[0][i] = a[0][i - 1];
		b[0][i] = b[0][i - 1];
	}
	a[0][0] = b[0][0] = 1;
	
	poly res = poly_mul(a[0], poly_inv(b[0])); // 因为 m 每次都翻倍，所以此时 FFT 的长度一定就是 m
	res.resize(f.size());
	return res;
}

// 返回长度和 g 相同
// 因为做 DFT 的时候没有二维转一维，所以还是只需要初始化 get_ntt_n(n + 1) * 2 就行了
poly poly_composite_inversion(const poly& g) {
	assert(!g[0] && g[1]);
	
	int n = (int)g.size() - 1;
	
	int g1_inv = qpow(g[1], p - 2);
	poly t(n + 1);
	for (int i = 1; i <= n; i++)
		t[i] = (ll)g[i] * g1_inv % p;
	
	poly res = bostan_mori(t);
	
	for (int i = 0, pw = 1; i < n; i++) {
		t[i] = (ll)n * inv[n - i] % p * res[n - i] % p * pw % p;
		pw = (ll)pw * g1_inv % p;
	}

	int ntt_n = 1;
	while (ntt_n <= n)
		ntt_n *= 2;
	t.resize(ntt_n);

	t = poly_pow(t, p - inv[n]);

	poly f(n + 1);
	for (int i = 1; i <= n; i++)
		f[i] = (ll)t[i - 1] * g1_inv % p;
	return f;
}
