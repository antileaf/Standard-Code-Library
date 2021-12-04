double kahanSum(vector<double> vec) {
	double sum = 0, c = 0;
	for (auto x : vec) {
		double y = x - c;
		double t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}
	return sum;
}