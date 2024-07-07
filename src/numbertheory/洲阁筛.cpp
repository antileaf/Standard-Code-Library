constexpr int MAXN = 200005, p = 1000000007;

long long N, divs[MAXN]; // 询问的 n 和存储所有整除结果的表
int sqrtn;

inline int getid(long long x) {
	if (x <= sqrtn)
		return x;
	
	return divs[0] - N / x + 1;
}

bool notp[MAXN];
int prime[MAXN], prime_cnt, rem[MAXN]; // 线性筛用数组

int f[MAXN], pr[MAXN], g[2][MAXN], dp[MAXN];
int l[MAXN], r[MAXN];

// 线性筛省略

inline int get_sum(long long n, int k) {
	n %= p;

	if (k == 1)
		return n * (n + 1) % p * ((p + 1) / 2) % p;
	
	else
		return n * (n + 1) % p * (2 * n + 1) % p * ((p + 1) / 6) % p;
}

void get_dp(long long n, int k, int *dp) {
	for (int j = 1; j <= divs[0]; j++)
		dp[j] = get_sum(divs[j], k);
	
	for (int i = 1; i <= prime_cnt; i++) {
		long long lb = (long long)prime[i] * prime[i];
		int pw = (k == 1 ? prime[i] : (int)(lb % p));

		pr[i] = (pr[i - 1] + pw) % p;
		
		for (int j = divs[0]; j && divs[j] >= lb; j--) {
			int t = getid(divs[j] / prime[i]);

			int tmp = dp[t];
			if (l[t] < i)
				tmp = (tmp - pr[min(i - 1, r[t])] + pr[l[t]]) % p;

			dp[j] = (dp[j] - (long long)pw * tmp) % p;
			if (dp[j] < 0)
				dp[j] += p;
		}
	}

	for (int j = 1; j <= divs[0]; j++) {
		dp[j] = (dp[j] - pr[r[j]] + pr[l[j]]) % p;

		dp[j] = (dp[j] + p - 1) % p; // 因为DP数组是有1的, 但后面计算不应该有1
	}
}

int calc1(long long n) {
	get_dp(n, 1, g[0]);
	get_dp(n, 2, g[1]);

	int ans = 0;
	
	for (int i = 1; i <= sqrtn; i++)
		ans = (ans + (long long)f[i] * (g[1][getid(N / i)] - g[0][getid(N / i)])) % p;
	
	if (ans < 0)
		ans += p;

	return ans;
}

int calc2(long long n) {
	for (int j = 1; j <= divs[0]; j++)
		dp[j] = 1;
	
	for (int i = 1; i <= prime_cnt; i++)
		pr[i] = (pr[i - 1] + f[prime[i]]) % p;
	
	for (int i = prime_cnt; i; i--) {
		long long lb = (long long)prime[i] * prime[i];

		for (int j = divs[0]; j && divs[j] >= lb; j--)
			for (long long pc = prime[i]; pc <= divs[j]; pc *= prime[i]) {
				int t = getid(divs[j] / pc);

				int tmp = dp[t];
				if (r[t] > i)
					tmp = (tmp + pr[r[t]] - pr[max(i, l[t])]) % p;
				
				dp[j] = (dp[j] + pc % p * ((pc - 1) % p) % p * tmp) % p;
			}
	}

	return (long long)(dp[divs[0]] + pr[r[divs[0]]] - pr[l[divs[0]]] + p) % p;
}

int main() {
	// ios::sync_with_stdio(false);

	cin >> N;

	sqrtn = (int)sqrt(N);

	get_table(sqrtn);
	
	for (int i = 1; i <= sqrtn; i++)
		divs[++divs[0]] = i;
	
	for (int i = 1; i <= sqrtn; i++)
		divs[++divs[0]] = N / i;
	
	sort(divs + 1, divs + divs[0] + 1);
	
	divs[0] = unique(divs + 1, divs + divs[0] + 1) - divs - 1;

	int li = 0, ri = 0;
	for (int j = 1; j <= divs[0]; j++) {
		while (ri < prime_cnt && prime[ri + 1] <= divs[j])
			ri++;
		
		while (li <= prime_cnt && (long long)prime[li] * prime[li] <= divs[j])
			li++;
		
		l[j] = li - 1;
		r[j] = ri;
	}

	cout << (calc1(N) + calc2(N)) % p << endl;

	return 0;
}
