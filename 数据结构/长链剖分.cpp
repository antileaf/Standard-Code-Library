//Long-chain Subdivision 长链剖分 O(n)
//By ysf
//通过题目：vijos lxhgww的奇思妙想（板子题）

//顾名思义，长链剖分是取最深的儿子作为重儿子
//长链剖分的两个应用：
//O(1)在线求一个点的第k祖先
//O(n)维护以深度为下标的子树信息

//------------------------------分割线------------------------------

//在线求一个点的第k祖先 O(n\log n)-O(1)
//其中O(n\log n)预处理是因为需要用到倍增
//理论基础：任意一个点x的k级祖先y所在长链长度一定>=k

//全局数组定义
vector<int>G[maxn],v[maxn];
int d[maxn],mxd[maxn],son[maxn],top[maxn],len[maxn];
int f[maxn][19],log_tbl[maxn];

//在主函数中两遍dfs之后加上如下预处理
log_tbl[0]=-1;
for(int i=1;i<=n;i++)log_tbl[i]=log_tbl[i>>1]+1;
for(int j=1;(1<<j)<n;j++)
	for(int i=1;i<=n;i++)
		f[i][j]=f[f[i][j-1]][j-1];

//第一遍dfs，用于计算深度和找出重儿子
//递归调用自身
void dfs1(int x){
	mxd[x]=d[x];
	for(int i=0;i<(int)G[x].size();i++)
		if(G[x][i]!=f[x][0]){
			f[G[x][i]][0]=x;
			d[G[x][i]]=d[x]+1;
			dfs1(G[x][i]);
			mxd[x]=max(mxd[x],mxd[G[x][i]]);
			if(mxd[G[x][i]]>mxd[son[x]])son[x]=G[x][i];
		}
}

//第二遍dfs，用于进行剖分和预处理梯子剖分（每条链向上延伸一倍）数组
//递归调用自身
void dfs2(int x){
	top[x]=(x==son[f[x][0]]?top[f[x][0]]:x);
	for(int i=0;i<(int)G[x].size();i++)
		if(G[x][i]!=f[x][0])dfs2(G[x][i]);
	if(top[x]==x){
		int u=x;
		while(top[son[u]]==x)u=son[u];
		len[x]=d[u]-d[x];
		for(int i=0;i<len[x];i++,u=f[u][0])v[x].push_back(u);
		u=x;
		for(int i=0;i<len[x]&&u;i++,u=f[u][0])v[x].push_back(u);
	}
}

//在线询问x的k级祖先 O(1)
//不存在时返回0
int query(int x,int k){
	if(!k)return x;
	if(k>d[x])return 0;
	x=f[x][log_tbl[k]];
	k^=1<<log_tbl[k];
	return v[top[x]][d[top[x]]+len[top[x]]-d[x]+k];
}

//------------------------------分割线------------------------------

//O(n)维护以深度为下标的子树信息