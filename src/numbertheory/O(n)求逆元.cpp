inv[1] = 1; // 如果 p 是素数，那 p % i 必不可能是 0
for (int i = 2; i <= n; i++)
	inv[i] = (long long)(p - (p / i)) * inv[p % i] % p;
