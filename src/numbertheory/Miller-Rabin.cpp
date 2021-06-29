// 复杂度可以认为是常数

// 封装好的函数体
// 需要调用check
bool Miller_Rabin(long long n) {
	if (n == 1)
		return false;
	if (n == 2)
		return true;
	if (n % 2 == 0)
		return false;

	for (int i : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
		if (i > n)
			break;
		if (!check(n, i))
			return false;
	}
	
	return true;
}

// 用一个数检测
// 需要调用long long快速幂和O(1)快速乘
bool check(long long n, long long b) { // b: base
	long long a = n - 1;
	int k = 0;
	
	while (a % 2 == 0) {
		a /= 2;
		k++;
	}

	long long t = qpow(b, a, n); // 这里的快速幂函数需要写O(1)快速乘
	if (t == 1 || t == n - 1)
		return true;

	while (k--) {
		t = mul(t, t, n); // mul是O(1)快速乘函数
		if(t == n - 1)
			return true;
	}

	return false;
}