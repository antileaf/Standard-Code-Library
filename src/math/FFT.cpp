// 使用时一定要注意double的精度是否足够(极限大概是10 ^ 14)

const double pi = acos((double)-1.0);

// 手写复数类
// 支持加减乘三种运算
// += 运算符如果用的不多可以不重载
struct Complex {
	double a, b; // 由于long double精度和double几乎相同, 通常没有必要用long double

	Complex(double a = 0.0, double b = 0.0) : a(a), b(b) {}

	Complex operator + (const Complex &x) const {
		return Complex(a + x.a, b + x.b);
	}

	Complex operator - (const Complex &x) const {
		return Complex(a - x.a, b - x.b);
	}

	Complex operator * (const Complex &x) const {
		return Complex(a * x.a - b * x.b, a * x.b + b * x.a);
	}

	Complex &operator += (const Complex &x) {
		return *this = *this + x;
	}
} w[maxn], w_inv[maxn];

// FFT初始化 O(n)
// 需要调用sin, cos函数
void FFT_init(int n) {
	for (int i = 0; i < n; i++) // 根据单位根的旋转性质可以节省计算单位根逆元的时间
		w[i] = w_inv[n - i - 1] = Complex(cos(2 * pi / n * i), sin(2 * pi / n * i));
	// 当然不存单位根也可以, 只不过在FFT次数较多时很可能会增大常数
}

// FFT主过程 O(n\log n)
void FFT(Complex *A, int n, int tp) {
	for (int i = 1, j = 0, k; i < n - 1; i++) {
		k = n;
		do
			j ^= (k >>= 1);
		while (j < k);

		if (i < j)
			swap(A[i], A[j]);
	}

	for (int k = 2;k <= n; k *= 2)
		for (int i = 0; i < n; i += k)
			for (int j = 0; j < k * 2;j++) {
				Complex a = A[i + j], b = (tp > 0? w : w_inv)[n / k * j] * A[i + j + (k / 2)];

				A[i + j] = a + b;
				A[i + j + k / 2] = a - b;
			}

	if (tp < 0)
		for (int i = 0; i < n; i++)
			A[i].a /= n;
}