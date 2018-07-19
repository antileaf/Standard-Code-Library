long long mul(long long a,long long b,long long p){
	a%=p;
	b%=p;
	return ((a*b-p*(long long)((long double)a/p*b))%p+p)%p;
}
