struct LinearRecurrence {
	vector<int> first, trans;
	vector<vector<int> > bin;

	vector<int> multi(const vector<int> &a, const vector<int> &b) {
		int n = (int)a.size() - 1;

		vector<int> c(n * 2 + 1);

		for (int i = 0; i <= n; i++)
			for (int j = 0; j <= n; j++)
				c[i + j] = (c[i + j] + (long long)a[i] * b[j]) % p;
		
		for (int i = n * 2; i > n; i--) {
			for (int j = 0; j < n; j++)
				c[i - 1 - j] = (c[i - 1 - j] + (long long)c[i] * trans[j]) % p;
			
			c[i] = 0;
		}

		c.resize(n + 1);
		return c;
	}

	LinearRecurrence(vector<int> &first, vector<int> &trans) : first(first), trans(trans) {
		int n = (int)first.size();

		vector<int> a(n + 1);
		a[1] = 1;
		bin.push_back(a);

		for (int i = 1; i < 64; i++)
			bin.push_back(multi(bin[i - 1], bin[i - 1]));
	}

	int calc(long long k) {
		int n = (int)first.size();

		vector<int> a(n + 1);
		a[0] = 1;

		for (int i = 0; i < 64; i++)
			if (k >> i & 1)
				a = multi(a, bin[i]);
		
		int ans = 0;
		for (int i = 0; i < n; i++)
			ans = (ans + (long long)a[i + 1] * first[i]) % p;
		
		return ans;
	}
};