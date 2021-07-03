void exgcd(LL a, LL b, LL &c, LL &x, LL &y) {
	if (b == 0) {
		c = a;
		x = 1;
		y = 0;
		return;
	}

	exgcd(b, a % b, c, x, y);

	LL tmp = x;
	x = y;
	y = tmp - (a / b) * y;