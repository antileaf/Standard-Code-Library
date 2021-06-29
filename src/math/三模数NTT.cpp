//以下为三模数NTT, 原理是选取三个乘积大于结果的NTT模数, 最后中国剩余定理合并
//以对23333333(不是质数)取模为例
constexpr int maxn = 262200, Mod = 23333333, g = 3, m[] = {998244353, 1004535809, 1045430273}, m0_inv = 669690699, m1_inv = 332747959, M_inv = 942377029; // 这三个模数最小原根都是3
constexpr long long M = (long long)m[0] * m[1];

// 主函数(当然更多时候包装一下比较好)
// 用来卷积的是A和B
// 需要调用mul
int n, N = 1, A[maxn], B[maxn], C[maxn], D[maxn], ans[3][maxn];
int main() {
	scanf("%d", &n);

	while (N < n * 2)
		N *= 2;

	for (int i = 0; i < n; i++)
		scanf("%d", &A[i]);
	for (int i = 0; i < n; i++)
		scanf("%d", &B[i]);

	for (int i = 0; i < 3; i++)
		mul(m[i], ans[i]);

	for (int i = 0; i < n; i++)
		printf("%d ", China(ans[0][i], ans[1][i], ans[2][i]));

	return 0;
}

// mul O(n\log n)
// 包装了模NTT模数的卷积
// 需要调用NTT
void mul(int p, int *ans) {
	copy(A, A + N, C);
	copy(B, B + N, D);

	NTT(C, N, 1, p);
	NTT(D, N, 1, p);

	for (int i = 0; i < N; i++)
		ans[i] = (long long)C[i] * D[i] % p;

	NTT(ans, N, -1, p);
}

// 中国剩余定理 O(1)
// 由于直接合并会爆long long, 采用神奇的方法合并
// 需要调用O(1)快速乘
inline int China(int a0, int a1, int a2) {
	long long A = (mul((long long)a0 * m1_inv, m[1], M) + mul((long long)a1 * m0_inv, m[0], M)) % M;
	int k = ((a2 - A) % m[2] + m[2]) % m[2] * M_inv % m[2];
	return (k % Mod * (M % Mod) % Mod + A % Mod) % Mod;
}