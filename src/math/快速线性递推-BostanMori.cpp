// $a_n = \sum_{i = 1} ^ m f_i a_{n - i}\quad (f_0 = 0)$
// f.size() = a.size() + 1
int linear_recurrance(int n, poly f, poly a) {
	int m = (int)a.size();

	int ntt_n = 1;
	while (ntt_n <= m)
		ntt_n *= 2;
	
	ntt_init(ntt_n * 2);

	f.resize(ntt_n);
	a.resize(ntt_n);

	for (int i = 1; i <= m; i++)
		f[i] = (p - f[i]) % p;
	f[0] = 1;

	a = poly_mul(a, f);
	a.resize(ntt_n);
	fill(a.data() + m, a.data() + ntt_n, 0);

	return bostan_mori(n, a, f);
}
