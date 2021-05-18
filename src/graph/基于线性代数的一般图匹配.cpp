// 这个算法基于Tutte定理和高斯消元,思维难度相对小一些,也更方便进行可行边的判定
// 注意这个算法复杂度是满的,并且常数有点大,而带花树通常是跑不满的
// 以及,根据Tutte定理,如果求最大匹配的大小的话直接输出Tutte矩阵的秩/2即可
// 需要输出方案时才需要再写后面那些乱七八糟的东西


// 复杂度和常数所限,1s之内500已经是这个算法的极限了
const int maxn = 505, p = 1000000007; // p可以是任意10^9以内的质数

// 全局数组和变量定义
int A[maxn][maxn], B[maxn][maxn], t[maxn][maxn], id[maxn], a[maxn];
bool row[maxn] = {false}, col[maxn] = {false};
int n, m, girl[maxn]; // girl是匹配点,用来输出方案

// 为了方便使用,贴上主函数
// 需要调用高斯消元和eliminate
int main() {
	srand(19260817); // 膜蛤专用随机种子,换一个也无所谓

	scanf("%d%d", &n, &m); // 点数和边数
	while (m--) {
		int x, y;
		scanf("%d%d", &x, &y);
		A[x][y] = rand() % p;
		A[y][x] = -A[x][y]; // Tutte矩阵是反对称矩阵
	}

	for (int i = 1; i <= n; i++)
		id[i] = i; // 输出方案用的,因为高斯消元的时候会交换列
	memcpy(t, A, sizeof(t));
	Gauss(A, NULL, n);
	
	m = n;
	n = 0; // 这里变量复用纯属个人习惯……

	for (int i = 1; i <= m; i++)
		if (A[id[i]][id[i]])
			a[++n] = i; // 找出一个极大满秩子矩阵

	for (int i = 1;i <= n; i++)
		for (int j = 1; j <= n; j++)
			A[i][j]=t[a[i]][a[j]];

	Gauss(A,B,n);

	for (int i = 1; i <= n; i++)
		if (!girl[a[i]])
			for (int j = i + 1; j <= n; j++)
				if (!girl[a[j]] && t[a[i]][a[j]] && B[j][i]) {
					// 注意上面那句if的写法,现在t是邻接矩阵的备份,
					// 逆矩阵j行i列不为0当且仅当这条边可行
					girl[a[i]] = a[j];
					girl[a[j]] = a[i];
					eliminate(i, j);
					eliminate(j, i);
					break;
				}

	printf("%d\n", n >> 1);
	for (int i = 1; i <= m; i++)
		printf("%d ", girl[i]);

	return 0;
}

// 高斯消元 O(n^3)
// 在传入B时表示计算逆矩阵,传入NULL则只需计算矩阵的秩
void Gauss(int A[][maxn], int B[][maxn], int n){
	if(B) {
		memset(B, 0, sizeof(t));
		for (int i = 1; i <= n; i++)
			B[i][i] = 1;
	}

	for (int i = 1; i <= n; i++) {
		if (!A[i][i]) {
			for (int j = i + 1; j <= n; j++)
				if (A[j][i]) {
					swap(id[i], id[j]);
					for (int k = i; k <= n; k++)
						swap(A[i][k], A[j][k]);

					if (B)
						for (int k = 1; k <= n; k++)
							swap(B[i][k], B[j][k]);
					break;
				}
				
			if (!A[i][i])
				continue;
		}

		int inv = qpow(A[i][i], p - 2);

		for (int j = 1; j <= n; j++)
			if (i != j && A[j][i]){
				int t = (long long)A[j][i] * inv % p;

				for (int k = i; k <= n; k++)
					if (A[i][k])
						A[j][k] = (A[j][k] - (long long)t * A[i][k]) % p;

				if (B)
					for (int k = 1; k <= n; k++)
						if (B[i][k])
							B[j][k] = (B[j][k] - (long long)t * B[i][k])%p;
			}
	}

	if (B)
		for (int i = 1; i <= n; i++) {
			int inv = qpow(A[i][i], p - 2);

			for (int j = 1; j <= n; j++)
				if (B[i][j])
					B[i][j] = (long long)B[i][j] * inv % p;
		}
}

// 消去一行一列 O(n^2)
void eliminate(int r, int c) {
	row[r] = col[c] = true; // 已经被消掉

	int inv = qpow(B[r][c], p - 2);

	for (int i = 1; i <= n; i++)
		if (!row[i] && B[i][c]) {
			int t = (long long)B[i][c] * inv % p;

			for (int j = 1; j <= n; j++)
				if (!col[j] && B[r][j])
					B[i][j] = (B[i][j] - (long long)t * B[r][j]) % p;
		}
}