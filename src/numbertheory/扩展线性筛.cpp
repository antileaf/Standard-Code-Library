// 此代码以计算约数之和函数\sigma_1(对10^9+7取模)为例
// 适用于任何f(p^k)便于计算的积性函数
constexpr int p = 1000000007;

int prime[maxn / 10], sigma_one[maxn], f[maxn], g[maxn];
// f: 除掉最小质因子后剩下的部分
// g: 最小质因子的幂次, 在f(p^k)比较复杂时很有用, 但f(p^k)可以递推时就可以省略了
// 这里没有记录最小质因子, 但根据线性筛的性质, 每个合数只会被它最小的质因子筛掉
bool notp[maxn]; // 顾名思义

void get_table(int n) {
	sigma_one[1] = 1; // 积性函数必有f(1) = 1
	for (int i = 2; i <= n; i++) {
		if (!notp[i]) { // 质数情况
			prime[++prime[0]] = i;
			sigma_one[i] = i + 1;
			f[i] = g[i] = 1;
		}

		for (int j = 1; j <= prime[0] && i * prime[j] <= n; j++) {
			notp[i * prime[j]] = true;

			if (i % prime[j]) { // 加入一个新的质因子, 这种情况很简单
				sigma_one[i * prime[j]] = (long long)sigma_one[i] * (prime[j] + 1) % p;
				f[i * prime[j]] = i;
				g[i * prime[j]] = 1;
			}
			else { // 再加入一次最小质因子,需要再进行分类讨论
				f[i * prime[j]] = f[i];
				g[i * prime[j]] = g[i] + 1;
				// 对于f(p^k)可以直接递推的函数, 这里的判断可以改成
				// i / prime[j] % prime[j] != 0, 这样可以省下f[]的空间,
				// 但常数很可能会稍大一些
				
				if (f[i] == 1) // 质数的幂次, 这里\sigma_1可以递推
					sigma_one[i * prime[j]] = (sigma_one[i] + i * prime[j]) % p;
					// 对于更一般的情况,可以借助g[]计算f(p^k)
				else sigma_one[i * prime[j]] = // 否则直接利用积性, 两半乘起来
					(long long)sigma_one[i * prime[j] / f[i]] * sigma_one[f[i]] % p;
				break;
			}
		}
	}
}