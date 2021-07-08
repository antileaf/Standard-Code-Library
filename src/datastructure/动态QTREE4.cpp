#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/priority_queue.hpp>

#define isroot(x) ((x)->p==null||((x)!=(x)->p->ch[0]&&(x)!=(x)->p->ch[1]))
#define dir(x) ((x)==(x)->p->ch[1])

using namespace std;
using namespace __gnu_pbds;

const int maxn = 100010;
const long long INF = 1000000000000000000ll;

struct binary_heap {
    __gnu_pbds::priority_queue<long long, less<long long>, binary_heap_tag>q1, q2;
    binary_heap() {}

    void push(long long x) {
        if (x > (-INF) >> 2)
            q1.push(x);
    }

    void erase(long long x) {
        if (x > (-INF) >> 2)
            q2.push(x);
    }

    long long top() {
        if (empty())
            return -INF;

        while (!q2.empty() && q1.top() == q2.top()) {
            q1.pop();
            q2.pop();
        }

        return q1.top();
    }

    long long top2() {
        if (size() < 2)
            return -INF;

        long long a = top();
        erase(a);
        long long b = top();
        push(a);
        return a + b;
    }

    int size() {
        return q1.size() - q2.size();
    }

    bool empty() {
        return q1.size() == q2.size();
    }
} heap; // 全局堆维护每条链的最大子段和

struct node {
    long long sum, maxsum, prefix, suffix;
    int key;
    binary_heap heap; // 每个点的堆存的是它的子树中到它的最远距离, 如果它是黑点的话还会包括自己
    node *ch[2], *p;
    bool rev;
    node(int k = 0): sum(k), maxsum(-INF), prefix(-INF),
        suffix(-INF), key(k), rev(false) {}
    inline void pushdown() {
        if (!rev)
            return;

        ch[0]->rev ^= true;
        ch[1]->rev ^= true;
        swap(ch[0], ch[1]);
        swap(prefix, suffix);
        rev = false;
    }
    inline void refresh() {
        pushdown();
        ch[0]->pushdown();
        ch[1]->pushdown();
        sum = ch[0]->sum + ch[1]->sum + key;
        prefix = max(ch[0]->prefix,
                     ch[0]->sum + key + ch[1]->prefix);
        suffix = max(ch[1]->suffix,
                     ch[1]->sum + key + ch[0]->suffix);
        maxsum = max(max(ch[0]->maxsum, ch[1]->maxsum),
                     ch[0]->suffix + key + ch[1]->prefix);

        if (!heap.empty()) {
            prefix = max(prefix,
                         ch[0]->sum + key + heap.top());
            suffix = max(suffix,
                         ch[1]->sum + key + heap.top());
            maxsum = max(maxsum, max(ch[0]->suffix,
                                     ch[1]->prefix) + key + heap.top());

            if (heap.size() > 1) {
                maxsum = max(maxsum, heap.top2() + key);
            }
        }
    }
} null[maxn << 1], *ptr = null;

void addedge(int, int, int);
void deledge(int, int);
void modify(int, int, int);
void modify_color(int);
node *newnode(int);
node *access(node *);
void makeroot(node *);
void link(node *, node *);
void cut(node *, node *);
void splay(node *);
void rot(node *, int);

queue<node *>freenodes;
tree<pair<int, int>, node *>mp;

bool col[maxn] = {false};
char c;
int n, m, k, x, y, z;

int main() {
    null->ch[0] = null->ch[1] = null->p = null;
    scanf("%d%d%d", &n, &m, &k);

    for (int i = 1; i <= n; i++)
        newnode(0);

    heap.push(0);

    while (k--) {
        scanf("%d", &x);

        col[x] = true;
        null[x].heap.push(0);
    }

    for (int i = 1; i < n; i++) {
        scanf("%d%d%d", &x, &y, &z);

        if (x > y)
            swap(x, y);
        addedge(x, y, z);
    }

    while (m--) {
        scanf(" %c%d", &c, &x);

        if (c == 'A') {
            scanf("%d", &y);

            if (x > y)
                swap(x, y);
            deledge(x, y);
        }
        else if (c == 'B') {
            scanf("%d%d", &y, &z);

            if (x > y)
                swap(x, y);
            addedge(x, y, z);
        }
        else if (c == 'C') {
            scanf("%d%d", &y, &z);

            if (x > y)
                swap(x, y);
            modify(x, y, z);
        }
        else
            modify_color(x);

        printf("%lld\n", (heap.top() > 0 ? heap.top() : -1));
    }

    return 0;
}
void addedge(int x, int y, int z) {
    node *tmp;
    if (freenodes.empty())
        tmp = newnode(z);
    else {
        tmp = freenodes.front();
        freenodes.pop();
        *tmp = node(z);
    }

    tmp->ch[0] = tmp->ch[1] = tmp->p = null;

    heap.push(tmp->maxsum);
    link(tmp, null + x);
    link(tmp, null + y);
    mp[make_pair(x, y)] = tmp;
}

