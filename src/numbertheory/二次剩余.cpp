int p, w;

struct pi {
	int a, b; // a + b * sqrt(w)

	pi(int a = 0, int b = 0) : a(a), b(b) {}

	friend pi operator * (const pi &u, const pi &v) {
		return pi(((long long)u.a * v.a + (long long)u.b * v.b % p * w) % p,
			((long long)u.a * v.b + (long long)u.b * v.a) % p);
	}
};

pi qpow(pi a, int b) {
	pi ans(1, 0);

	while (b) {
		if (b & 1)
			ans = ans * a;
		
		b >>= 1;
		a = a * a;
	}

	return ans;
}

int qpow(int a, int b) {
	int ans = 1;

	while (b) {
		if (b & 1)
			ans = (long long)ans * a % p;
		
		b >>= 1;
		a = (long long)a * a % p;
	}

	return ans;
}

int my_legendre(int a) { // std有同名函数, 最好换个名字, 不然传了两个数都查不出来
	return qpow(a, (p - 1) / 2);
}

int quadratic_residual(int b, int mod) {
	p = mod;

	if (p == 2)
		return 1;

	if (my_legendre(b) == p - 1)
		return -1; // 无解

	int a;
	do {
		a = rand() % p;
		w = ((long long)a * a - b) % p;
		if (w < 0)
			w += p;
	} while (my_legendre(w) != p - 1);

	return qpow(pi(a, 1), (p + 1) / 2).a;
}