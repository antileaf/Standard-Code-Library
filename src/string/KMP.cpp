char s[maxn], t[maxn];
int fail[maxn];
int n, m;

void init() { // 注意字符串是0-based, 但是fail是1-based
	// memset(fail, 0, sizeof(fail));

	for (int i = 1; i < m; i++) {
		int j = fail[i];
		while (j && t[i] != t[j])
			j = fail[j];
		
		if (t[i] == t[j])
			fail[i + 1] = j + 1;
		else
			fail[i + 1] = 0;
	}
}

int KMP() {
	int cnt = 0, j = 0;
	
	for (int i = 0; i < n; i++) {
		while (j && s[i] != t[j])
			j = fail[j];
		
		if (s[i] == t[j])
			j++;
		if (j == m)
			cnt++;
	}

	return cnt;
}