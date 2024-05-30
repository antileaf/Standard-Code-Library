poly poly_pow(const poly& a, int k) {
	poly c = poly_ln(a);
	for (int i = 0; i < (int)c.size(); i++)
		c[i] = (ll)c[i] * k % p;
	return poly_exp(c);
}

void dft(vector<poly>& a) {
	int n = (int)a.size(), m = (int)a[0].size();

	int ntt_n = 1;
	while (ntt_n < n * 2)
		ntt_n *= 2;
	
	a.resize(ntt_n);
	for (int i = 0; i < ntt_n; i++) {
		a[i].resize(m * 2);

		if (i < n)
			ntt(a[i].data(), m * 2, 1);
	}

	for (int j = 0; j < m * 2; j++) {
		poly t(ntt_n);

		for (int i = 0; i < n; i++)
			t[i] = a[i][j];
		
		ntt(t.data(), ntt_n, 1);

		for (int i = 0; i < ntt_n; i++)
			a[i][j] = t[i];
	}
}

void idft(vector<poly>& a, int n, int r) { // 只保留 % 2 = r 的行
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
		if (i % 2 == r)
			ntt(a[i].data(), m, -1);
}

// 对所有 $k \in [1, n]$，求 $[x^n] f^k(x)$，注意这里 n 是最高次数
poly bostan_mori(const poly& f) {
	int n = (int)f.size() - 1;
	vector<poly> a(n + 1), b(n + 1);

	for (int i = 0; i <= n; i++) {
		a[i].push_back(0);
		b[i].push_back((p - f[i]) % p);
	}
	
	bool a_constant = true;
	int m = 1;
	while (n) {
		vector<poly> c(b), ac(a), bc(b);
		for (int i = 0; i <= n; i++)
			for (int j = 0; j < m; j++) {
				if (i % 2)
					c[i][j] = (p - c[i][j]) % p;
				
				if (a_constant)
					(a[i][j] += c[i][j]) %= p;
				
				(b[i][j] += c[i][j]) %= p;
			}
		
		dft(ac);
		dft(bc);
		dft(c);

		for (int i = 0; i < (int)c.size(); i++)
			for (int j = 0; j < m * 2; j++) {
				ac[i][j] = (ll)ac[i][j] * c[i][j] % p;
				bc[i][j] = (ll)bc[i][j] * c[i][j] % p;
			}
		
		idft(ac, n + 1, n % 2);
		idft(bc, n + 1, 0);

		for (int i = 0; i <= n; i++) {
			if (i % 2 == n % 2) {
				a[i].resize(m * 2);

				for (int j = 1; j < m * 2; j++)
					(a[i][j] += ac[i][j - 1]) %= p;
			}

			if (i % 2 == 0) {
				b[i].resize(m * 2);

				for (int j = 1; j < m * 2; j++)
					(b[i][j] += bc[i][j - 1]) %= p;
			}
		}
		
		for (int i = 0; i <= n / 2; i++) {
			a[i].swap(a[i * 2 + n % 2]);
			b[i].swap(b[i * 2]);
		}

		a_constant &= !(n % 2);
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
	res.resize(f.size() + 1);
	return res;
}

// 返回长度和 g 相同
poly poly_compound_inversion(const poly& g) {
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

int main() { // 这里记得初始化要比平常再多开一倍
	int ntt_n = 1;
	while (ntt_n < n)
		ntt_n *= 2;
	ntt_init(ntt_n * 4);
}
