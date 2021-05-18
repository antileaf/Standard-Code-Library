
#include <bits/stdc++.h>
 
using namespace std;
 
constexpr int maxn = 1000005, mod = 1000000007;
 
int val[maxn], par[maxn], go[maxn][26], fail[maxn][26], pam_last[maxn], pam_cnt;
int weight[maxn], pow_26[maxn];
 
int trie[maxn][26], trie_cnt, d[maxn], mxd[maxn], son[maxn], top[maxn], len[maxn], sum[maxn];
char chr[maxn];
int f[25][maxn], log_tbl[maxn];
vector<int> v[maxn];
 
vector<int> queries[maxn];
 
char str[maxn];
int n, m, ans[maxn];
 
int add(int x, int c) {
    if (!trie[x][c]) {
        trie[x][c] = ++trie_cnt;
        f[0][trie[x][c]] = x;
        chr[trie[x][c]] = c + 'a';
    }
     
    return trie[x][c];
}
 
int del(int x) {
    return f[0][x];
}
 
void dfs1(int x) {
    mxd[x] = d[x] = d[f[0][x]] + 1;
 
    for (int i = 0; i < 26; i++)
        if (trie[x][i]) {
            int y = trie[x][i];
 
            dfs1(y);
 
            mxd[x] = max(mxd[x], mxd[y]);
            if (mxd[y] > mxd[son[x]])
                son[x] = y;
        }
}
 
void dfs2(int x) {
    if (x == son[f[0][x]])
        top[x] = top[f[0][x]];
    else
        top[x] = x;
     
    for (int i = 0; i < 26; i++)
        if (trie[x][i]) {
            int y = trie[x][i];
            dfs2(y);
        }
     
    if (top[x] == x) {
        int u = x;
        while (top[son[u]] == x)
            u = son[u];
         
        len[x] = d[u] - d[x];
 
        for (int i = 0; i < len[x]; i++) {
            v[x].push_back(u);
            u = f[0][u];
        }
 
        u = x;
        for (int i = 0; i < len[x]; i++) { // 梯子剖分，要延长一倍
            v[x].push_back(u);
            u = f[0][u];
        }
    }
}
 
int get_anc(int x, int k) {
    if (!k)
        return x;
    if (k > d[x])
        return 0;
     
    x = f[log_tbl[k]][x];
    k ^= 1 << log_tbl[k];
 
    return v[top[x]][d[top[x]] + len[top[x]] - d[x] + k];
}
 
char get_char(int x, int k) { // 查询x前面k个的字符是哪个
    return chr[get_anc(x, k)];
}
 
int getfail(int x, int p) {
    if (get_char(x, val[p] + 1) == chr[x])
        return p;
    return fail[p][chr[x] - 'a'];
}
 
int extend(int x) {
 
    int p = pam_last[f[0][x]], c = chr[x] - 'a';
     
    p = getfail(x, p);
 
    int new_last;
     
    if (!go[p][c]) {
        int q = ++pam_cnt, now = p;
        val[q] = val[p] + 2;
 
        p = getfail(x, par[p]);
 
        par[q] = go[p][c];
        new_last = go[now][c] = q;
         
        for (int i = 0; i < 26; i++)
            fail[q][i] = fail[par[q]][i];
         
        if (get_char(x, val[par[q]]) >= 'a')
            fail[q][get_char(x, val[par[q]]) - 'a'] = par[q];
         
        if (val[q] <= n)
            weight[q] = (weight[par[q]] + (long long)(n - val[q] + 1) * pow_26[n - val[q]]) % mod;
        else
            weight[q] = weight[par[q]];
    }
    else
        new_last = go[p][c];
     
    pam_last[x] = new_last;
 
    return weight[pam_last[x]];
}
 
void bfs() {
 
    queue<int> q;
 
    q.push(1);
 
    while (!q.empty()) {
        int x = q.front();
        q.pop();
 
        sum[x] = sum[f[0][x]];
        if (x > 1)
            sum[x] = (sum[x] + extend(x)) % mod;
             
        for (int i : queries[x])
            ans[i] = sum[x];
         
        for (int i = 0; i < 26; i++)
            if (trie[x][i])
                q.push(trie[x][i]);
    }
     
}
 
int main() {
 
    pow_26[0] = 1;
    log_tbl[0] = -1;
 
    for (int i = 1; i <= 1000000; i++) {
        pow_26[i] = 26ll * pow_26[i - 1] % mod;
        log_tbl[i] = log_tbl[i / 2] + 1;
    }
 
    int T;
    scanf("%d", &T);
 
    while (T--) {
        scanf("%d%d%s", &n, &m, str);
 
        trie_cnt = 1;
        chr[1] = '#';
 
        int last = 1;
        for (char *c = str; *c; c++)
            last = add(last, *c - 'a');
         
        queries[last].push_back(0);
         
        for (int i = 1; i <= m; i++) {
            int op;
            scanf("%d", &op);
             
            if (op == 1) {
                char c;
                scanf(" %c", &c);
 
                last = add(last, c - 'a');
            }
            else
                last = del(last);
             
            queries[last].push_back(i);
        }
 
        dfs1(1);
        dfs2(1);
 
        for (int j = 1; j <= log_tbl[trie_cnt]; j++)
            for (int i = 1; i <= trie_cnt; i++)
                f[j][i] = f[j - 1][f[j - 1][i]];
         
        par[0] = pam_cnt = 1;
         
         
        for (int i = 0; i < 26; i++)
            fail[0][i] = fail[1][i] = 1;
         
        val[1] = -1;
        pam_last[1] = 1;
 
        bfs();
 
        for (int i = 0; i <= m; i++)
            printf("%d\n", ans[i]);
         
        for (int j = 0; j <= log_tbl[trie_cnt]; j++)
            memset(f[j], 0, sizeof(f[j]));
 
        for (int i = 1; i <= trie_cnt; i++) {
            chr[i] = 0;
            d[i] = mxd[i] = son[i] = top[i] = len[i] = pam_last[i] = sum[i] = 0;
            v[i].clear();
            queries[i].clear();
 
            memset(trie[i], 0, sizeof(trie[i]));
        }
        trie_cnt = 0;
 
        for (int i = 0; i <= pam_cnt; i++) {
            val[i] = par[i] = weight[i];
 
            memset(go[i], 0, sizeof(go[i]));
            memset(fail[i], 0, sizeof(fail[i]));
        }
        pam_cnt = 0;
 
    }
 
    return 0;
}