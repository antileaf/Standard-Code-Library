//Extended KMP 扩展KMP
//By AntiLeaf
//通过题目：小作业OJ 4182

//全局变量与数组定义
char s[maxn], t[maxn];
int n, m, a[maxn];

//主过程 O(n + m)
//把t的每个后缀与s的LCP输出到a中，s的后缀和自己的LCP存在nx中
//0-based，s的长度是m，t的长度是n
void exKMP(const char *s, const char *t, int *a) {
	static int nx[maxn];

	memset(nx, 0, sizeof(nx));

	int j = 0;
	while (j + 1 < m && s[j] == s[j + 1])
		j++;
	nx[1] = j;

	for (int i = 2, k = 1;i < m; i++) {
		int pos = k + nx[k], len = nx[i - k];

		if (i + len < pos)
			nx[i] = len;
		else {
			j = max(pos - i, 0);
			while (i + j < m && s[j] == s[i + j])
				j++;

			nx[i] = j;
			k = i;
		}
	}

	j = 0;
	while (j < n && j < m && s[j] == t[j])
		j++;
	a[0] = j;

	for (int i = 1, k = 0; i < n; i++) {
		int pos = k + a[k], len = nx[i - k];
		if (i + len < pos)
			a[i] = len;
		else {
			j = max(pos - i, 0);
			while(j < m && i + j < n && s[j] == t[i + j])
				j++;
				
			a[i] = j;
			k = i;
		}
	}
}