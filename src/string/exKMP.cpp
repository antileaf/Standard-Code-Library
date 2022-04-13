void exkmp(const char *s, int *a, int n) {
	int l = 0, r = 0;
	a[0] = n;

	for (int i = 1; i <= n; i++) {
		a[i] = i > r ? 0 : min(r - i + 1, a[i - l]);

		while (i + a[i] < n && s[a[i]] == s[i + a[i]])
			a[i]++;
		
		if (i + a[i] - 1 > r) {
			l = i;
			r = i + a[i] - 1;
		}
	}
}