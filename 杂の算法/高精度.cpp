// 注意如果只需要正数运算的话
// 可以只抄英文名的运算函数
// 按需自取
// 乘法O(n ^ 2)，除法O(10 * n ^ 2)

const int maxn = 1005;

struct big_decimal {
	int a[maxn];
	bool negative;

	big_decimal() {
		memset(a, 0, sizeof(a));
		negative = false;
	}

	big_decimal(long long x) {
		memset(a, 0, sizeof(a));
		negative = false;

		if (x < 0) {
			negative = true;
			x = -x;
		}

		while (x) {
			a[++a[0]] = x % 10;
			x /= 10;
		}
	}

	big_decimal(string s) {
		memset(a, 0, sizeof(a));
		negative = false;

		if (s == "")
			return;

		if (s[0] == '-') {
			negative = true;
			s = s.substr(1);
		}
		a[0] = s.size();
		for (int i = 1; i <= a[0]; i++)
			a[i] = s[a[0] - i] - '0';
		
		while (a[0] && !a[a[0]])
			a[0]--;
	}

	void input() {
		string s;
		cin >> s;
		*this = s;
	}

	string str() const {
		if (!a[0])
			return "0";
		
		string s;
		if (negative)
			s = "-";
		
		for (int i = a[0]; i; i--)
			s.push_back('0' + a[i]);
		
		return s;
	}

	operator string () const {
		return str();
	}

	big_decimal operator - () const {
		big_decimal o = *this;
		if (a[0])
			o.negative ^= true;
		
		return o;
	}

	friend big_decimal abs(const big_decimal &u) {
		big_decimal o = u;
		o.negative = false;
		return o;
	}

	big_decimal &operator <<= (int k) {
		a[0] += k;
		
		for (int i = a[0]; i > k; i--)
			a[i] = a[i - k];

		for(int i = k; i; i--)
			a[i] = 0;
		
		return *this;
	}

	friend big_decimal operator << (const big_decimal &u, int k) {
		big_decimal o = u;
		return o <<= k;
	}

	big_decimal &operator >>= (int k) {
		if (a[0] < k)
			return *this = big_decimal(0);
		
		a[0] -= k;
		for (int i = 1; i <= a[0]; i++)
			a[i] = a[i + k];
		
		for (int i = a[0] + 1; i <= a[0] + k; i++)
			a[i] = 0;
		
		return *this;
	}

	friend big_decimal operator >> (const big_decimal &u, int k) {
		big_decimal o = u;
		return o >>= k;
	}

	friend int cmp(const big_decimal &u, const big_decimal &v) {
		if (u.negative || v.negative) {
			if (u.negative && v.negative)
				return -cmp(-u, -v);
			
			if (u.negative)
				return -1;
			
			if (v.negative)
				return 1;
		}

		if (u.a[0] != v.a[0])
			return u.a[0] < v.a[0] ? -1 : 1;
		
		for (int i = u.a[0]; i; i--)
			if (u.a[i] != v.a[i])
				return u.a[i] < v.a[i] ? -1 : 1;
		
		return 0;
	}

	friend bool operator < (const big_decimal &u, const big_decimal &v) {
		return cmp(u, v) == -1;
	}

	friend bool operator > (const big_decimal &u, const big_decimal &v) {
		return cmp(u, v) == 1;
	}

	friend bool operator == (const big_decimal &u, const big_decimal &v) {
		return cmp(u, v) == 0;
	}

	friend bool operator <= (const big_decimal &u, const big_decimal &v) {
		return cmp(u, v) <= 0;
	}

	friend bool operator >= (const big_decimal &u, const big_decimal &v) {
		return cmp(u, v) >= 0;
	}

	friend big_decimal decimal_plus(const big_decimal &u, const big_decimal &v) { // 保证u, v均为正数的话可以直接调用
		big_decimal o;

		o.a[0] = max(u.a[0], v.a[0]);

		for (int i = 1; i <= u.a[0] || i <= v.a[0]; i++) {
			o.a[i] += u.a[i] + v.a[i];

			if (o.a[i] >= 10) {
				o.a[i + 1]++;
				o.a[i] -= 10;
			}
		}

		if (o.a[o.a[0] + 1])
			o.a[0]++;
		
		return o;
	}

