#include <vector>
#include <complex>
#include <assert.h>

using namespace std;

typedef complex<double> Complex;
typedef Complex Point;
typedef vector<Point> Polygon;

const double EPS = 1e-8;
const double INF = 1e12;

struct Line : public vector<Point> {
  Line(const Point &a, const Point &b) { push_back(a); push_back(b); }
};

struct Circle {
  Point p;
  double r;
  Circle(const Point &p, double r) : p(p), r(r) {;}
};

namespace std {
  bool operator<(const Point &lhs, const Point &rhs) {
    return real(lhs) != real(rhs) ? real(lhs) < real(rhs) : imag(lhs) < imag(rhs);
  }
}

#define CURR(P, i) (P[i])
#define NEXT(P, i) (P[(i + 1) % P.size()])
enum { OUT, ON, IN };

double cross(const Point &a, const Point &b) {
  return imag(conj(a) * b);
}

double dot(const Point &a, const Point &b) {
  return real(conj(a) * b);
}

int ccw(Point a, Point b, Point c) {
  b -= a; c -= a;
  if (cross(b, c) > 0) { return 1; }     // counter clockwise
  if (cross(b, c) < 0) { return -1; }    // clockwise
  if (dot(b, c) < 0) { return 2; }       // c--a--b on line
  if (norm(b) < norm(c)) { return -2; }  // a--b--c on line
  return 0;
}

bool intersectLL(const Line &l, const Line &m) {
  return abs(cross(l[1] - l[0], m[1] - m[0])) > EPS || // non-parallel
         abs(cross(l[1] - l[0], m[0] - l[0])) < EPS;  // same line
}

bool intersectLS(const Line &l, const Line &s) {
  return cross(l[1] - l[0], s[0] - l[0]) *       // s[0] is left of l
         cross(l[1] - l[0], s[1] - l[0]) < EPS;  // s[1] is right of l
}

bool intersectLP(const Line &l, const Point &p) {
  return abs(cross(l[1] - p, l[0] - p)) < EPS;
}

bool intersectSS(const Line &s, const Line &t) {
  return ccw(s[0], s[1], t[0]) * ccw(s[0], s[1], t[1]) <= 0 &&
         ccw(t[0], t[1], s[0]) * ccw(t[0], t[1], s[1]) <= 0;
}

bool intersectSP(const Line &s, const Point &p) {
  return abs(s[0] - p) + abs(s[1] -p) - abs(s[1] - s[0]) < EPS;  // triangle inequality
}

bool intersectPP(const Point &a, const Point &b) {
  return abs(a - b) < EPS;
}

bool intersectSPolygon(const Line &l, const Polygon &p) {
  for (int i = 0; i < (int)p.size(); i++) {
    Line s(CURR(p, i), NEXT(p, i));
    if (intersectSS(l, s)) { return true; }
  }
  return false;
}

bool intersectPolygonPolygon(const Polygon &p1, const Polygon &p2) {
  for (int i = 0; i < (int)p1.size(); i++) {
    Line l(CURR(p1, i), NEXT(p1, i));
    if (intersectSPolygon(l, p2)) { return true; }
  }
  return false;
}

bool contains(const Polygon &P, const Point &p) {
  bool in = false;
  for (int i = 0; i < (int)P.size(); i++) {
    Point a = CURR(P, i) - p;
    Point b = NEXT(P, i) - p;
    if (imag(a) > imag(b)) { swap(a, b); }
    if (imag(a) <= 0 && 0 < imag(b)) {
      if (cross(a, b) < 0) { in = !in; }
    }
    if (cross(a, b) == 0 && dot(a, b) <= 0) { return ON; }
  }
  return in ? IN : OUT;
}

vector<Point> convex_hull(vector<Point> ps) {
  const int n = ps.size();
  int k = 0;
  sort(ps.begin(), ps.end());
  vector<Point> ch(2 * n);
  for (int i = 0; i < n; ch[k++] = ps[i++]) {  // lower-hull
    while (k >= 2 && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) { --k; }
  }
  for (int i = n - 2, t = k + 1; i >= 0; ch[k++] = ps[i--]) {  // upper-hull
    while (k >= t && ccw(ch[k - 2], ch[k - 1], ps[i]) <= 0) { --k; }
  }
  ch.resize(k - 1);
  return ch;
}

Polygon makePolygon(Random &random, int &n, double left, double top, double right, double bottom) {
  assert(n >= 3);
  Polygon polygon(0);
  while (polygon.size() < 3) {
    polygon = Polygon(n);
    for (int i = 0; i < n; i++) {
      polygon[i] = Point(random.drand(left, right), random.drand(bottom, top));
    }
    convex_hull(polygon);
  }
  n = (int)polygon.size();
  return polygon;
}

Polygon makeNotIntersectPolygon(const vector<Polygon> &polys, Random &random, int &n, double left, double top, double right, int bottom) {
  Polygon polygon;
  int m;
  while (true) {
    m = n;
    polygon = makePolygon(random, m, left, top, right, bottom);
    for (int i = 0; i < (int)polys.size(); i++) {
      if (intersectPolygonPolygon(polygon, polys[i])) { goto next; }
    }
    break;
next:;
  }
  n = m;
  return polygon;
}
