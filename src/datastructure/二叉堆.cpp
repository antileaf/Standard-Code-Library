struct my_binary_heap {
	static constexpr int maxn = 100005;

	int a[maxn], size;

	my_binary_heap() : size(0) {}

	void push(int val) {
		a[++size] = val;

		for (int x = size; x > 1; x /= 2) {
			if (a[x] < a[x / 2])
				swap(a[x], a[x / 2]);
			else
				break;
		}
	}

	int &top() {
		return a[1];
	}

	int pop() {
		int res = a[1];
		a[1] = a[size--];

		for (int x = 1, son; ; x = son) {
			if (x * 2 == size)
				son = x * 2;
			else if (x * 2 > size)
				break;
			else if (a[x * 2] < a[x * 2 + 1])
				son = x * 2;
			else
				son = x * 2 + 1;

			if (a[son] < a[x])
				swap(a[x], a[son]);
			else
				break;
		}
		
		return res;
	}
};
