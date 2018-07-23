//Convolution Modulo Integer 任意模数卷积 O(n\log n)
//By ysf
//通过题目：COGS2294 释迦（板子题）

//只要求模数在10^9以内，无其他特殊要求
//常数很大，慎用
//在卷积结果不超过10^14时可以直接double暴力乘，这时就不要写任意模数卷积了

//以下为三模数NTT，原理是选取三个乘积大于结果的NTT模数，最后中国剩余定理合并
//以对23333333（不是质数）取模为例
const int maxn=262200,Mod=23333333,g=3,m[]={998244353,1004535809,1045430273},
	m0_inv=669690699,m1_inv=332747959,M_inv=942377029;//这三个模数最小原根都是3
const long long M=(long long)m[0]*m[1];

//主函数（当然更多时候包装一下比较好）
//用来卷积的是A和B
//需要调用mul
int n,N=1,A[maxn],B[maxn],C[maxn],D[maxn],ans[3][maxn];
int main(){
	scanf("%d",&n);
	while(N<(n<<1))N<<=1;
	for(int i=0;i<n;i++)scanf("%d",&A[i]);
	for(int i=0;i<n;i++)scanf("%d",&B[i]);
	for(int i=0;i<3;i++)mul(m[i],ans[i]);
	for(int i=0;i<n;i++)printf("%d ",China(ans[0][i],ans[1][i],ans[2][i]));
	return 0;
}

//mul O(n\log n)
//包装了模NTT模数的卷积
//需要调用NTT
void mul(int p,int *ans){
	copy(A,A+N,C);
	copy(B,B+N,D);
	NTT(C,N,1,p);
	NTT(D,N,1,p);
	for(int i=0;i<N;i++)ans[i]=(long long)C[i]*D[i]%p;
	NTT(ans,N,-1,p);
}

//中国剩余定理 O(1)
//由于直接合并会爆long long，采用神奇的方法合并
//需要调用O(1)快速乘
inline int China(int a0,int a1,int a2){
	long long A=(mul((long long)a0*m1_inv,m[1],M)
		+mul((long long)a1*m0_inv,m[0],M))%M;
	int k=((a2-A)%m[2]+m[2])%m[2]*M_inv%m[2];
	return (k%Mod*(M%Mod)%Mod+A%Mod)%Mod;
}

//------------------------分割线--------------------------

//以下为拆系数FFT，原理是减小结果范围使得double精度能够承受
//仍然以模23333333为例
const int maxn=262200,p=23333333,M=4830;//M取

//需要开的数组
struct Complex{//内容略
}w[maxn],w_inv[maxn],A[maxn],B[maxn],C[maxn],D[maxn],F[maxn],G[maxn],H[maxn];

//主函数（当然更多时候包装一下比较好）
//需要调用FFT初始化、FFT
int main(){
	scanf("%d",&n);
	int N=1;
	while(N<(n<<1))N<<=1;
	for(int i=0,x;i<n;i++){
		scanf("%d",&x);
		A[i]=x/M;
		B[i]=x%M;
	}
	for(int i=0,x;i<n;i++){
		scanf("%d",&x);
		C[i]=x/M;
		D[i]=x%M;
	}
	FFT_init(N);
	FFT(A,N,1);
	FFT(B,N,1);
	FFT(C,N,1);
	FFT(D,N,1);
	for(int i=0;i<N;i++){
		F[i]=A[i]*C[i];
		G[i]=A[i]*D[i]+B[i]*C[i];
		H[i]=B[i]*D[i];
	}
	FFT(F,N,-1);
	FFT(G,N,-1);
	FFT(H,N,-1);
	for(int i=0;i<n;i++)
		printf("%d\n",(int)((M*M*((long long)(F[i].a+0.5)%p)%p+
		M*((long long)(G[i].a+0.5)%p)%p+(long long)(H[i].a+0.5)%p)%p));
	return 0;
}