void deledge(int x, int y) {
    node *tmp = mp[make_pair(x, y)];

    cut(tmp, null + x);
    cut(tmp, null + y);

    freenodes.push(tmp);
    heap.erase(tmp->maxsum);
    mp.erase(make_pair(x, y));
}

void modify(int x, int y, int z) {
    node *tmp = mp[make_pair(x, y)];
    makeroot(tmp);
    tmp->pushdown();

    heap.erase(tmp->maxsum);
    tmp->key = z;
    tmp->refresh();
    heap.push(tmp->maxsum);
}

void modify_color(int x) {
    makeroot(null + x);
    col[x] ^= true;

    if (col[x])
        null[x].heap.push(0);
    else
        null[x].heap.erase(0);

    heap.erase(null[x].maxsum);
    null[x].refresh();
    heap.push(null[x].maxsum);
}
node *newnode(int k) {
    *(++ptr) = node(k);
    ptr->ch[0] = ptr->ch[1] = ptr->p = null;
    return ptr;
}
node *access(node *x) {
    splay(x);
    heap.erase(x->maxsum);
    x->refresh();

    if (x->ch[1] != null) {
        x->ch[1]->pushdown();
        x->heap.push(x->ch[1]->prefix);
        x->refresh();
        heap.push(x->ch[1]->maxsum);
    }

    x->ch[1] = null;
    x->refresh();
    node *y = x;
    x = x->p;

    while (x != null) {
        splay(x);
        heap.erase(x->maxsum);

        if (x->ch[1] != null) {
            x->ch[1]->pushdown();
            x->heap.push(x->ch[1]->prefix);
            heap.push(x->ch[1]->maxsum);
        }

        x->heap.erase(y->prefix);
        x->ch[1] = y;
        (y = x)->refresh();
        x = x->p;
    }

    heap.push(y->maxsum);
    return y;
}
void makeroot(node *x) {
    access(x);
    splay(x);
    x->rev ^= true;
}
void link(node *x, node *y) { // 新添一条虚边, 维护y对应的堆
    makeroot(x);
    makeroot(y);

    x->pushdown();
    x->p = y;
    heap.erase(y->maxsum);
    y->heap.push(x->prefix);
    y->refresh();
    heap.push(y->maxsum);
}
void cut(node *x, node *y) { // 断开一条实边, 一条链变成两条链, 需要维护全局堆
    makeroot(x);
    access(y);
    splay(y);

    heap.erase(y->maxsum);
    heap.push(y->ch[0]->maxsum);
    y->ch[0]->p = null;
    y->ch[0] = null;
    y->refresh();
    heap.push(y->maxsum);
}
void splay(node *x) {
    x->pushdown();

    while (!isroot(x)) {
        if (!isroot(x->p))
            x->p->p->pushdown();

        x->p->pushdown();
        x->pushdown();

        if (isroot(x->p)) {
            rot(x->p, dir(x) ^ 1);
            break;
        }

        if (dir(x) == dir(x->p))
            rot(x->p->p, dir(x->p) ^ 1);
        else
            rot(x->p, dir(x) ^ 1);

        rot(x->p, dir(x) ^ 1);
    }
}
void rot(node *x, int d) {
    node *y = x->ch[d ^ 1];

    if ((x->ch[d ^ 1] = y->ch[d]) != null)
        y->ch[d]->p = x;

    y->p = x->p;

    if (!isroot(x))
        x->p->ch[dir(x)] = y;

    (y->ch[d] = x)->p = y;
    
    x->refresh();
    y->refresh();
}