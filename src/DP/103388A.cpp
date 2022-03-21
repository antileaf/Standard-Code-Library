constexpr int maxn = 5005, p = (int)1e9 + 7;
 
int inv[maxn];
int a[maxn], f[maxn][maxn], dp[maxn];
 
int main() {
 
	int n, m;
	scanf("%d%d", &n, &m);
	
	inv[1] = 1;
	for (int i = 2; i <= n; i++)
		inv[i] = (long long)(p - p / i) * inv[p % i] % p;
 
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
		a[i] = m - a[i] + 1;
	}
 
	if (any_of(a + 1, a + n + 1, [] (int x) {return x <= 0;})) {
		printf("0\n");
		return 0;
	}
 
	for (int i = n - 1; i; i--)
		a[i] = min(a[i], a[i + 1]);
 
	// b_i >= b_{i - 1} && b_i <= a_i
	// 我们可以假设 b_i <= a_i 是必定被满足的，然后对 bi 非严格递增的条件进行容斥，枚举某一段是严格递减的
	// 如果 [j, i] 严格递减，显然它们都 <= a_j，所以这个区间的方案数是 {a_j \choose i - j + 1}
	// 如果 i 是合法的，直接一个个转移即可，因为这一部分的转移和区间长度没有关系
 
	for (int i = 1; i <= n; i++) {
		f[i][0] = 1;
 
		for (int j = 1; j <= n - i + 1 && j <= a[i]; j++)
			f[i][j] = (long long)f[i][j - 1] * (a[i] - j + 1) % p * inv[j] % p;
	}
 
	dp[0] = 1;
 
	for (int i = 1; i <= n; i++) {
		dp[i] = (long long)dp[i - 1] * a[i] % p;
 
		for (int j = 1; j < i; j++) {
			int tmp = (long long)dp[j - 1] * f[j][i - j + 1] % p;
 
			if ((i - j) % 2)
				tmp = p - tmp;
			
			dp[i] = (dp[i] + tmp) % p;
		}
	}
 
	printf("%d\n", dp[n]);
 
	return 0;
}