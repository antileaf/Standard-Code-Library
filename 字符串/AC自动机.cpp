// Aho-Corasick Automata AC自动机
// By AntiLeaf
// 通过题目：bzoj3881 Divljak


// 全局变量与数组定义
int ch[maxm][26] = {{0}}, f[maxm][26] = {{0}}, q[maxm] = {0}, sum[maxm] = {0}, cnt = 0;


// 在字典树中插入一个字符串 O(n)
int insert(const char *c) {
    int x = 0;
    while (*c) {
        if (!ch[x][*c - 'a'])
            ch[x][*c - 'a'] = ++cnt;
        x = ch[x][*c++ - 'a'];
    }
    return x;
}


// 建AC自动机 O(n*sigma)
void getfail() {
    int x, head = 0, tail = 0;

    for (int c = 0; c < 26; c++)
        if (ch[0][c])
            q[tail++] = ch[0][c]; // 把根节点的儿子加入队列
    
    while (head != tail) {
        x = q[head++];
        
        G[f[x][0]].push_back(x);
        fill(f[x] + 1, f[x] + 26, cnt + 1);

        for (int c = 0; c < 26; c++) {
            if (ch[x][c]) {
                int y = f[x][0];

                while (y&&!ch[y][c])
                    y=f[y][0];

                f[ch[x][c]][0] = ch[y][c];
                q[tail++] = ch[x][c];
            }
            else
                ch[x][c] = ch[f[x][0]][c];
        }
    }
    fill(f[0], f[0] + 26, cnt + 1);
}