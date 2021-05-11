#include <bits/stdc++.h>
 
using namespace std;
 
const int maxn = 300005;
 
int a[maxn], q[maxn], p[maxn], g[maxn]; // 存左端点，右端点就是下一个左端点 - 1
 
long long f[maxn], s[maxn];
 
int n, m;
 
long long calc(int l, int r) {
    if (r < l)
        return 0;
 
    int mid = (l + r) / 2;
    if ((r - l + 1) % 2 == 0)
        return (s[r] - s[mid]) - (s[mid] - s[l - 1]);
    else
        return (s[r] - s[mid]) - (s[mid - 1] - s[l - 1]);
}
 
int solve(long long tmp) {
    memset(f, 63, sizeof(f));
    f[0] = 0;
 
    int head = 1, tail = 0;
 
    // printf("---------- solve(%lld) ----------\n", tmp);
 
    for (int i = 1; i <= n; i++) {
        f[i] = calc(1, i);
        g[i] = 1;
 
        while (head < tail && p[head + 1] <= i)
            head++;
        if (head <= tail) {
            if (f[q[head]] + calc(q[head] + 1, i) < f[i]) {
                f[i] = f[q[head]] + calc(q[head] + 1, i);
                g[i] = g[q[head]] + 1;
            }
            while (head < tail && p[head + 1] <= i + 1)
                head++;
            if (head <= tail)
                p[head] = i + 1;
        }
        f[i] += tmp;
        // printf("f[%d] = %lld g[%d] = %d\n", i, f[i], i, g[i]);
        
        /*
        if (head <= tail && f[q[tail]] + calc(q[tail] + 1, n) <= f[i] + calc(i + 1, n))
            continue;
        */
 
        int r = n;
 
        while(head <= tail) {
            if (f[q[tail]] + calc(q[tail] + 1, p[tail]) > f[i] + calc(i + 1, p[tail])) {
                r = p[tail] - 1;
                tail--;
            }
            else if (f[q[tail]] + calc(q[tail] + 1, r) <= f[i] + calc(i + 1, r)) {
                if (r < n) {
                    q[++tail] = i;
                    p[tail] = r + 1;
                }
                break;
            }
            else {
                int L = p[tail], R = r;
                while (L < R) {
                    int M = (L + R) / 2;
 
                    if (f[q[tail]] + calc(q[tail] + 1, M) <= f[i] + calc(i + 1, M))
                        L = M + 1;
                    else
                        R = M;
                }
 
                q[++tail] = i;
                p[tail] = L;
 
                break;
            }
        }
        if (head > tail) {
            q[++tail] = i;
            p[tail] = i + 1;
        }
    }
 
    return g[n];
}
 
int main() {
    scanf("%d%d", &n, &m);
 
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        s[i] = s[i - 1] + a[i];
    }
    
    long long L = 0, R = 1e16;
 
    while (L < R) {
        long long M = (L + R) / 2;
        if (solve(M) > m)
            L = M + 1;
        else
            R = M;
    }
 
    solve(L);
 
    printf("%lld\n", f[n] - m * L);
 
    return 0;
}