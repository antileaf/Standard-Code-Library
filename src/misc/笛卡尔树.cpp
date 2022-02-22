int s[maxn], root, lc[maxn], rc[maxn];

int top = 0;
s[++top] = root = 1;
for (int i = 2; i <= n; i++) {
	s[top + 1] = 0;
	while (top && a[i] < a[s[top]]) // 小根笛卡尔树
		top--;

	if (top)
		rc[s[top]] = i;
	else
		root = i;
	
	lc[i] = s[top + 1];
	s[++top] = i;
}