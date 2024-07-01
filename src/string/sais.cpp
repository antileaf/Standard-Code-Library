constexpr int l_type = 0, s_type = 1;

// 判断一个字符是否为LMS字符
bool is_lms(vector<int>& tp, int x) {
	return x > 0 && tp[x] == s_type && tp[x - 1] == l_type;
}

// 判断两个LMS子串是否相同
bool equal_substr(int* s, int x, int y, vector<int>& tp) {
	do {
		if (s[x] != s[y])
			return false;
		x++;
		y++;
	} while (!is_lms(tp, x) && !is_lms(tp, y));

	return s[x] == s[y];
}

// s 是输入字符串，len 是字符串的长度，m 是字符集的大小
vector<int> sais(int *s, int len, int m) {
	int n = len - 1;

	vector<int> tp(n + 1), pos(n + 1), name(n + 1, -1), sa(n + 1, -1);
	vector<int> buc(m + 1), lbuc(m + 1), sbuc(m + 1);

	// 诱导排序（从 * 型诱导到 L 型，从 L 型诱导到 S 型）
	// 调用之前应将 * 型按要求放入 SA 中
	auto induced_sort = [&]() {
		for (int i = 0; i <= n; i++)
			if (sa[i] > 0 && tp[sa[i] - 1] == l_type)
				sa[lbuc[s[sa[i] - 1]]++] = sa[i] - 1;
			
		for (int i = 1; i <= m; i++)
			sbuc[i] = buc[i] - 1;
		
		for (int i = n; ~i; i--)
			if (sa[i] > 0 && tp[sa[i] - 1] == s_type)
				sa[sbuc[s[sa[i] - 1]]--] = sa[i] - 1;
	};

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
	
	for (int i = 0; i < cnt; i++)
		sa[sbuc[s[pos[i]]]--] = pos[i];
	induced_sort();

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

	int* t = new int[cnt];
	int p = 0;
	for (int i = 0; i <= n; i++)
		if (name[i] >= 0)
			t[p++] = name[i];

	vector<int> tsa;
	if (!flag) {
		tsa.resize(cnt);

		for (int i = 0; i < cnt; i++)
			tsa[t[i]] = i;
	}
	else
		tsa = move(sais(t, cnt, namecnt));
	
	lbuc[0] = sbuc[0] = 0;
	for (int i = 1; i <= m; i++) {
		lbuc[i] = buc[i - 1];
		sbuc[i] = buc[i] - 1;
	}

	sa.assign(n + 1, -1);
	for (int i = cnt - 1; ~i; i--)
		sa[sbuc[s[pos[tsa[i]]]]--] = pos[tsa[i]];
	induced_sort();
	
	return sa;
}

// 封装好的函数, 1-based
void get_sa(char *s, int n, int *sa, int *rnk, int *height) {
	static int a[MAXN];

	a[n] = '$';
	for (int i = 1; i <= n; i++)
		a[i - 1] = s[i];

	vector<int> t = sais(a, n + 1, 256);
	copy(t.begin(), t.end(), sa);

	sa[0] = 0;
	for (int i = 1; i <= n; i++)
		rnk[++sa[i]] = i;
		
	for (int i = 1, k = 0; i <= n; i++) {
		if (k)
			k--;

		while (s[i + k] == s[sa[rnk[i] - 1] + k])
			k++;

		height[rnk[i]] = k; // height[i] = lcp(sa[i], sa[i - 1])
	}
}
