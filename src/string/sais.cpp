// 注意求完的SA有效位只有1~n, 但它是0-based, 如果其他部分是1-based记得+1再用

constexpr int maxn = 100005, l_type = 0, s_type = 1;

// 判断一个字符是否为LMS字符
bool is_lms(int *tp, int x) {
	return x > 0 && tp[x] == s_type && tp[x - 1] == l_type;
}

// 判断两个LMS子串是否相同
bool equal_substr(int *s, int x, int y, int *tp) {
	do {
		if (s[x] != s[y])
			return false;
		x++;
		y++;
	} while (!is_lms(tp, x) && !is_lms(tp, y));

	return s[x] == s[y];
}

// 诱导排序(从*型诱导到L型,从L型诱导到S型)
// 调用之前应将*型按要求放入SA中
void induced_sort(int *s, int *sa, int *tp, int *buc, int *lbuc, int *sbuc, int n, int m) {
	for (int i = 0; i <= n; i++)
		if (sa[i] > 0 && tp[sa[i] - 1] == l_type)
			sa[lbuc[s[sa[i] - 1]]++] = sa[i] - 1;
		
	for (int i = 1; i <= m; i++)
		sbuc[i] = buc[i] - 1;
	
	for (int i = n; ~i; i--)
		if (sa[i] > 0 && tp[sa[i] - 1] == s_type)
			sa[sbuc[s[sa[i] - 1]]--] = sa[i] - 1;
}

// s是输入字符串, n是字符串的长度, m是字符集的大小
int *sais(int *s, int len, int m) {
	int n = len - 1;

	int *tp = new int[n + 1];
	int *pos = new int[n + 1];
	int *name = new int[n + 1];
	int *sa = new int[n + 1];
	int *buc = new int[m + 1];
	int *lbuc = new int[m + 1];
	int *sbuc = new int[m + 1];

	memset(buc, 0, sizeof(int) * (m + 1));
	memset(lbuc, 0, sizeof(int) * (m + 1));
	memset(sbuc, 0, sizeof(int) * (m + 1));

	for (int i = 0; i <= n; i++)
		buc[s[i]]++;
	
	for (int i = 1; i <= m; i++) {
		buc[i] += buc[i - 1];

		lbuc[i] = buc[i - 1];
		sbuc[i] = buc[i] - 1;
	}

	tp[n] = s_type;
	for (int i = n - 1; ~i; i--) {
		if (s[i] < s[i + 1])
			tp[i] = s_type;
		else if (s[i] > s[i + 1])
			tp[i] = l_type;
		else
			tp[i] = tp[i + 1];
	}

	int cnt = 0;
	for (int i = 1; i <= n; i++)
		if (tp[i] == s_type && tp[i - 1] == l_type)
			pos[cnt++] = i;
	
	memset(sa, -1, sizeof(int) * (n + 1));
	for (int i = 0; i < cnt; i++)
		sa[sbuc[s[pos[i]]]--] = pos[i];
	induced_sort(s, sa, tp, buc, lbuc, sbuc, n, m);

	memset(name, -1, sizeof(int) * (n + 1));
	int lastx = -1, namecnt = 1;
	bool flag = false;
	
	for (int i = 1; i <= n; i++) {
		int x = sa[i];

		if (is_lms(tp, x)) {
			if (lastx >= 0 && !equal_substr(s, x, lastx, tp))
				namecnt++;
			
			if (lastx >= 0 && namecnt == name[lastx])
				flag = true;

			name[x] = namecnt;
			lastx = x;
		}
	}
	name[n] = 0;

	int *t = new int[cnt];
	int p = 0;
	for (int i = 0; i <= n; i++)
		if (name[i] >= 0)
			t[p++] = name[i];

	int *tsa;
	if (!flag) {
		tsa = new int[cnt];

		for (int i = 0; i < cnt; i++)
			tsa[t[i]] = i;
	}
	else
		tsa = sais(t, cnt, namecnt);
	
	lbuc[0] = sbuc[0] = 0;
	for (int i = 1; i <= m; i++) {
		lbuc[i] = buc[i - 1];
		sbuc[i] = buc[i] - 1;
	}

	memset(sa, -1, sizeof(int) * (n + 1));
	for (int i = cnt - 1; ~i; i--)
		sa[sbuc[s[pos[tsa[i]]]]--] = pos[tsa[i]];
	induced_sort(s, sa, tp, buc, lbuc, sbuc, n, m);
	
	return sa;
}

// O(n)求height数组, 注意是sa[i]与sa[i - 1]的LCP
void get_height(int *s, int *sa, int *rnk, int *height, int n) {
	for (int i = 0; i <= n; i++)
		rnk[sa[i]] = i;

	int k = 0;
	for (int i = 0; i <= n; i++) {
		if (!rnk[i])
			continue;
		
		if (k)
			k--;
		
		while (s[sa[rnk[i]] + k] == s[sa[rnk[i] - 1] + k])
			k++;
		
		height[rnk[i]] = k;
	}
}

char str[maxn];
int n, s[maxn], sa[maxn], rnk[maxn], height[maxn];

// 方便起见附上主函数
int main() {
	scanf("%s", str);
	n = strlen(str);
	str[n] = '$';

	for (int i = 0; i <= n; i++)
		s[i] = str[i];
	
	memcpy(sa, sais(s, n + 1, 256), sizeof(int) * (n + 1));

	get_height(s, sa, rnk, height, n);

	return 0;
}