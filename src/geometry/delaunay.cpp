#include <bits/stdc++.h>
 
using namespace std;

constexpr int maxn = 500005;

using ll = long long;

constexpr int INF = 0x3f3f3f3f;
constexpr ll LINF = 0x3f3f3f3f3f3f3f3fll;
constexpr double eps = 1e-8;

template <class T>
int sgn(T x) {
	return x > 0 ? 1 : x < 0 ? -1 : 0;
}

struct point {
	ll x, y;

	point() = default;

	point(ll x, ll y) : x(x), y(y) {}

	point operator - (const point &p) const {
		return point(x - p.x, y - p.y);
	}

	ll cross(const point &p) const {
		return x * p.y - y * p.x;
	}

	ll cross(const point &a, const point &b) const {
		return (a - *this).cross(b - *this);
	}

	ll dot(const point &p) const {
		return x * p.x + y * p.y;
	}

	ll dot(const point &a, const point &b) const {
		return (a - *this).dot(b - *this);
	}

	ll abs2() const {
		return this -> dot(*this);
	}

	bool operator == (const point &p) const {
		return x == p.x && y == p.y;
	}

	bool operator < (const point &p) const {
		if (x != p.x) return x < p.x;
		return y < p.y;
	}
};



const point inf_point = point(1e18, 1e18);

struct quad_edge {
	point origin;
	quad_edge *rot = nullptr;
	quad_edge *onext = nullptr;
	bool used = false;

	quad_edge *rev() const {
		return rot -> rot;
	}
	quad_edge *lnext() const {
		return rot -> rev() -> onext -> rot;
	}
	quad_edge *oprev() const {
		return rot -> onext -> rot;
	}
	point dest() const {
		return rev() -> origin;
	}
};

quad_edge *make_edge(point from, point to) {
	quad_edge *e1 = new quad_edge;
	quad_edge *e2 = new quad_edge;
	quad_edge *e3 = new quad_edge;
	quad_edge *e4 = new quad_edge;

	e1 -> origin = from;
	e2 -> origin = to;
	e3 -> origin = e4 -> origin = inf_point;

	e1 -> rot = e3;
	e2 -> rot = e4;
	e3 -> rot = e2;
	e4 -> rot = e1;

	e1 -> onext = e1;
	e2 -> onext = e2;
	e3 -> onext = e4;
	e4 -> onext = e3;

	return e1;
}

void splice(quad_edge *a, quad_edge *b) { // 拼接
	swap(a -> onext -> rot -> onext, b -> onext -> rot -> onext);
	swap(a -> onext, b -> onext);
}

void delete_edge(quad_edge *e) {
	splice(e, e->oprev());
	splice(e->rev(), e->rev()->oprev());

	delete e->rev()->rot;
	delete e->rev();
	delete e->rot;
	delete e;
}

quad_edge *connect(quad_edge *a, quad_edge *b) {
	quad_edge *e = make_edge(a -> dest(), b -> origin);

	splice(e, a->lnext());
	splice(e->rev(), b);
	
	return e;
}

bool left_of(point p, quad_edge *e) {
	return p.cross(e -> origin, e -> dest()) > 0;
}

bool right_of(point p, quad_edge *e) {
	return p.cross(e -> origin, e -> dest()) < 0;
}

template <class T>
T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
	return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) +
		   a3 * (b1 * c2 - c1 * b2);
}

bool in_circle(point a, point b, point c, point d) { // 如果有__int128就直接计算行列式，否则算角度
#if defined(__LP64__) || defined(_WIN64)
	__int128 det = -det3<__int128>(b.x, b.y, b.abs2(), c.x, c.y, c.abs2(), d.x, d.y, d.abs2());
	det += det3<__int128>(a.x, a.y, a.abs2(), c.x, c.y, c.abs2(), d.x, d.y, d.abs2());
	det -= det3<__int128>(a.x, a.y, a.abs2(), b.x, b.y, b.abs2(), d.x, d.y, d.abs2());
	det += det3<__int128>(a.x, a.y, a.abs2(), b.x, b.y, b.abs2(), c.x, c.y, c.abs2());

	return det > 0;
#else
	auto ang = [] (point l, point mid, point r) {
		ll x = mid.dot(l, r);
		ll y = mid.cross(l, r);
		long double res = atan2((long double)x, (long double)y);
		return res;
	};

	long double kek = ang(a, b, c) + ang(c, d, a) - ang(b, c, d) - ang(d, a, b);

	return kek > 1e-10;
#endif
}

