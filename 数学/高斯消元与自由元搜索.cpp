
void gauss() {
	for (int i = 63; ~i; i--) {
		if (!(a[i] >> i & 1)) {
			for (int j = i - 1; ~j; j--)
				if (a[j] >> i & 1) {
					swap(a[i], a[j]);
					swap(b[i], b[j]);
					break;
				}
		}

		if (!(a[i] >> i & 1))
			continue;
		
		for (int j = 63; ~j; j--)
			if (j != i && (a[j] >> i & 1)) {
				a[j] ^= a[i];
				b[j] ^= b[i];
			}
	}

	/*
	printf("after gauss:\n");
	for (int i = 63; ~i; i--) {
		for (int j = 63; ~j; j--)
			printf("%llu", a[i] >> j & 1);
		printf(" %llu\n", b[i]);
	}
	*/
	
}

bool check() {
	unsigned long long x = 0;
	for (int i = 0; i < 64; i++)
		if (ans[i])
			x |= 1ull << i;
	
	for (int i = 1; i <= n; i++) {
		x ^= x << 13;
		x ^= x >> 7;
		x ^= x << 17;

		if (x % i != val[i])
			return false;
	}
	return true;
}

bool dfs(int i) {
	if (i == 64)
		return check();
	
	if (a[i] >> i & 1) {
		ans[i] = b[i];
		for (int j = 0; j < i; j++)
			if (a[i] >> j & 1)
				ans[i] ^= ans[j];

		return dfs(i + 1);
	}

	ans[i] = 0;
	if (dfs(i + 1))
		return true;
	
	ans[i] = 1;
	return dfs(i + 1);

}