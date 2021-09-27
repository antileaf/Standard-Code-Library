constexpr int maxn = 200005, p = 1000000007;

long long N, val[maxn]; // 询问的n和存储所有整除结果的表
int sqrtn;

inline int getid(long long x) {
	if (x <= sqrtn)
		return x;
	
	return val[0] - N / x + 1;
}

bool notp[maxn];
int prime[maxn], prime_cnt, rem[maxn]; // 线性筛用数组

int f[maxn], pr[maxn], g[2][maxn], dp[maxn];
int l[maxn], r[maxn];

// 线性筛省略

inline int get_sum(long long n, int k) {
	n %= p;

	if (k == 1)
		return n * (n + 1) % p * ((p + 1) / 2) % p;
	
	else
		return n * (n + 1) % p * (2 * n + 1) % p * ((p + 1) / 6) % p;
}

void get_dp(long long n, int k, int *dp) {
	for (int j = 1; j <= val[0]; j++)
		dp[j] = get_sum(val[j], k);
	
	for (int i = 1; i <= prime_cnt; i++) {
		long long lb = (long long)prime[i] * prime[i];
		int pw = (k == 1 ? prime[i] : (int)(lb % p));

		pr[i] = (pr[i - 1] + pw) % p;
		
		for (int j = val[0]; j && val[j] >= lb; j--) {
			int t = getid(val[j] / prime[i]);

			int tmp = dp[t];
			if (l[t] < i)
				tmp = (tmp - pr[min(i - 1, r[t])] + pr[l[t]]) % p;

			dp[j] = (dp[j] - (long long)pw * tmp) % p;
			if (dp[j] < 0)
				dp[j] += p;
		}
	}

	for (int j = 1; j <= val[0]; j++) {
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
	for (int j = 1; j <= val[0]; j++)
		dp[j] = 1;
	
	for (int i = 1; i <= prime_cnt; i++)
		pr[i] = (pr[i - 1] + f[prime[i]]) % p;
	
	for (int i = prime_cnt; i; i--) {
		long long lb = (long long)prime[i] * prime[i];

		for (int j = val[0]; j && val[j] >= lb; j--)
			for (long long pc = prime[i]; pc <= val[j]; pc *= prime[i]) {
				int t = getid(val[j] / pc);

				int tmp = dp[t];
				if (r[t] > i)
					tmp = (tmp + pr[r[t]] - pr[max(i, l[t])]) % p;
				
				dp[j] = (dp[j] + pc % p * ((pc - 1) % p) % p * tmp) % p;
			}
	}

	return (long long)(dp[val[0]] + pr[r[val[0]]] - pr[l[val[0]]] + p) % p;
}

int main() {

	// ios::sync_with_stdio(false);

	cin >> N;

	sqrtn = (int)sqrt(N);

	get_table(sqrtn);
	
	for (int i = 1; i <= sqrtn; i++)
		val[++val[0]] = i;
	
	for (int i = 1; i <= sqrtn; i++)
		val[++val[0]] = N / i;
	
	sort(val + 1, val + val[0] + 1);
	
	val[0] = unique(val + 1, val + val[0] + 1) - val - 1;

	int li = 0, ri = 0;
	for (int j = 1; j <= val[0]; j++) {
		while (ri < prime_cnt && prime[ri + 1] <= val[j])
			ri++;
		
		while (li <= prime_cnt && (long long)prime[li] * prime[li] <= val[j])
			li++;
		
		l[j] = li - 1;
		r[j] = ri;
	}

	cout << (calc1(N) + calc2(N)) % p << endl;

	return 0;
}