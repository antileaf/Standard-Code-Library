//Dynamic Divide and Couquer on Tree 动态树分治 O(n\log n)-O(\log n)
//By ysf
//通过题目：COGS2278 树黑白

//为了减小常数，这里采用bfs写法（实测预处理比dfs快将近一半
//以下以维护一个点到每个黑点的距离之和为例

//全局数组定义
vector<int>G[maxn],W[maxn];
int size[maxn],son[maxn],q[maxn];
int p[maxn],depth[maxn],id[maxn][20],d[maxn][20];//id是对应层所在子树的根
int a[maxn],ca[maxn],b[maxn][20],cb[maxn][20];//维护距离和用的
bool vis[maxn]={false},col[maxn]={false};

//建树 总计O(n\log n)
//需要调用找重心、预处理距离，同时递归调用自身
void build(int x,int k,int s,int pr){//结点，深度，连通块大小，点分树上的父亲
	x=getcenter(x,s);
	vis[x]=true;
	depth[x]=k;
	p[x]=pr;
	for(int i=0;i<(int)G[x].size();i++)
		if(!vis[G[x][i]]){
			d[G[x][i]][k]=W[x][i];
			p[G[x][i]]=x;
			getdis(G[x][i],k,G[x][i]);
		}
	for(int i=0;i<(int)G[x].size();i++)
		if(!vis[G[x][i]])build(G[x][i],k+1,size[G[x][i]],x);
}

//找重心 O(n)
int getcenter(int x,int s){
	int head=0,tail=0;
	q[tail++]=x;
	while(head!=tail){
		x=q[head++];
		size[x]=1;
		son[x]=0;
		for(int i=0;i<(int)G[x].size();i++)
			if(!vis[G[x][i]]&&G[x][i]!=p[x]){
				p[G[x][i]]=x;
				q[tail++]=G[x][i];
			}
	}
	for(int i=tail-1;i;i--){
		x=q[i];
		size[p[x]]+=size[x];
		if(size[x]>size[son[p[x]]])son[p[x]]=x;
	}
	x=q[0];
	while(son[x]&&(size[son[x]]<<1)>=s)x=son[x];
	return x;
}

//预处理距离 O(n)
//方便起见，这里直接用了笨一点的方法，O(n\log n)全存下来
void getdis(int x,int k,int rt){
	int head=0,tail=0;
	q[tail++]=x;
	while(head!=tail){
		x=q[head++];
		size[x]=1;
		id[x][k]=rt;
		for(int i=0;i<(int)G[x].size();i++)
			if(!vis[G[x][i]]&&G[x][i]!=p[x]){
				p[G[x][i]]=x;
				d[G[x][i]][k]=d[x][k]+W[x][i];
				q[tail++]=G[x][i];
			}
	}
	for(int i=tail-1;i;i--)
		size[p[q[i]]]+=size[q[i]];
}

//修改 O(\log n)
void modify(int x){
	if(col[x])ca[x]--;
	else ca[x]++;//记得先特判自己作为重心的那层
	for(int u=p[x],k=depth[x]-1;u;u=p[u],k--){
		if(col[x]){
			a[u]-=d[x][k];
			ca[u]--;
			b[id[x][k]][k]-=d[x][k];
			cb[id[x][k]][k]--;
		}
		else{
			a[u]+=d[x][k];
			ca[u]++;
			b[id[x][k]][k]+=d[x][k];
			cb[id[x][k]][k]++;
		}
	}
	col[x]^=true;
}

//询问 O(\log n)
int query(int x){
	int ans=a[x];//特判自己是重心的那层
	for(int u=p[x],k=depth[x]-1;u;u=p[u],k--)
		ans+=a[u]-b[id[x][k]][k]+d[x][k]*(ca[u]-cb[id[x][k]][k]);
	return ans;
}