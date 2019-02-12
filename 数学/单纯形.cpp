//Simplex Method 单纯形方法求解线性规划
//By ysf
//通过题目：UOJ#179 线性规划（然而被hack了QAQ……）

//单纯形其实是指数算法，但实践中跑得飞快，所以复杂度什么的也就无所谓了


const double eps=1e-10;

double A[maxn][maxn],x[maxn];
int n,m,t,id[maxn<<1];

//方便起见，这里附上主函数
int main(){
	scanf("%d%d%d",&n,&m,&t);
	for(int i=1;i<=n;i++){
		scanf("%lf",&A[0][i]);
		id[i]=i;
	}
	for(int i=1;i<=m;i++){
		for(int j=1;j<=n;j++)scanf("%lf",&A[i][j]);
		scanf("%lf",&A[i][0]);
	}
	if(!initalize())printf("Infeasible");
	else if(!simplex())printf("Unbounded");
	else{
		printf("%.15lf\n",-A[0][0]);
		if(t){
			for(int i=1;i<=m;i++)x[id[i+n]]=A[i][0];
			for(int i=1;i<=n;i++)printf("%.15lf ",x[i]);
		}
	}
	return 0;
}

//初始化
//对于初始解可行的问题，可以把初始化省略掉
bool initalize(){
	for(;;){
		double t=0.0;
		int l=0,e=0;
		for(int i=1;i<=m;i++)if(A[i][0]+eps<t){
			t=A[i][0];
			l=i;
		}
		if(!l)return true;
		for(int i=1;i<=n;i++)if(A[l][i]<-eps&&(!e||id[i]<id[e]))e=i;
		if(!e)return false;
		pivot(l,e);
	}
}

//求解
bool simplex(){
	for(;;){
		int l=0,e=0;
		for(int i=1;i<=n;i++)if(A[0][i]>eps&&(!e||id[i]<id[e]))e=i;
		if(!e)return true;
		double t=1e50;
		for(int i=1;i<=m;i++)if(A[i][e]>eps&&A[i][0]/A[i][e]<t){
			l=i;
			t=A[i][0]/A[i][e];
		}
		if(!l)return false;
		pivot(l,e);
	}
}

//转轴操作，本质是
void pivot(int l,int e){
	swap(id[e],id[n+l]);
	double t=A[l][e];
	A[l][e]=1.0;
	for(int i=0;i<=n;i++)A[l][i]/=t;
	for(int i=0;i<=m;i++)if(i!=l){
		t=A[i][e];
		A[i][e]=0.0;
		for(int j=0;j<=n;j++)A[i][j]-=t*A[l][j];
	}
}
