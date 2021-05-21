const double eps = 1e-10;

double A[maxn][maxn], x[maxn];
int n, m, t, id[maxn * 2];

// 方便起见,这里附上主函数
int main() {
	scanf("%d%d%d", &n, &m, &t);

	for (int i = 1; i <= n; i++) {
		scanf("%lf", &A[0][i]);
		id[i] = i;
	}

	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++)
			scanf("%lf", &A[i][j]);

		scanf("%lf", &A[i][0]);
	}

	if (!initalize())
		printf("Infeasible"); // 无解
	else if (!simplex())
		printf("Unbounded"); // 最优解无限大

	else {
		printf("%.15lf\n", -A[0][0]);
		if (t) {
			for (int i = 1; i <= m; i++)
				x[id[i + n]] = A[i][0];
			for (int i = 1; i <= n; i++)
				printf("%.15lf ",x[i]);
		}
	}
	return 0;
}

//初始化
//对于初始解可行的问题,可以把初始化省略掉
bool initalize() {
	while (true) {
		double t = 0.0;
		int l = 0, e = 0;

		for (int i = 1; i <= m; i++)
			if (A[i][0] + eps < t) {
				t = A[i][0];
				l = i;
			}

		if (!l)
			return true;

		for (int i = 1; i <= n; i++)
			if (A[l][i] < -eps && (!e || id[i] < id[e]))
				e = i;

		if (!e)
			return false;

		pivot(l, e);
	}
}

//求解
bool simplex(){
	while (true) {
		int l = 0, e = 0;
		for (int i = 1; i <= n; i++)
			if (A[0][i] > eps && (!e || id[i] < id[e]))
				e = i;

		if (!e)
			return true;

		double t = 1e50;
		for (int i = 1; i <= m; i++)
			if (A[i][e] > eps && A[i][0] / A[i][e] < t) {
				l = i;
				t = A[i][0]/A[i][e];
			}

		if (!l)
			return false;

		pivot(l, e);
	}
}

//转轴操作,本质是在凸包上沿着一条棱移动
void pivot(int l, int e) {
	swap(id[e], id[n + l]);
	double t = A[l][e];
	A[l][e] = 1.0;
	
	for (int i = 0; i <= n; i++)
		A[l][i] /= t;

	for (int i = 0; i <= m; i++)
		if (i != l) {
			t = A[i][e];
			A[i][e] = 0.0;
			for (int j = 0; j <= n; j++)
				A[i][j] -= t * A[l][j];
		}
}
