int get_ntt_n(int n) { // not inclusive
	int ntt_n = 1;
	while (ntt_n < n)
		ntt_n *= 2;
	return ntt_n;
}

using poly = vector<int>;

// u, v 长度要相同，返回长度是两倍
poly poly_calc(const poly& u, const poly& v, function<int(int, int)> op) {
	static int a[MAXN], b[MAXN], c[MAXN];

	int n = (int)u.size();

	memcpy(a, u.data(), sizeof(int) * n);
	fill(a + n, a + n * 2, 0);
	memcpy(b, v.data(), sizeof(int) * n);
	fill(b + n, b + n * 2, 0);

	ntt(a, n * 2, 1);
	ntt(b, n * 2, 1);

	for (int i = 0; i < n * 2; i++)
		c[i] = op(a[i], b[i]);

	ntt(c, n * 2, -1);

	return poly(c, c + n * 2);
}

// 乘法，返回长度是两倍
poly poly_mul(const poly& u, const poly& v) {
	return poly_calc(u, v, [](int a, int b) { return (ll)a * b % p; });
}

// 求逆，返回长度不变
poly poly_inv(const poly& a) {
	poly c{qpow(a[0], p - 2)}; // 常数项一般都是 1

	for (int k = 2; k <= (int)a.size(); k *= 2) {
		c.resize(k);

		poly b(a.begin(), a.begin() + k);
		c = poly_calc(b, c, [](int bi, int ci) {
			return ((2 - (ll)bi * ci) % p + p) * ci % p;
		});
		memset(c.data() + k, 0, sizeof(int) * k);
	}

	c.resize(a.size());
	return c;
}

// 开根，返回长度不变
poly poly_sqrt(const poly& a) {
	poly c{1}; // 常数项不是 1 的话要写二次剩余

	for (int k = 2; k <= (int)a.size(); k *= 2) {
		c.resize(k);

		poly b(a.begin(), a.begin() + k);
		b = poly_mul(b, poly_inv(c));

		for (int i = 0; i < k; i++)
			c[i] = (ll)(c[i] + b[i]) * inv_2 % p; // inv_2 是 2 的逆元
	}

	c.resize(a.size());
	return c;
}

// 求导
poly poly_derivative(const poly& a) {
	poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++)
		c[i - 1] = (ll)a[i] * i % p;
	return c;
}

// 不定积分，最好预处理逆元
poly poly_integrate(const poly& a) {
	poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++)
		c[i] = (ll)a[i - 1] * inv[i] % p;
	return c;
}

// ln，常数项不能为 0，返回长度不变
poly poly_ln(const poly& a) {
	poly c = poly_mul(poly_derivative(a), poly_inv(a));
	c.resize(a.size());
	return poly_integrate(c);
}

// exp，常数项必须是 0，返回长度不变
// 常数很大并且总代码很长, 一般可以改用分治 FFT
// 依据: 设 $G(x) = \exp F(x)$, 则 $g_i = \frac{1}{i} \sum_{k=1}^{i-1} g_{i-k} \, k \, f_k$
poly poly_exp(const poly& a) {
	poly c{1};

	for (int k = 2; k <= (int)a.size(); k *= 2) {
		c.resize(k);
		
		poly b = poly_ln(c);
		for (int i = 0; i < k; i++) {
			b[i] = a[i] - b[i];
			if (b[i] < 0)
				b[i] += p;
		}
		(++b[0]) %= p;

		c = poly_mul(b, c);
		memset(c.data() + k, 0, sizeof(int) * k);
	}

	c.resize(a.size());
	return c;
}

// k 次幂，返回长度不变
// 注意常数项必须是 0，一次项必须是 1，否则需要在调用前处理一下
poly poly_pow(const poly& a, int k) {
	poly c = poly_ln(a);
	for (int i = 0; i < (int)c.size(); i++)
		c[i] = (ll)c[i] * k % p;
	return poly_exp(c);
}

// 自动判断长度的乘法
poly poly_auto_mul(poly a, poly b) {
	int res_len = (int)a.size() + (int)b.size() - 1;
	int ntt_n = get_ntt_n(res_len);
	
	a.resize(ntt_n);
	b.resize(ntt_n);

	ntt(a.data(), ntt_n, 1);
	ntt(b.data(), ntt_n, 1);

	for (int i = 0; i < ntt_n; i++)
		a[i] = (ll)a[i] * b[i] % p;
	
	ntt(a.data(), ntt_n, -1);
	a.resize(res_len);
	return a;
}

// 多项式除法，a 和 b 长度可以任意
// 商的长度是 n - m + 1，余数的长度是 m - 1
poly poly_div(const poly& a, const poly& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n < m)
		return {};
	
	int ntt_n = get_ntt_n(n - m + 1);
	
	poly f(ntt_n), g(ntt_n);
	for (int i = 0; i < n - m + 1; i++)
		f[i] = a[n - i - 1];
	for (int i = 0; i < m && i < n - m + 1; i++)
		g[i] = b[m - i - 1];
	
	poly g_inv = poly_inv(g);
	fill(g_inv.begin() + n - m + 1, g_inv.end(), 0);
	poly c = poly_mul(f, g_inv);
	c.resize(n - m + 1);
	reverse(c.begin(), c.end());
	return c;
}

// 多项式取模，a 和 b 长度可以任意，返回 (余数，商)
pair<poly, poly> poly_mod(const poly& a, const poly& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n < m)
		return {a, {}};
	
	poly d = poly_div(a, b);
	poly c = poly_auto_mul(b, d);

	poly r(m - 1);
	for (int i = 0; i < m - 1; i++)
		r[i] = (a[i] - c[i] + p) % p;
	return {r, d};
}

// 多项式多点求值，f 是多项式，x 是询问
struct poly_eval {
	poly f;
	vector<int> x;
	vector<poly> gs;
	vector<int> ans;

	poly_eval(const poly& f, const vector<int>& x) : f(f), x(x) {}

	void pretreat(int l, int r, int o) {
		poly& g = gs[o];

		if (l == r) {
			g = poly{p - x[l], 1};
			return;
		}

		int mid = (l + r) / 2;
		pretreat(l, mid, o * 2);
		pretreat(mid + 1, r, o * 2 + 1);

		if (o > 1)
			g = poly_auto_mul(gs[o * 2], gs[o * 2 + 1]);
	}

	void solve(int l, int r, int o, const poly& f) {
		if (l == r) {
			ans[l] = f[0];
			return;
		}

		int mid = (l + r) / 2;
		solve(l, mid, o * 2, poly_mod(f, gs[o * 2]).first);
		solve(mid + 1, r, o * 2 + 1, poly_mod(f, gs[o * 2 + 1]).first);
	}

	vector<int> operator() () {
		int n = (int)f.size(), m = (int)x.size();
		if (m <= n)
			x.resize(m = n + 1);
		else if (n < m - 1)
			f.resize(n = m - 1);
		
		int bit_ceil = get_ntt_n(m);
		ntt_init(bit_ceil * 2); // 注意这里初始化了

		gs.resize(2 * bit_ceil + 1);
		pretreat(0, m - 1, 1);

		ans.resize(m);
		solve(0, m - 1, 1, f);
		return ans;
	}
};
