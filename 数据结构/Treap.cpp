//Treap Minimum Heap Version 小根堆版本
//By ysf
//通过题目：普通平衡树

//注意：相同键值可以共存

struct node{//结点类定义
	int key,size,p;//分别为键值、子树大小、优先度
	node *ch[2];//0表示左儿子，1表示右儿子
	node(int key=0):key(key),size(1),p(rand()){}
	void refresh(){size=ch[0]->size+ch[1]->size+1;}//更新子树大小（和附加信息）
}null[maxn],*root=null,*ptr=null;//数组名叫做null是为了方便开哨兵节点
//如果需要删除而空间不能直接开下所有结点，则需要再写一个垃圾回收
//注意：数组里的元素一定不能delete，否则会导致RE

//重要！！！
//在主函数最开始一定要加上以下预处理：
null->ch[0]=null->ch[1]=null;
null->size=0;

//伪构造函数 O(1)
//为了方便，在结点类外面再定义一个伪构造函数
node *newnode(int x){//键值为x
	*++ptr=node(x);
	ptr->ch[0]=ptr->ch[1]=null;
	return ptr;
}

//插入键值 期望O(\log n)
//需要调用旋转
void insert(int x,node *&rt){//rt为当前结点，建议调用时传入root，下同
	if(rt==null){
		rt=newnode(x);
		return;
	}
	int d=x>rt->key;
	insert(x,rt->ch[d]);
	rt->refresh();
	if(rt->ch[d]->p<rt->p)rot(rt,d^1);
}

//删除一个键值 期望O(\log n)
//要求键值必须存在至少一个，否则会导致RE
//需要调用旋转
void erase(int x,node *&rt){
	if(x==rt->key){
		if(rt->ch[0]!=null&&rt->ch[1]!=null){
			int d=rt->ch[0]->p<rt->ch[1]->p;
			rot(rt,d);
			erase(x,rt->ch[d]);
		}
		else rt=rt->ch[rt->ch[0]==null];
	}
	else erase(x,rt->ch[x>rt->key]);
	if(rt!=null)rt->refresh();
}

//求元素的排名（严格小于键值的个数+1） 期望O(\log n)
//非递归
int rank(int x,node *rt){
	int ans=1,d;
	while(rt!=null){
		if((d=x>rt->key))ans+=rt->ch[0]->size+1;
		rt=rt->ch[d];
	}
	return ans;
}

//返回排名第k（从1开始）的键值对应的指针 期望O(\log n)
//非递归
node *kth(int x,node *rt){
	int d;
	while(rt!=null){
		if(x==rt->ch[0]->size+1)return rt;
		if((d=x>rt->ch[0]->size))x-=rt->ch[0]->size+1;
		rt=rt->ch[d];
	}
	return rt;
}

//返回前驱（最大的比给定键值小的键值）对应的指针 期望O(\log n)
//非递归
node *pred(int x,node *rt){
	node *y=null;
	int d;
	while(rt!=null){
		if((d=x>rt->key))y=rt;
		rt=rt->ch[d];
	}
	return y;
}

//返回后继（最小的比给定键值大的键值）对应的指针 期望O(\log n)
//非递归
node *succ(int x,node *rt){
	node *y=null;
	int d;
	while(rt!=null){
		if((d=x<rt->key))y=rt;
		rt=rt->ch[d^1];
	}
	return y;
}

//旋转（平衡树基础操作） O(1)
void rot(node *&x,int d){//x为被转下去的结点，会被修改以维护树结构
	node *y=x->ch[d^1];
	x->ch[d^1]=y->ch[d];
	y->ch[d]=x;
	x->refresh();
	(x=y)->refresh();
}