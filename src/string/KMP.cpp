void kmp(const char *s, int *fail, int n) { // 1-based
	int j = fail[0] = 0;

	for (int i = 1; i <= n; i++) {
		while (j && s[i] != s[j])
			j = fail[j - 1];
		
		fail[i] = (j += s[i] == s[j]);
	}
}