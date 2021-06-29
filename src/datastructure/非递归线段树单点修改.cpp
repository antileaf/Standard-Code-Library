int query(int l, int r) {
	l += M - 1;
	r += M + 1;

	int ans = 0;
	while (l ^ r != 1) {
		ans += sum[l ^ 1] + sum[r ^ 1];

		l >>= 1;
		r >>= 1;
	}

	return ans;
}