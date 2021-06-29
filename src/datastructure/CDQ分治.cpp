// 四维偏序

void CDQ1(int l, int r) {
	if (l >= r)
		return;

	int mid = (l + r) / 2;

	CDQ1(l, mid);
	CDQ1(mid + 1, r);

	int i = l, j = mid + 1, k = l;

	while (i <= mid && j <= r) {
		if (a[i].x < a[j].x) {
			a[i].ins = true;
			b[k++] = a[i++];
		}
		else {
			a[j].ins = false;
			b[k++] = a[j++];
		}
	}

	while (i <= mid) {
		a[i].ins = true;
		b[k++] = a[i++];
	}

	while (j <= r) {
		a[j].ins = false;
		b[k++] = a[j++];
	}

	copy(b + l, b + r + 1, a + l); // 后面的分治会破坏排序, 所以要复制一份

	CDQ2(l, r);
}

void CDQ2(int l, int r) {
	if (l >= r)
		return;

	int mid = (l + r) / 2;

	CDQ2(l, mid);
	CDQ2(mid + 1, r);

	int i = l, j = mid + 1, k = l;

	while (i <= mid && j <= r) {
		if (b[i].y < b[j].y) {
			if (b[i].ins)
				add(b[i].z, 1); // 树状数组

			t[k++] = b[i++];
		}
		else{
			if (!b[j].ins)
				ans += query(b[j].z - 1);

			t[k++] = b[j++];
		}
	}

	while (i <= mid) {
		if (b[i].ins)
			add(b[i].z, 1);

		t[k++] = b[i++];
	}

	while (j <= r) {
		if (!b[j].ins)
			ans += query(b[j].z - 1);

		t[k++] = b[j++];
	}

	for (i = l; i <= mid; i++)
		if (b[i].ins)
			add(b[i].z, -1);

	copy(t + l, t + r + 1, b + l);
}