poly poly_pow(const poly& a, int k) {
	poly c = poly_ln(a);
	for (int i = 0; i < (int)c.size(); i++)
		c[i] = (ll)c[i] * k % p;
	return poly_exp(c);
}

void dft(vector<poly>& a) {
	int n = (int)a.size(), m = (int)a[0].size();

	int ntt_n = 1, ntt_m = 1;
	while (ntt_n < n * 2)
		ntt_n *= 2;
	while (ntt_m < m * 2)
		ntt_m *= 2;
	
	a.resize(ntt_n);
	for (int i = 0; i < ntt_n; i++) {
		a[i].resize(ntt_m);

		if (i < n)
			ntt(a[i].data(), ntt_m, 1);
	}

	for (int j = 0; j < ntt_m; j++) {
		poly t(ntt_n);

		for (int i = 0; i < ntt_n; i++)
			t[i] = a[i][j];
		
		ntt(t.data(), ntt_n, 1);

		for (int i = 0; i < ntt_n; i++)
			a[i][j] = t[i];
	}
}

void idft(vector<poly>& a, int n) {
	int ntt_n = (int)a.size(), ntt_m = (int)a[0].size();

	for (int j = 0; j < ntt_m; j++) {
		poly t(ntt_n);

		for (int i = 0; i < ntt_n; i++)
			t[i] = a[i][j];
		
		ntt(t.data(), ntt_n, -1);

		for (int i = 0; i < n; i++)
			a[i][j] = t[i];
	}

	for (int i = 0; i < n; i++)
		ntt(a[i].data(), ntt_m, -1);
}

// 对所有 $k \in [1, n]$，求 $[x^n] f^k(x)$，注意这里 n 是最高次数
poly bostan_mori(const poly& f) {
	int n = (int)f.size() - 1;
	vector<poly> a(n + 1), b(n + 1);
	for (int i = 0; i <= n; i++) {
		a[i].resize(2);
		b[i].resize(2);
	}

	a[0][0] = b[0][0] = 1;
	for (int i = 0; i <= n; i++)
		b[i][1] = (p - f[i]) % p;
	
	int m = 1;
	while (n) {
		vector<poly> c(b);
		for (int i = 1; i <= n; i += 2)
			for (int j = 0; j <= m; j++)
				c[i][j] = (p - c[i][j]) % p;
		
		dft(a);
		dft(b);
		dft(c);

		for (int i = 0; i < (int)a.size(); i++)
			for (int j = 0; j < (int)a[0].size(); j++) {
				a[i][j] = (ll)a[i][j] * c[i][j] % p;
				b[i][j] = (ll)b[i][j] * c[i][j] % p;
			}
		
		idft(a, n * 2 + 1);
		idft(b, n * 2 + 1);

		for (int i = 0; i <= n / 2; i++) {
			a[i].swap(a[i * 2 + n % 2]);
			a[i].resize(m * 2 + 1);
			b[i].swap(b[i * 2]);
			b[i].resize(m * 2 + 1);
		}

		n /= 2;
		m *= 2;

		a.resize(n + 1);
		b.resize(n + 1);
	}

	int ntt_n = 1;
	while (ntt_n <= (int)f.size())
		ntt_n *= 2;
	
	b[0].resize(ntt_n);
	poly res = poly_auto_mul(a[0], poly_inv(b[0]));
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
