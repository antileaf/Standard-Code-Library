#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
const int maxn=200010,B=1213;
int d;
struct node{
	int x[2],l[2],r[2],w,sum;
	node *ch[2];
	bool operator<(const node &a)const{return x[d]<a.x[d];}
	void refresh(){
		sum=ch[0]->sum+ch[1]->sum+w;
		l[0]=min(x[0],min(ch[0]->l[0],ch[1]->l[0]));
		l[1]=min(x[1],min(ch[0]->l[1],ch[1]->l[1]));
		r[0]=max(x[0],max(ch[0]->r[0],ch[1]->r[0]));
		r[1]=max(x[1],max(ch[0]->r[1],ch[1]->r[1]));
	}
}null[maxn],*root=null;
void build(int,int,int,node*&);
void query(node*);
int l[2],r[2],x[B+10][2],w[B+10];
int n,op,ans=0,cnt=0,tmp=0;
int main(){
	freopen("bzoj_4066.in","r",stdin);
	freopen("bzoj_4066.out","w",stdout);
	null->l[0]=null->l[1]=10000000;
	null->r[0]=null->r[1]=-10000000;
	null->sum=0;
	null->ch[0]=null->ch[1]=null;
	scanf("%*d");
	while(scanf("%d",&op)==1&&op!=3){
		if(op==1){
			tmp++;
			scanf("%d%d%d",&x[tmp][0],&x[tmp][1],&w[tmp]);
			x[tmp][0]^=ans;x[tmp][1]^=ans;w[tmp]^=ans;
			if(tmp==B){
				for(int i=1;i<=tmp;i++){
					null[cnt+i].x[0]=x[i][0];
					null[cnt+i].x[1]=x[i][1];
					null[cnt+i].w=w[i];
				}
				build(1,cnt+=tmp,0,root);
				tmp=0;
			}
		}
		else{
			scanf("%d%d%d%d",&l[0],&l[1],&r[0],&r[1]);
			l[0]^=ans;l[1]^=ans;r[0]^=ans;r[1]^=ans;
			ans=0;
			for(int i=1;i<=tmp;i++)if(l[0]<=x[i][0]&&l[1]<=x[i][1]&&x[i][0]<=r[0]&&x[i][1]<=r[1])ans+=w[i];
			query(root);
			printf("%d\n",ans);
		}
	}
	return 0;
}
void build(int l,int r,int k,node *&rt){
	if(l>r){
		rt=null;
		return;
	}
	int mid=(l+r)>>1;
	d=k;
	nth_element(null+l,null+mid,null+r+1);
	rt=null+mid;
	build(l,mid-1,k^1,rt->ch[0]);
	build(mid+1,r,k^1,rt->ch[1]);
	rt->refresh();
}
void query(node *rt){
	if(l[0]<=rt->l[0]&&l[1]<=rt->l[1]&&rt->r[0]<=r[0]&&rt->r[1]<=r[1]){
		ans+=rt->sum;
		return;
	}
	else if(l[0]>rt->r[0]||l[1]>rt->r[1]||r[0]<rt->l[0]||r[1]<rt->l[1])return;
	if(l[0]<=rt->x[0]&&l[1]<=rt->x[1]&&rt->x[0]<=r[0]&&rt->x[1]<=r[1])ans+=rt->w;
	query(rt->ch[0]);
	query(rt->ch[1]);
}