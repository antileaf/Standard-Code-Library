//注意,虽然Pollard's Rho的理论复杂度是O(n^{1/4})的,
//但实际跑起来比较慢,一般用于做long long范围内的质因数分解


//封装好的函数体
//需要调用solve
void factorize(long long n,vector<long long>&v){//v用于存分解出来的质因子,重复的会放多个
	for(int i:{2,3,5,7,11,13,17,19})
		while(n%i==0){
			v.push_back(i);
			n/=i;
		}
	solve(n,v);
	sort(v.begin(),v.end());//从小到大排序后返回
}

//递归过程
//需要调用Pollard's Rho主过程,同时递归调用自身
void solve(long long n,vector<long long>&v){
	if(n==1)return;
	long long p;
	do p=Pollards_Rho(n);while(!p);//p是任意一个非平凡因子
	if(p==n){
		v.push_back(p);//说明n本身就是质数
		return;
	}
	solve(p,v);//递归分解两半
	solve(n/p,v);
}

//Pollard's Rho主过程
//需要使用Miller-Rabin作为子算法
//同时需要调用O(1)快速乘和gcd函数
long long Pollards_Rho(long long n){
	assert(n>1);
	if(Miller_Rabin(n))return n;
	long long c=rand()%(n-2)+1,i=1,k=2,x=rand()%(n-3)+2,u=2;//注意这里rand函数需要重定义一下
	for(;;){
		i++;
		x=(mul(x,x,n)+c)%n;//mul是O(1)快速乘函数
		long long g=gcd((u-x+n)%n,n);
		if(g>1&&g<n)return g;
		if(u==x)return 0;//失败，需要重新调用
		if(i==k){
			u=x;
			k<<=1;
		}
	}
}