#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;
const int maxn=1000005;
struct Tree{
	vector<int>G[maxn],W[maxn];
	int p[maxn],d[maxn],size[maxn],mn[maxn],mx[maxn];
	bool col[maxn];
	long long ans_sum;
	int ans_min,ans_max;
	void add(int x,int y,int z){
		G[x].push_back(y);
		W[x].push_back(z);
	}
	void dfs(int x){
		size[x]=col[x];
		mx[x]=(col[x]?d[x]:-0x3f3f3f3f);
		mn[x]=(col[x]?d[x]:0x3f3f3f3f);
		for(int i=0;i<(int)G[x].size();i++){
			d[G[x][i]]=d[x]+W[x][i];
			dfs(G[x][i]);
			ans_sum+=(long long)size[x]*size[G[x][i]]*d[x];
			ans_max=max(ans_max,mx[x]+mx[G[x][i]]-(d[x]<<1));
			ans_min=min(ans_min,mn[x]+mn[G[x][i]]-(d[x]<<1));
			size[x]+=size[G[x][i]];
			mx[x]=max(mx[x],mx[G[x][i]]);
			mn[x]=min(mn[x],mn[G[x][i]]);
		}
	}
	void clear(int x){
		G[x].clear();
		W[x].clear();
		col[x]=false;
	}
	void solve(int rt){
		ans_sum=0;
		ans_max=1<<31;
		ans_min=(~0u)>>1;
		dfs(rt);
		ans_sum<<=1;
	}
}virtree;
void dfs(int);
int LCA(int,int);
vector<int>G[maxn];
int f[maxn][20],d[maxn],dfn[maxn],tim=0;
bool cmp(int x,int y){return dfn[x]<dfn[y];}
int n,m,lgn=0,a[maxn],s[maxn],v[maxn];
int main(){
	scanf("%d",&n);
	for(int i=1,x,y;i<n;i++){
		scanf("%d%d",&x,&y);
		G[x].push_back(y);
		G[y].push_back(x);
	}
	G[n+1].push_back(1);
	dfs(n+1);
	for(int i=1;i<=n+1;i++)G[i].clear();
	lgn--;
	for(int j=1;j<=lgn;j++)for(int i=1;i<=n;i++)f[i][j]=f[f[i][j-1]][j-1];
	scanf("%d",&m);
	while(m--){
		int k;
		scanf("%d",&k);
		for(int i=1;i<=k;i++)scanf("%d",&a[i]);
		sort(a+1,a+k+1,cmp);
		int top=0,cnt=0;
		s[++top]=v[++cnt]=n+1;
		long long ans=0;
		for(int i=1;i<=k;i++){
			virtree.col[a[i]]=true;
			ans+=d[a[i]]-1;
			int u=LCA(a[i],s[top]);
			if(s[top]!=u){
				while(top>1&&d[s[top-1]]>=d[u]){
					virtree.add(s[top-1],s[top],d[s[top]]-d[s[top-1]]);
					top--;
				}
				if(s[top]!=u){
					virtree.add(u,s[top],d[s[top]]-d[u]);
					s[top]=v[++cnt]=u;
				}
			}
			s[++top]=a[i];
		}
		for(int i=top-1;i;i--)virtree.add(s[i],s[i+1],d[s[i+1]]-d[s[i]]);
		virtree.solve(n+1);
		ans*=k-1;
		printf("%lld %d %d\n",ans-virtree.ans_sum,virtree.ans_min,virtree.ans_max);
		for(int i=1;i<=k;i++)virtree.clear(a[i]);
		for(int i=1;i<=cnt;i++)virtree.clear(v[i]);
		
	}
	return 0;
}
void dfs(int x){
	dfn[x]=++tim;
	d[x]=d[f[x][0]]+1;
	while((1<<lgn)<d[x])lgn++;
	for(int i=0;i<(int)G[x].size();i++)if(G[x][i]!=f[x][0]){
		f[G[x][i]][0]=x;
		dfs(G[x][i]);
	}
}
int LCA(int x,int y){
	if(d[x]!=d[y]){
		if(d[x]<d[y])swap(x,y);
		for(int i=lgn;i>=0;i--)if(((d[x]-d[y])>>i)&1)x=f[x][i];
	}
	if(x==y)return x;
	for(int i=lgn;i>=0;i--)if(f[x][i]!=f[y][i]){
		x=f[x][i];
		y=f[y][i];
	}
	return f[x][0];
}