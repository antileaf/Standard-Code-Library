int bostan_mori(int k, poly a, poly b) {
	int n = (int)a.size();

	while (k) {
		poly c = b;
		for (int i = 1; i < n; i += 2)
			c[i] = (p - c[i]) % p;
		
		a = poly_mul(a, c);
		b = poly_mul(b, c);

		for (int i = 0; i < n; i++) {
			a[i] = a[i * 2 + k % 2];
			b[i] = b[i * 2];
		}

		a.resize(n);
		b.resize(n);
		k /= 2;
	}

	return (ll)a[0] * qpow(b[0], p - 2) % p;
}
