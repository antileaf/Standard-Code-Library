struct Card {
	int suit, value; // Ace is treated as 14

	Card(string s) {
		char a = s[0];

		if (isdigit(a))
			value = a - '0';
		else if (a == 'T')
			value = 10;
		else if (a == 'A')
			value = 14;
		else if (a == 'J')
			value = 11;
		else if (a == 'Q')
			value = 12;
		else if (a == 'K')
			value = 13;
		else
			value = -1; // error
		
		char b = s[1];
		suit = b; // Club, Diamond, Heart, Spade
	}

	friend bool operator < (const Card &a, const Card &b) {
		return a.value < b.value;
	}

	friend bool operator == (const Card &a, const Card &b) {
		return a.value == b.value;
	}
};

constexpr int Highcard = 1, Pair = 2, TwoPairs = 3, ThreeofaKind = 4, Straight = 5,
Flush = 6, FullHouse = 7, FourofaKind = 8, StraightFlush = 9;

struct Hand {
	vector<Card> v;
	int type;

	Hand() : type(0) {}

	Hand(const Hand &o) : v(o.v), type(o.type) {}

	Hand(const vector<Card> &v) : v(v), type(0) {}

	void init_high() {
		sort(v.begin(), v.end()); // 升序排序
		
		bool straight = false;
		if (v.back().value == 14) {
			if (v[0].value == 2 && v[1].value == 3 && v[2].value == 4 && v[3].value == 5) {
				straight = true;
				rotate(v.begin(), v.begin() + 1, v.end());
			}
		}

		if (!straight) {
			bool ok = true;
			for (int i = 1; i < 5; i++)
				ok &= (v[i].value == v[i - 1].value + 1);
			
			if (ok)
				straight = true;
		}

		bool flush = all_of(v.begin(), v.end(), [&] (const Card &a) {return a.suit == v.front().suit;});

		if (flush && straight) { // 同花顺
			type = StraightFlush;
			reverse(v.begin(), v.end());
			return;
		}

		vector<int> c;
		c.assign(15, 0);

		for (auto &o : v)
			c[o.value]++;
		
		vector<int> kind[5];

		for (int i = 2; i <= 14; i++)
			if (c[i] > 1)
				kind[c[i]].push_back(i);
		
		if (!kind[4].empty()) { // 四条
			type = FourofaKind;

			for (int i = 0; i < 4; i++)
				if (v[i].value != kind[4].front()) {
					swap(v[i], v.back());
					break;
				}
			
			return;
		}

		if (!kind[3].empty() && !kind[2].empty()) {
			type = FullHouse;

			sort(v.begin(), v.end(), [&] (const Card &a, const Card &b) {
				bool ta = (a.value == kind[3].front()), tb = (b.value == kind[3].front());

				return ta > tb;
			});

			return;
		}

		if (flush) {
			type = Flush;
			sort(v.begin(), v.end());
			reverse(v.begin(), v.end());

			return;
		}

		if (straight) {
			type = Straight;
			reverse(v.begin(), v.end());
			return;
		}

		if (!kind[3].empty()) {
			type = ThreeofaKind;

			sort(v.begin(), v.end(), [&] (const Card &a, const Card &b) {
				bool ta = (a.value == kind[3].front()), tb = (b.value == kind[3].front());

				return ta > tb;
			});
			
			if (v[3] < v[4])
				swap(v[3], v[4]);
			
			return;
		}

		if ((int)kind[2].size() == 2) {
			type = TwoPairs;

			sort(v.begin(), v.end(), [&] (const Card &a, const Card &b) {
				bool ta = (c[a.value] == 2), tb = (c[b.value] == 2);

				if (ta != tb)
					return ta > tb;
				
				return a.value > b.value;
			});

			return;
		}

		if ((int)kind[2].size() == 1) {
			type = Pair;

			sort(v.begin(), v.end(), [&] (const Card &a, const Card &b) {
				bool ta = (c[a.value] == 2), tb = (c[b.value] == 2);

				if (ta != tb)
					return ta > tb;
				
				return a.value > b.value;
			});

			return;
		}

		type = Highcard;

		sort(v.begin(), v.end());
		reverse(v.begin(), v.end());
	}

	void init_low() {
		for (auto &o : v)
			if (o.value == 14)
				o.value = 1;

		sort(v.begin(), v.end());
		reverse(v.begin(), v.end());
	}

