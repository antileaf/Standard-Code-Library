//Quick Multiplication O(1)快速乘
//By ysf
//在两数直接相乘会爆long long时才有必要使用
//常数比直接long long乘法+取模大很多，非必要时不建议使用

long long mul(long long a,long long b,long long p){
	a%=p;b%=p;
	return ((a*b-p*(long long)((long double)a/p*b+0.5))%p+p)%p;
}
