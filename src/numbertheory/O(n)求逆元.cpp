// 要求p为质数

inv[0] = inv[1] = 1;
for (int i = 2; i <= n; i++)
	inv[i] = (long long)(p - (p / i)) * inv[p % i] % p; // p为模数
// i ^ -1 = -(p / i) * (p % i) ^ -1