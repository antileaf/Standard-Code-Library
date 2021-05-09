// Quick Multiplication O(1)快速乘
// By ysf

// long double 快速乘
// 在两数直接相乘会爆long long时才有必要使用
// 常数比直接long long乘法+取模大很多，非必要时不建议使用
long long mul(long long a,long long b,long long p){
	a%=p;b%=p;
	return ((a*b-p*(long long)((long double)a/p*b+0.5))%p+p)%p;
}

// 指令集快速乘
// 试机记得测试能不能过编译
inline long long mul(const long long a, const long long b, const long long p) {
	long long ans;
	__asm__ __volatile__ ("\tmulq %%rbx\n\tdivq %%rcx\n" :  "=d"(ans) : "a"(a), "b"(b), "c"(p));
	return ans;
}