pair<quad_edge*, quad_edge*> divide_and_conquer(int l, int r, vector<point> &p) {
	if (r - l + 1 == 2) {
		quad_edge *res = make_edge(p[l], p[r]);
		return make_pair(res, res -> rev());
	}

	if (r - l + 1 == 3) {
		quad_edge *a = make_edge(p[l], p[l + 1]), *b = make_edge(p[l + 1], p[r]);
		splice(a -> rev(), b);

		int sg = sgn(p[l].cross(p[l + 1], p[r]));

		if (sg == 0)
			return make_pair(a, b -> rev());

		quad_edge *c = connect(b, a);

		if (sg == 1)
			return make_pair(a, b -> rev());
		else
			return make_pair(c -> rev(), c);
	}

	int mid = (l + r) / 2;

	quad_edge *ldo, *ldi, *rdo, *rdi;
	tie(ldo, ldi) = divide_and_conquer(l, mid, p);
	tie(rdi, rdo) = divide_and_conquer(mid + 1, r, p);
	
	while (true) {
		if (left_of(rdi -> origin, ldi))
			ldi = ldi -> lnext();

		else if (right_of(ldi -> origin, rdi))
			rdi = rdi -> rev() -> onext;

		else
			break;
	}

	quad_edge *basel = connect(rdi -> rev(), ldi);
	auto is_valid = [&basel] (quad_edge *e) {
		return right_of(e -> dest(), basel);
	};

	if (ldi -> origin == ldo -> origin)
		ldo = basel -> rev();
	if (rdi -> origin == rdo -> origin)
		rdo = basel;

	while (true) {
		quad_edge *lcand = basel -> rev() -> onext;
		if (is_valid(lcand)) {
			while (in_circle(basel -> dest(), basel -> origin, lcand -> dest(), lcand -> onext -> dest())) {
				quad_edge *t = lcand -> onext;
				delete_edge(lcand);
				lcand = t;
			}
		}

		quad_edge *rcand = basel -> oprev();
		if (is_valid(rcand)) {
			while (in_circle(basel -> dest(), basel -> origin, rcand -> dest(), rcand -> oprev() -> dest())) {
				quad_edge *t = rcand -> oprev();
				delete_edge(rcand);
				rcand = t;
			}
		}

		if (!is_valid(lcand) && !is_valid(rcand))
			break;

		if (!is_valid(lcand) || (is_valid(rcand) && in_circle(lcand -> dest(), lcand -> origin, rcand -> origin, rcand -> dest())))
			basel = connect(rcand, basel -> rev());
		else
			basel = connect(basel -> rev(), lcand -> rev());
	}

	return make_pair(ldo, rdo);
}

vector<tuple<point, point, point> > delaunay(vector<point> p) { // Delaunay 三角剖分
	sort(p.begin(), p.end(), [] (const point &a, const point &b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y); // 实际上已经重载小于了，只是为了清晰
	});

	auto res = divide_and_conquer(0, (int)p.size() - 1, p);

	quad_edge *e = res.first;
	vector<quad_edge*> edges = {e};

	while (e -> onext -> dest().cross(e -> dest(), e -> origin) < 0)
		e = e -> onext;

	auto add = [&p, &e, &edges] () { // 修改 p, e, edges
		quad_edge *cur = e;
		do {
			cur -> used = true;
			p.push_back(cur -> origin);
			edges.push_back(cur -> rev());

			cur = cur -> lnext();
		} while (cur != e);
	};

	add();
	p.clear();

	int kek = 0;
	while (kek < (int)edges.size())
		if (!(e = edges[kek++]) -> used)
			add();

	vector<tuple<point, point, point> > ans;
	for (int i = 0; i < (int)p.size(); i += 3)
		ans.push_back(make_tuple(p[i], p[i + 1], p[i + 2]));

	return ans; // 返回每个三角形
}

#define sq(x) ((x) * (ll)(x)) // 平方

ll dist(point p, point q) { // 两点间距离的平方
	return (p - q).abs2();
}

ll sarea2(point p, point q, point r) { // 三角形面积的两倍（叉积）
	return (q - p).cross(r - q);
}

point v[maxn];

int main() {
	int n;
	cin >> n;

	// read the points, v[1 ~ n]

	bool col_linear = true; // 如果给出的所有点都共线则需要特判
	for (int i = 3; i <= n; i++)
		if (sarea2(v[1], v[2], v[i]))
			col_linear = false;
 
	if (col_linear) {
		// do something

		return 0;
	}

	auto triangles = delaunay(vector<point>(v + 1, v + n + 1));

	// do something

	return 0;
}