	friend int cmp_high(const Hand &a, const Hand &b) {
		if (a.type != b.type)
			return a.type < b.type ? -1 : 1;
		
		if (a.v != b.v)
			return a.v < b.v ? -1 : 1;
		
		return 0;
	}

	friend bool small_high(const Hand &a, const Hand &b) {
		return cmp_high(a, b) < 0;
	}

	friend int cmp_low(const Hand &a, const Hand &b) {
		for (int i = 0; i < 5; i++)
			if (a.v[i].value != b.v[i].value)
				return a.v[i] < b.v[i] ? 1 : -1;
			
		return 0;
	}

	friend bool small_low(const Hand &a, const Hand &b) {
		return cmp_low(a, b) < 0;
	}

	bool operator ! () const {
		return v.empty();
	}

	string str() const {
		stringstream ss;

		for (auto &o : v)
			ss << o.value << ' ';
		
		return ss.str();
	}
};

Hand get_max_high(vector<Card> u, vector<Card> v) { // private, public
	Hand ans;

	for (int i = 0; i < 4; i++)
		for (int j = i + 1; j < 4; j++)
			for (int k = 0; k < 5; k++)
				for (int p = k + 1; p < 5; p++)
					for (int q = p + 1; q < 5; q++) {
						Hand tmp({u[i], u[j], v[k], v[p], v[q]});

						tmp.init_high();

						if (!ans || cmp_high(tmp, ans) > 0)
							ans = tmp;
					}
	
	return ans;
}

Hand get_max_low(vector<Card> tu, vector<Card> tv) {
	vector<Card> u, v;

	for (auto o : tu)
		if (o.value == 14 || o.value <= 8)
			u.push_back(o);

	for (auto o : tv)
		if (o.value == 14 || o.value <= 8)
			v.push_back(o);
	
	Hand ans;

	for (int i = 0; i < (int)u.size(); i++)
		for (int j = i + 1; j < (int)u.size(); j++)
			for (int k = 0; k < (int)v.size(); k++)
				for (int p = k + 1; p < (int)v.size(); p++)
					for (int q = p + 1; q < (int)v.size(); q++) {
						vector<Card> vec = {u[i], u[j], v[k], v[p], v[q]};

						bool bad = false;

						for (int a = 0; a < 5; a++)
							for (int b = a + 1; b < 5; b++)
								if (vec[a].value == vec[b].value)
									bad = true;
						
						if (bad)
							continue;

						Hand tmp(vec);

						tmp.init_low();

						if (!ans || cmp_low(tmp, ans) > 0)
							ans = tmp;
					}
	
	return ans;
}

int main() {

	ios::sync_with_stdio(false);

	int T;
	cin >> T;

	while (T--) {
		int p;
		cin >> p;

		vector<Card> alice, bob, pub;

		for (int i = 0; i < 4; i++) {
			string s;
			cin >> s;
			alice.push_back(Card(s));
		}

		for (int i = 0; i < 4; i++) {
			string s;
			cin >> s;
			bob.push_back(Card(s));
		}

		for (int i = 0; i < 5; i++) {
			string s;
			cin >> s;
			pub.push_back(Card(s));
		}

		Hand alice_high = get_max_high(alice, pub), bob_high = get_max_high(bob, pub);
		Hand alice_low = get_max_low(alice, pub), bob_low = get_max_low(bob, pub);
		
		int dh = cmp_high(alice_high, bob_high);
		int ans[2] = {0};

		if (!alice_low && !bob_low) {
			if (!dh) {
				ans[0] = p - p / 2;
				ans[1] = p / 2;
			}
			else
				ans[dh == -1] = p;
		}
		else if (!alice_low || !bob_low) {
			ans[!alice_low] += p / 2;

			if (!dh) {
				ans[0] += p - p / 2 - (p - p / 2) / 2;
				ans[1] += (p - p / 2) / 2;
			}
			else
				ans[dh == -1] += p - p / 2;
		}
		else {
			int dl = cmp_low(alice_low, bob_low);

			if (!dl) {
				ans[0] += p / 2 - p / 2 / 2;
				ans[1] += p / 2 / 2;
			}
			else
				ans[dl == -1] += p / 2;

			if (!dh) {
				ans[0] += p - p / 2 - (p - p / 2) / 2;
				ans[1] += (p - p / 2) / 2;
			}
			else
				ans[dh == -1] += p - p / 2;
		}

		cout << ans[0] << ' ' << ans[1] << '\n';
	}

	return 0;
}