	friend big_decimal decimal_minus(const big_decimal &u, const big_decimal &v) { // 保证u, v均为正数的话可以直接调用
		int k = cmp(u, v);
		
		if (k == -1)
			return -decimal_minus(v, u);
		else if (k == 0)
			return big_decimal(0);
		
		big_decimal o;

		o.a[0] = u.a[0];

		for (int i = 1; i <= u.a[0]; i++) {
			o.a[i] += u.a[i] - v.a[i];
			
			if (o.a[i] < 0) {
				o.a[i] += 10;
				o.a[i + 1]--;
			}
		}

		while (o.a[0] && !o.a[o.a[0]])
			o.a[0]--;

		return o;
	}

	friend big_decimal decimal_multi(const big_decimal &u, const big_decimal &v) {
		big_decimal o;
		
		o.a[0] = u.a[0] + v.a[0] - 1;

		for (int i = 1; i <= u.a[0]; i++)
			for (int j = 1; j <= v.a[0]; j++)
				o.a[i + j - 1] += u.a[i] * v.a[j];
		
		for (int i = 1; i <= o.a[0]; i++)
			if (o.a[i] >= 10) {
				o.a[i + 1] += o.a[i] / 10;
				o.a[i] %= 10;
			}
		
		if (o.a[o.a[0] + 1])
			o.a[0]++;
		
		return o;
	}

	friend pair<big_decimal, big_decimal> decimal_divide(big_decimal u, big_decimal v) { // 整除
		if (v > u)
			return make_pair(big_decimal(0), u);
		
		big_decimal o;
		o.a[0] = u.a[0] - v.a[0] + 1;

		int m = v.a[0];
		v <<= u.a[0] - m;
		
		for (int i = u.a[0]; i >= m; i--) {
			while (u >= v) {
				u = u - v;
				o.a[i - m + 1]++;
			}

			v >>= 1;
		}

		while (o.a[0] && !o.a[o.a[0]])
			o.a[0]--;
		
		return make_pair(o, u);
	}

	friend big_decimal operator + (const big_decimal &u, const big_decimal &v) {
		if (u.negative || v.negative) {
			if (u.negative && v.negative)
				return -decimal_plus(-u, -v);
			
			if (u.negative)
				return v - (-u);
			
			if (v.negative)
				return u - (-v);
		}

		return decimal_plus(u, v);
	}

	friend big_decimal operator - (const big_decimal &u, const big_decimal &v) {
		if (u.negative || v.negative) {
			if (u.negative && v.negative)
				return -decimal_minus(-u, -v);
			
			if (u.negative)
				return -decimal_plus(-u, v);
			
			if (v.negative)
				return decimal_plus(u, -v);
		}

		return decimal_minus(u, v);
	}

	friend big_decimal operator * (const big_decimal &u, const big_decimal &v) {
		if (u.negative || v.negative) {
			big_decimal o = decimal_multi(abs(u), abs(v));

			if (u.negative ^ v.negative)
				return -o;
			return o;
		}

		return decimal_multi(u, v);
	}

	big_decimal operator * (long long x) const {
		if (x >= 10)
			return *this * big_decimal(x);
		
		if (negative)
			return -(*this * x);
		
		big_decimal o;

		o.a[0] = a[0];

		for (int i = 1; i <= a[0]; i++) {
			o.a[i] += a[i] * x;

			if (o.a[i] >= 10) {
				o.a[i + 1] += o.a[i] / 10;
				o.a[i] %= 10;
			}
		}

		if (o.a[a[0] + 1])
			o.a[0]++;
		
		return o;
	}

	friend pair<big_decimal, big_decimal> decimal_div(const big_decimal &u, const big_decimal &v) {
		if (u.negative || v.negative) {
			pair<big_decimal, big_decimal> o = decimal_div(abs(u), abs(v));

			if (u.negative ^ v.negative)
				return make_pair(-o.first, -o.second);
			return o;
		}

		return decimal_divide(u, v);
	}

	friend big_decimal operator / (const big_decimal &u, const big_decimal &v) { // v不能是0
		if (u.negative || v.negative) {
			big_decimal o = abs(u) / abs(v);

			if (u.negative ^ v.negative)
				return -o;
			return o;
		}

		return decimal_divide(u, v).first;
	}

	friend big_decimal operator % (const big_decimal &u, const big_decimal &v) {
		if (u.negative || v.negative) {
			big_decimal o = abs(u) % abs(v);

			if (u.negative ^ v.negative)
				return -o;
			return o;
		}

		return decimal_divide(u, v).second;
	}
};