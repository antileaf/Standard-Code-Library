//Suffix Automaton 后缀自动机 O(n)
//By ysf
//通过题目：Bzoj3473 字符串

//在字符集比较小的时候可以直接开go数组，否则需要用map或者哈希表替换
//注意！！！结点数要开成串长的两倍

//全局变量与数组定义
int last,val[maxn],par[maxn],go[maxn][26],cnt;
int c[maxn],q[maxn];//用来桶排序

//在主函数开头加上这句初始化
last=cnt=1;

//以下是按val进行桶排序的代码
for(int i=1;i<=cnt;i++)c[val[i]+1]++;
for(int i=1;i<=n;i++)c[i]+=c[i-1];//这里n是串长
for(int i=1;i<=cnt;i++)q[++c[val[i]]]=i;

//加入一个字符 均摊O(1)
void extend(int c){
	int p=last,np=++cnt;
	val[np]=val[p]+1;
	while(p&&!go[p][c]){
		go[p][c]=np;
		p=par[p];
	}
	if(!p)par[np]=1;
	else{
		int q=go[p][c];
		if(val[q]==val[p]+1)par[np]=q;
		else{
			int nq=++cnt;
			val[nq]=val[p]+1;
			memcpy(go[nq],go[q],sizeof(go[q]));
			par[nq]=par[q];
			par[np]=par[q]=nq;
			while(p&&go[p][c]==q){
				go[p][c]=nq;
				p=par[p];
			}
		}
	}
	last=np;
}