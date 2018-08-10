//Manacher O(n)
//By ysf
//通过题目：51Nod1089 最长回文子串V2

//n为串长，字符串存在s中，回文半径输出到p数组中
//字符串会被修改，记得备份，以及数组要开串长的两倍
void manacher(int n){
	for(int i=n;i;i--)s[i<<1]=s[i];
	for(int i=0;i<=n;i++)s[i<<1|1]='#';
	s[0]='$';
	s[(n+1)<<1]='\0';
	n=n<<1|1;
	int mx=0,j=0;
	for(int i=1;i<=n;i++){
		p[i]=mx>i?min(p[(j<<1)-i],mx-i):1;
		while(s[i-p[i]]==s[i+p[i]])p[i]++;
		if(i+p[i]>mx){
			mx=i+p[i];
			j=i;
		}
	}
}