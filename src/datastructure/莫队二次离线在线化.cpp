constexpr int maxn = 100005, B = 315, maxb = maxn / B + 5;

int n, bid[maxn], L[maxb], R[maxb], cntb;

struct DS { // O(sqrt(n))修改  O(1)查询
	int total;
	int sa[maxn], sb[maxb];

	void init(const DS &o) {
		total = o.total;
		memcpy(sa, o.sa, sizeof(int) * (n + 1));
		memcpy(sb, o.sb, sizeof(int) * (cntb + 1));
	}

	void add(int x) {
		total++;
		for (int k = 1; k <= bid[x]; k++)
			sb[k]++;
		for (int i = L[bid[x]]; i <= x; i++)
			sa[i]++;
	}
	
	int querys(int x) {
		if (x > n)
			return 0;

		return sb[bid[x] + 1] + sa[x];
	}

	int queryp(int x) {
		return total - querys(x + 1);
	}
} pr[maxb];

int c[maxn]; // 树状数组

void addc(int x, int d) {
	while (x) {
		c[x] += d;
		x -= x & -x;
	}
}

int queryc(int x) {
	int ans = 0;
	while (x <= n) {
		ans += c[x];
		x += x & -x;
	}
	return ans;
}

long long fp[maxn], fs[maxn];

int rnk[maxn], val[maxn][B + 5];

long long dat[maxb][maxb];

int a[maxn];

int main() {

	int m;
	cin >> n >> m;

	for (int i = 1; i <= n; i++) {
		cin >> a[i];

		bid[i] = (i - 1) / B + 1;
		if (!L[bid[i]])
			L[bid[i]] = i;
		R[bid[i]] = i;
		cntb = bid[i];

		rnk[i] = i;
	}

	for (int k = 1; k <= cntb; k++)
		sort(rnk + L[k], rnk + R[k] + 1, [](int x, int y) {return a[x] < a[y];}); // 每个块排序
	
	for (int i = n; i; i--)
		for (int j = 2; i + j - 1 <= R[bid[i]]; j++) {
			val[i][j] = val[i + 1][j - 1] + val[i][j - 1] - val[i + 1][j - 2];
			if (a[i] > a[i + j - 1])
				val[i][j]++; // 块内用二维前缀和预处理
		}

	for (int k = 1; k <= cntb; k++) {
		for (int i = L[k]; i <= R[k]; i++) {
			dat[k][k] += queryc(a[i] + 1); // 单块内的逆序对直接用树状数组预处理
			addc(a[i], 1);
		}

		for (int i = L[k]; i <= R[k]; i++)
			addc(a[i], -1);
	}

	for (int i = 1; i <= n; i++) {
		if (i > 1 && i == L[bid[i]])
			pr[bid[i]].init(pr[bid[i] - 1]);
		
		fp[i] = fp[i - 1] + pr[bid[i]].querys(a[i] + 1);
		
		pr[bid[i]].add(a[i]);
	}

	for (int i = n; i; i--) {
		fs[i] = fs[i + 1] + (n - i - queryc(a[i] + 1));
		addc(a[i], 1);
	}

	for (int s = 1; s <= cntb; s++)
		for (int t = s + 1; t <= cntb; t++) {
			dat[s][t] = dat[s][t - 1] + dat[t][t];

			for (int i = L[t]; i <= R[t]; i++) // 块间的逆序对用刚才处理的分块求出
				dat[s][t] += pr[t - 1].querys(a[i] + 1) - pr[s - 1].querys(a[i] + 1);
		}
	
	long long ans = 0;

	while (m--) {
		long long s, t;
		cin >> s >> t;

		int l = s ^ ans, r = t ^ ans;

		if (bid[l] == bid[r])
			ans = val[l][r - l + 1];
		else {
			ans = dat[bid[l] + 1][bid[r] - 1];

			ans += fp[r] - fp[L[bid[r]] - 1];
			for (int i = L[bid[r]]; i <= r; i++)
				ans -= pr[bid[l]].querys(a[i] + 1);
			
			ans += fs[l] - fs[R[bid[l]] + 1];
			for (int i = l; i <= R[bid[l]]; i++)
				ans -= (a[i] - 1) - pr[bid[r] - 1].queryp(a[i] - 1);
			
			int i = L[bid[l]], j = L[bid[r]], w = 0; // 手写归并

			while (true) {
				while (i <= R[bid[l]] && rnk[i] < l)
					i++;
				while (j <= R[bid[r]] && rnk[j] > r)
					j++;
				
				if (i > R[bid[l]] && j > R[bid[r]])
					break;
				
				int x = (i <= R[bid[l]] ? a[rnk[i]] : (int)1e9), y = (j <= R[bid[r]] ? a[rnk[j]] : (int)1e9);

				if (x < y) {
					ans += w;
					i++;
				}
				else {
					j++;
					w++;
				}
			}
		}

		cout << ans << '\n';
	}

	return 0;
}