#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;
const int maxn=100010;
const double alpha=0.7;
struct node{
	static int randint(){
		static int a=1213,b=97818217,p=998244353,x=751815431;
		x=a*x+b;x%=p;
		return x<0?(x+=p):x;
	}
	int data,size,p;
	node *ch[2];
	node(int d):data(d),size(1),p(randint()){}
	inline void refresh(){size=ch[0]->size+ch[1]->size+1;}
}*null=new node(0),*root[maxn],*root1[maxn][50];
void addnode(int,int);
void rebuild(int,int,int,int);
void dfs_getcenter(int,int,int&);
void dfs_getdis(int,int,int,int);
void dfs_destroy(int,int);
void insert(int,node*&);
int order(int,node*);
void destroy(node*&);
void rot(node*&,int);
vector<int>G[maxn],W[maxn];
int size[maxn]={0},siz[maxn][50]={0},son[maxn];
bool vis[maxn];
int depth[maxn],p[maxn],d[maxn][50],id[maxn][50];
int n,m,w[maxn],tmp;
long long ans=0;
int main(){
	freopen("flowera.in","r",stdin);
	freopen("flowera.out","w",stdout);
	null->size=0;
	null->ch[0]=null->ch[1]=null;
	scanf("%*d%d",&n);
	fill(vis,vis+n+1,true);
	fill(root,root+n+1,null);
	for(int i=0;i<=n;i++)fill(root1[i],root1[i]+50,null);
	scanf("%*d%*d%d",&w[1]);
	insert(-w[1],root[1]);
	size[1]=1;
	printf("0\n");
	for(int i=2;i<=n;i++){
		scanf("%d%d%d",&p[i],&tmp,&w[i]);
		p[i]^=(ans%(int)1e9);
		G[i].push_back(p[i]);
		W[i].push_back(tmp);
		G[p[i]].push_back(i);
		W[p[i]].push_back(tmp);
		addnode(i,tmp);
		printf("%lld\n",ans);
	}
	return 0;
}
void addnode(int x,int z){//wj-dj>=di-wi
	depth[x]=depth[p[x]]+1;
	size[x]=1;
	insert(-w[x],root[x]);
	int rt=0;
	for(int u=p[x],k=depth[p[x]];u;u=p[u],k--){
		if(u==p[x]){
			id[x][k]=x;
			d[x][k]=z;
		}
		else{
			id[x][k]=id[p[x]][k];
			d[x][k]=d[p[x]][k]+z;
		}
		ans+=order(w[x]-d[x][k],root[u])-order(w[x]-d[x][k],root1[id[x][k]][k]);
		insert(d[x][k]-w[x],root[u]);
		insert(d[x][k]-w[x],root1[id[x][k]][k]);
		size[u]++;
		siz[id[x][k]][k]++;
		if(siz[id[x][k]][k]>size[u]*alpha+5)rt=u;
	}
	id[x][depth[x]]=0;
	d[x][depth[x]]=0;
	if(rt){
		dfs_destroy(rt,depth[rt]);
		rebuild(rt,depth[rt],size[rt],p[rt]);
	}
}
void rebuild(int x,int k,int s,int pr){
	int u=0;
	dfs_getcenter(x,s,u);
	vis[x=u]=true;
	p[x]=pr;
	depth[x]=k;
	size[x]=s;
	d[x][k]=id[x][k]=0;
	destroy(root[x]);
	insert(-w[x],root[x]);
	if(s<=1)return;
	for(int i=0;i<(int)G[x].size();i++)if(!vis[G[x][i]]){
		p[G[x][i]]=0;
		d[G[x][i]][k]=W[x][i];
		siz[G[x][i]][k]=p[G[x][i]]=0;
		destroy(root1[G[x][i]][k]);
		dfs_getdis(G[x][i],x,G[x][i],k);
	}
	for(int i=0;i<(int)G[x].size();i++)if(!vis[G[x][i]])rebuild(G[x][i],k+1,size[G[x][i]],x);
}
void dfs_getcenter(int x,int s,int &u){
	size[x]=1;
	son[x]=0;
	for(int i=0;i<(int)G[x].size();i++)if(!vis[G[x][i]]&&G[x][i]!=p[x]){
		p[G[x][i]]=x;
		dfs_getcenter(G[x][i],s,u);
		size[x]+=size[G[x][i]];
		if(size[G[x][i]]>size[son[x]])son[x]=G[x][i];
	}
	if(!u||max(s-size[x],size[son[x]])<max(s-size[u],size[son[u]]))u=x;
}
void dfs_getdis(int x,int u,int rt,int k){
	insert(d[x][k]-w[x],root[u]);
	insert(d[x][k]-w[x],root1[rt][k]);
	id[x][k]=rt;
	siz[rt][k]++;
	size[x]=1;
	for(int i=0;i<(int)G[x].size();i++)if(!vis[G[x][i]]&&G[x][i]!=p[x]){
		p[G[x][i]]=x;
		d[G[x][i]][k]=d[x][k]+W[x][i];
		dfs_getdis(G[x][i],u,rt,k);
		size[x]+=size[G[x][i]];
	}
}
void dfs_destroy(int x,int k){
	vis[x]=false;
	for(int i=0;i<(int)G[x].size();i++)if(depth[G[x][i]]>=k&&G[x][i]!=p[x]){
		p[G[x][i]]=x;
		dfs_destroy(G[x][i],k);
	}
}
void insert(int x,node *&rt){
	if(rt==null){
		rt=new node(x);
		rt->ch[0]=rt->ch[1]=null;
		return;
	}
	int d=x>=rt->data;
	insert(x,rt->ch[d]);
	rt->refresh();
	if(rt->ch[d]->p<rt->p)rot(rt,d^1);
}
int order(int x,node *rt){
	int ans=0,d;
	x++;
	while(rt!=null){
		if((d=x>rt->data))ans+=rt->ch[0]->size+1;
		rt=rt->ch[d];
	}
	return ans;
}
void destroy(node *&x){
	if(x==null)return;
	destroy(x->ch[0]);
	destroy(x->ch[1]);
	delete x;
	x=null;
}
void rot(node *&x,int d){
	node *y=x->ch[d^1];
	x->ch[d^1]=y->ch[d];
	y->ch[d]=x;
	x->refresh();
	(x=y)->refresh();
}