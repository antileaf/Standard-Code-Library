// 复杂度可以认为是常数

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


// 封装好的函数体
// 需要调用check
bool Miller_Rabin(long long n) {
	if (n == 1)
		return false;
	if (n == 2)
		return true;
	if (n % 2 == 0)
		return false;

	// int范围内只需要检查 {2, 7, 61}
	// long long范围内只需要检查 {2, 325, 9375, 28178, 450775, 9780504, 1795265022}

	for (int i : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
		if (i >= n)
			break;
		if (!check(n, i))
			return false;
	}
	
	return true;
}
