//Yuhao Du's Sieve 杜教筛 O(n^{2/3})
//By ysf
//通过题目：51Nod1239 欧拉函数之和

//用于求可以用狄利克雷卷积构造出好求和的东西的函数的前缀和（有点绕）
//有些题只要求n<=10^9，这时就没必要开long long了，但记得乘法时强转

//常量/全局变量/数组定义
const int maxn=5000005,table_size=5000000,p=1000000007,inv_2=(p+1)/2;
bool notp[maxn];
int prime[maxn/20],phi[maxn],tbl[100005];
//tbl用来顶替哈希表，其实开到n^{1/3}就够了，不过保险起见开成\sqrt n比较好
long long N;

//主函数前面加上这么一句
memset(tbl,-1,sizeof(tbl));

//线性筛预处理部分略去

//杜教筛主过程 总计O(n^{2/3})
//递归调用自身
//递推式还需具体情况具体分析，这里以求欧拉函数前缀和(mod 10^9+7)为例
int S(long long n){
	if(n<=table_size)return phi[n];
	else if(~tbl[N/n])return tbl[N/n];
	//原理：n除以所有可能的数的结果一定互不相同
	int ans=0;
	for(long long i=2,last;i<=n;i=last+1){
		last=n/(n/i);
		ans=(ans+(last-i+1)%p*S(n/i))%p;//如果n是int范围的话记得强转
	}
	ans=(n%p*((n+1)%p)%p*inv_2-ans+p)%p;//同上
	return tbl[N/n]=ans;
}