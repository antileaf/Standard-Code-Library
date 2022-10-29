int fail[maxn];

void kmp(const char *s, int n) {
	fail[0] = fail[1] = 0;

	for (int i = 1; i < n; i++) {
		int j = fail[i];

		while (j && s[i + 1] != s[j + 1])
			j = fail[j];
		
		if (s[i + 1] == s[j + 1])
			fail[i + 1] = j + 1;
		else
			fail[i + 1] = 0;
	}
}