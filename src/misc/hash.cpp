struct fuck_hash {
	fuck_hash() = default;

	size_t operator() (const fuck &f) const {
		return (size_t)f[0] ^ ((size_t)f[1] << 7) ^
			((size_t)f[2] << 15) ^ ((size_t)f[3] << 23);
	}
};

unordered_map<fuck, int, fuck_hash> cnt, sum;