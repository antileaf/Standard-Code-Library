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

	Complex operator * (double x) const {
		return Complex(a * x, b * x);
	}

	Complex &operator += (const Complex &x) {
		return *this = *this + x;
	}

	Complex conj() const { // 共轭, 一般只有MTT需要用
		return Complex(a, -b);
	}
} omega[maxn], omega_inv[maxn];
const Complex ima = Complex(0, 1);

int fft_n; // 要在主函数里初始化

// FFT初始化
void FFT_init(int n) {
	fft_n = n;

	for (int i = 0; i < n; i++) // 根据单位根的旋转性质可以节省计算单位根逆元的时间
		omega[i] = Complex(cos(2 * pi / n * i), sin(2 * pi / n * i));
	
	omega_inv[0] = omega[0];
	for (int i = 1; i < n; i++)
		omega_inv[i] = omega[n - i];
	// 当然不存单位根也可以, 只不过在FFT次数较多时很可能会增大常数
}

// FFT主过程
void FFT(Complex *a, int n, int tp) {
	for (int i = 1, j = 0, k; i < n - 1; i++) {
		k = n;
		do
			j ^= (k >>= 1);
		while (j < k);

		if (i < j)
			swap(a[i], a[j]);
	}

	for (int k = 2, m = fft_n / 2; k <= n; k *= 2, m /= 2)
		for (int i = 0; i < n; i += k)
			for (int j = 0; j < k / 2; j++) {
				Complex u = a[i + j], v = (tp > 0 ? omega : omega_inv)[m * j] * a[i + j + k / 2];

				a[i + j] = u + v;
				a[i + j + k / 2] = u - v;
			}

	if (tp < 0)
		for (int i = 0; i < n; i++) {
			a[i].a /= n;
			a[i].b /= n; // 一般情况下是不需要的, 只有MTT时才需要
		}
}