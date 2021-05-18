/**************************************************************
    Problem: 3514
    User: hzoier
    Language: C++
    Result: Accepted
    Time:33584 ms
    Memory:93752 kb
****************************************************************/
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<map>
#define isroot(x) ((x)->p==null||((x)->p->ch[0]!=(x)&&(x)->p->ch[1]!=(x)))
#define dir(x) ((x)==(x)->p->ch[1])
using namespace std;
const int maxn=200010;
struct node{
    int key,mn,pos;
    bool rev;
    node *ch[2],*p;
    node(int key=(~0u)>>1):key(key),mn(key),pos(-1),rev(false){}
    inline void pushdown(){
        if(!rev)return;
        ch[0]->rev^=true;
        ch[1]->rev^=true;
        swap(ch[0],ch[1]);
        if(pos!=-1)pos^=1;
        rev=false;
    }
    inline void refresh(){
        mn=key;
        pos=-1;
        if(ch[0]->mn<mn){
            mn=ch[0]->mn;
            pos=0;
        }
        if(ch[1]->mn<mn){
            mn=ch[1]->mn;
            pos=1;
        }
    }
}null[maxn<<1],*ptr=null;
node *newnode(int);
node *access(node*);
void makeroot(node*);
void link(node*,node*);
void cut(node*,node*);
node *getroot(node*);
node *getmin(node*,node*);
void splay(node*);
void rot(node*,int);
void build(int,int,int&,int);
void query(int,int,int,int);
int sm[maxn<<5]={0},lc[maxn<<5]={0},rc[maxn<<5]={0},root[maxn]={0},cnt=0;
map<node*,pair<node*,node*> >mp;
node *tmp;
int n,m,q,tp,x,y,k,l,r,t,ans=0;
int main(){
    null->ch[0]=null->ch[1]=null->p=null;
    scanf("%d%d%d%d",&n,&m,&q,&tp);
    for(int i=1;i<=n;i++)newnode((~0u)>>1);
    for(int i=1;i<=m;i++){
        scanf("%d%d",&x,&y);
        if(x==y){
            root[i]=root[i-1];
            continue;
        }
        if(getroot(null+x)!=getroot(null+y)){
            tmp=newnode(i);
            k=0;
        }
        else{
            tmp=getmin(null+x,null+y);
            cut(tmp,mp[tmp].first);
            cut(tmp,mp[tmp].second);
            k=tmp->key;
            tmp->key=i;
            tmp->refresh();
        }
        link(tmp,null+x);
        link(tmp,null+y);
        mp[tmp]=make_pair(null+x,null+y);
        build(0,m-1,root[i],root[i-1]);
    }
    while(q--){
        scanf("%d%d",&l,&r);
        if(tp){
            l^=ans;
            r^=ans;
        }
        ans=n;
        t=--l;
        query(0,m-1,root[r],root[l]);
        printf("%d\n",ans);
    }
    return 0;
}
node *newnode(int x){
    *++ptr=node(x);
    ptr->ch[0]=ptr->ch[1]=ptr->p=null;
    return ptr;
}
node *access(node *x){
    node *y=null;
    while(x!=null){
        splay(x);
        x->ch[1]=y;
        (y=x)->refresh();
        x=x->p;
    }
    return y;
}
void makeroot(node *x){
    access(x);
    splay(x);
    x->rev^=true;
}
void link(node *x,node *y){
    makeroot(x);
    x->p=y;
}
void cut(node *x,node *y){
    makeroot(x);
    access(y);
    splay(y);
    y->ch[0]->p=null;
    y->ch[0]=null;
    y->refresh();
}
node *getroot(node *x){
    x=access(x);
    while(x->pushdown(),x->ch[0]!=null)x=x->ch[0];
    splay(x);
    return x;
}
node *getmin(node *x,node *y){
    makeroot(x);
    x=access(y);
    while(x->pushdown(),x->pos!=-1)x=x->ch[x->pos];
    splay(x);
    return x;
}
void splay(node *x){
    x->pushdown();
    while(!isroot(x)){
        if(!isroot(x->p))x->p->p->pushdown();
        x->p->pushdown();
        x->pushdown();
        if(isroot(x->p)){
            rot(x->p,dir(x)^1);
            break;
        }
        if(dir(x)==dir(x->p))rot(x->p->p,dir(x->p)^1);
        else rot(x->p,dir(x)^1);
        rot(x->p,dir(x)^1);
    }
}
void rot(node *x,int d){
    node *y=x->ch[d^1];
    if((x->ch[d^1]=y->ch[d])!=null)y->ch[d]->p=x;
    y->p=x->p;
    if(!isroot(x))x->p->ch[dir(x)]=y;
    (y->ch[d]=x)->p=y;
    x->refresh();
    y->refresh();
}
void build(int l,int r,int &rt,int pr){
    sm[rt=++cnt]=sm[pr]+1;
    if(l==r)return;
    lc[rt]=lc[pr];
    rc[rt]=rc[pr];
    int mid=(l+r)>>1;
    if(k<=mid)build(l,mid,lc[rt],lc[pr]);
    else build(mid+1,r,rc[rt],rc[pr]);
}
void query(int l,int r,int rt,int pr){
    if(!rt&&!pr)return;
    if(t>=r){
        ans-=sm[rt]-sm[pr];
        return;
    }
    int mid=(l+r)>>1;
    query(l,mid,lc[rt],lc[pr]);
    if(t>mid)query(mid+1,r,rc[rt],rc[pr]);
}
/*
如果是离线的话，我们可以LCT+莫队什么的乱搞是吧，但是在线就……
不过还是有一个很喵的做法——
我们用LCT维护一棵生成树，当加入一条边i的时候(i是其编号)，其连接的两个点可能已经联通，加入i之后会形成一个环，我们弹掉这个环上编号最小的边(也就是加入最早的边)，并记录其编号为ntr_i。特殊的，如果i没有弹掉任何边，我们记ntr_i=0。
对于一个询问[L,R](表示我们只保留e|e∈[L,R])，答案就是$n?\sum_{i=L}^R(ntr_i<L)$。这个就是主席树了。
——YouSiki

这做法简直是妙啊……
*/