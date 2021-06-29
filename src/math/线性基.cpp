void add(unsigned long long x) {
	for (int i = 63; i >= 0; i--)
		if (x >> i & 1) {
			if (b[i])
				x ^= b[i];
			else {
				b[i] = x;

				for (int j = i - 1; j >= 0; j--)
					if (b[j] && (b[i] >> j & 1))
						b[i] ^= b[j];
				
				for (int j = i + 1; j < 64; j++)
					if (b[j] >> i & 1)
						b[j] ^= b[i];
				
				break;
			}
		}
}