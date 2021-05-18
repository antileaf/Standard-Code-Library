#include<bits/stdc++.h>
using namespace std;
const int maxn=100005;
void expand(int);
void dfs(int);
int root,last,cnt=0,val[maxn<<1]={0},par[maxn<<1]={0},go[maxn<<1][26]={{0}};
bool vis[maxn<<1]={0};
char s[maxn];
int n,id[maxn<<1]={0},ch[maxn<<1][26]={{0}},height[maxn],tim=0;
int main(){
    root=last=++cnt;
    scanf("%s",s+1);
    n=strlen(s+1);
    for(int i=n;i;i--){
        expand(s[i]-'a');
        id[last]=i;
    }
    vis[1]=true;
    for(int i=1;i<=cnt;i++)if(id[i])for(int x=i,pos=n;x&&!vis[x];x=par[x]){
        vis[x]=true;
        pos-=val[x]-val[par[x]];
        ch[par[x]][s[pos+1]-'a']=x;
    }
    dfs(root);
    printf("\n");
    for(int i=1;i<n;i++)printf("%d ",height[i]);
    return 0;
}
void expand(int c){
    int p=last,np=++cnt;
    val[np]=val[p]+1;
    while(p&&!go[p][c]){
        go[p][c]=np;
        p=par[p];
    }
    if(!p)par[np]=root;
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
void dfs(int x){
    if(id[x]){
        printf("%d ",id[x]);
        height[tim++]=val[last];
        last=x;
    }
    for(int c=0;c<26;c++)if(ch[x][c])dfs(ch[x][c]);
    last=par[x];
}