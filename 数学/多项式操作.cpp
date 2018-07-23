//Polymial Operations 多项式操作
//By ysf
//通过题目：COGS2189 帕秋莉的超级多项式（板子题）

const int maxn=262200;//以下所有代码均为NTT版本
//以下所有代码均满足：A为输入（不进行修改），C为输出，n为所需长度

//多项式求逆 O(n\log n)
//要求A常数项不为0
void getinv(int *A,int *C,int n){
	static int B[maxn];
	memset(C,0,sizeof(int)*(n<<1));
	C[0]=qpow(A[0],p-2);//一般题目直接赋值为1就可以
	for(int k=2;k<=n;k<<=1){
		memcpy(B,A,sizeof(int)*k);
		memset(B+k,0,sizeof(int)*k);
		NTT(B,k<<1,1);
		NTT(C,k<<1,1);
		for(int i=0;i<(k<<1);i++)
			C[i]=((2-(long long)B[i]*C[i])%p*C[i]%p+p)%p;
		NTT(C,k<<1,-1);
		memset(C+k,0,sizeof(int)*k);
	}
}

//多项式开根 O(n\log n)
//要求A常数项可以开根/存在二次剩余
//需要调用多项式求逆，且需要预处理2的逆元
void getsqrt(int *A,int *C,int n){
	static int B[maxn],D[maxn];
	memset(C,0,sizeof(int)*(n<<1));
	C[0]=(int)(sqrt(A[0])+1e-7);//一般题目直接赋值为1就可以
	for(int k=2;k<=n;k<<=1){
		memcpy(B,A,sizeof(int)*k);
		memset(B+k,0,sizeof(int)*k);
		getinv(C,D,k);
		NTT(B,k<<1,1);
		NTT(D,k<<1,1);
		for(int i=0;i<(k<<1);i++)B[i]=(long long)B[i]*D[i]%p;
		NTT(B,k<<1,-1);
		for(int i=0;i<k;i++)C[i]=(long long)(C[i]+B[i])*inv_2%p;//inv_2是2的逆元
	}
}

//求导 O(n)
void getderivative(int *A,int *C,int n){
	for(int i=1;i<n;i++)C[i-1]=(long long)A[i]*i%p;
	C[n-1]=0;
}

//不定积分 O(n\log n)，如果预处理过逆元可以降到O(n)
void getintegrate(int *A,int *C,int n){
	for(int i=1;i<n;i++)C[i]=(long long)A[i-1]*qpow(i,p-2)%p;
	C[0]=0;//由于是不定积分，结果没有常数项
}

//多项式\ln O(n\log n)
//要求A常数项不为0/存在离散对数
//需要调用多项式求逆、求导、不定积分
void getln(int *A,int *C,int n){//通常情况下A常数项都是1
	static int B[maxn];
	getderivative(A,B,n);
	memset(B+n,0,sizeof(int)*n);
	getinv(A,C,n);
	NTT(B,n<<1,1);
	NTT(C,n<<1,1);
	for(int i=0;i<(n<<1);i++)B[i]=(long long)B[i]*C[i]%p;
	NTT(B,n<<1,-1);
	getintegrate(B,C,n);
	memset(C+n,0,sizeof(int)*n);
}

//多项式\exp O(n\log n)
//要求A没有常数项
//需要调用多项式\ln
//常数很大且总代码较长，在时间效率要求不高时最好替换为分治FFT
//分治FFT依据：设G(x)=\exp F(x)，则有g_i=\sum_{k=1}^i f_k g_{i-k}
void getexp(int *A,int *C,int n){
	static int B[maxn];
	memset(C,0,sizeof(int)*(n<<1));
	C[0]=1;
	for(int k=2;k<=n;k<<=1){
		getln(C,B,k);
		for(int i=0;i<k;i++){
			B[i]=A[i]-B[i];
			if(B[i]<0)B[i]+=p;
		}
		(++B[0])%=p;
		NTT(B,k<<1,1);
		NTT(C,k<<1,1);
		for(int i=0;i<(k<<1);i++)C[i]=(long long)C[i]*B[i]%p;
		NTT(C,k<<1,-1);
		memset(C+k,0,sizeof(int)*k);
	}
}

//多项式k次幂 O(n\log n)
//在A常数项不为1时需要转化
//需要调用多项式/exp、\ln
//常数较大且总代码较长，在时间效率要求不高时最好替换为暴力快速幂
void getpow(int *A,int *C,int n,int k){
	static int B[maxn];
	getln(A,B,n);
	for(int i=0;i<n;i++)B[i]=(long long)B[i]*k%p;
	getexp(B,C,n);
}
