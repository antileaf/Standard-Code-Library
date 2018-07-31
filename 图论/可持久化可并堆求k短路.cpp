//Kth Shortest Path via Persistable Mergeable Heap
//可持久化可并堆求k短路 O(SSSP+(m+k)\log n)
//By ysf
//通过题目：USACO Mar08 牛跑步（板子题）

//注意这是个多项式算法，在k比较大时很有优势，但k比较小时最好还是用A*
//DAG和有环的情况都可以，有重边或自环也无所谓，但不能有零环
//以下代码以Dijkstra+可持久化左偏树为例

const int maxn=1005,maxe=10005,maxm=maxe*30;//点数，边数，左偏树结点数

//需要用到的结构体定义
struct A{//用来求最短路
	int x,d;
	A(int x,int d):x(x),d(d){}
	bool operator<(const A &a)const{return d>a.d;}
};

struct node{//左偏树结点
	int w,i,d;//i：最后一条边的编号 d：左偏树附加信息
	node *lc,*rc;
	node(){}
	node(int w,int i):w(w),i(i),d(0){}
	void refresh(){d=rc->d+1;}
}null[maxm],*ptr=null,*root[maxn];

struct B{//维护答案用
	int x,w;//x是结点编号，w表示之前已经产生的权值
	node *rt;//这个答案对应的堆顶，注意可能不等于任何一个结点的堆
	B(int x,node *rt,int w):x(x),w(w),rt(rt){}
	bool operator<(const B &a)const{return w+rt->w>a.w+a.rt->w;}
};

//全局变量和数组定义
vector<int>G[maxn],W[maxn],id[maxn];//最开始要存反向图，然后把G清空作为儿子列表
bool vis[maxn],used[maxe];//used表示边是否在最短路树上
int u[maxe],v[maxe],w[maxe];//存下每条边，注意是有向边
int d[maxn],p[maxn];//p表示最短路树上每个点的父边
int n,m,k,s,t;//s,t分别表示起点和终点

//以下是主函数中较关键的部分
for(int i=0;i<=n;i++)root[i]=null;//一定要加上！！！
//(读入&建反向图)
Dijkstra();
//(清空G,W,id)
for(int i=1;i<=n;i++)
	if(p[i]){
		used[p[i]]=true;//在最短路树上
		G[v[p[i]]].push_back(i);
	}
for(int i=1;i<=m;i++){
	w[i]-=d[u[i]]-d[v[i]];//现在的w[i]表示这条边能使路径长度增加多少
	if(!used[i])
		root[u[i]]=merge(root[u[i]],newnode(w[i],i));
}
dfs(t);
priority_queue<B>heap;
heap.push(B(s,root[s],0));//初始状态是找贡献最小的边加进去
printf("%d\n",d[s]);//第1短路需要特判
while(--k){//其余k-1短路径用二叉堆维护
	if(heap.empty())printf("-1\n");
	else{
		int x=heap.top().x,w=heap.top().w;
		node *rt=heap.top().rt;
		heap.pop();
		printf("%d\n",d[s]+w+rt->w);
		if(rt->lc!=null||rt->rc!=null)
			heap.push(B(x,merge(rt->lc,rt->rc),w));//pop掉当前边，换成另一条贡献大一点的边
		if(root[v[rt->i]]!=null)
			heap.push(B(v[rt->i],root[v[rt->i]],w+rt->w));//保留当前边，往后面再接上另一条边
	}
}
//主函数到此结束

//Dijkstra预处理最短路 O(m\log n)
void Dijkstra(){
	memset(d,63,sizeof(d));
	d[t]=0;
	priority_queue<A>heap;
	heap.push(A(t,0));
	while(!heap.empty()){
		int x=heap.top().x;
		heap.pop();
		if(vis[x])continue;
		vis[x]=true;
		for(int i=0;i<(int)G[x].size();i++)
			if(!vis[G[x][i]]&&d[G[x][i]]>d[x]+W[x][i]){
				d[G[x][i]]=d[x]+W[x][i];
				p[G[x][i]]=id[x][i];
				heap.push(A(G[x][i],d[G[x][i]]));
			}
	}
}

//dfs求出每个点的堆 总计O(m\log n)
//需要调用merge，同时递归调用自身
void dfs(int x){
	root[x]=merge(root[x],root[v[p[x]]]);
	for(int i=0;i<(int)G[x].size();i++)
		dfs(G[x][i]);
}

//包装过的new node() O(1)
node *newnode(int w,int i){
	*++ptr=node(w,i);
	ptr->lc=ptr->rc=null;
	return ptr;
}

//带可持久化的左偏树合并 总计O(\log n)
//递归调用自身
node *merge(node *x,node *y){
	if(x==null)return y;
	if(y==null)return x;
	if(x->w>y->w)swap(x,y);
	node *z=newnode(x->w,x->i);
	z->lc=x->lc;
	z->rc=merge(x->rc,y);
	if(z->lc->d>z->rc->d)swap(z->lc,z->rc);
	z->refresh();
	return z;
}