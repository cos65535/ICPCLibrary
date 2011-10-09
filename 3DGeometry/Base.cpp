// valarrayはサイズが同じでないとコピーが上手く働かないので注意。
// Point p;
// p = a - b;
// といったコードはバグる。
// グローバル変数にも注意。

#include <valarray>
typedef valarray<double> Point;
Point PointInit(double x, double y, double z) {
  Point ret(3);
  ret[0] = x;
  ret[1] = y;
  ret[2] = z;
  return ret;
}
struct Line : public vector<Point> {
  Line() {;}
  Line(Point a, Point b) { push_back(a); push_back(b); }
};

struct Circle {
  Point p;
  double r;
  Circle() {;}
  Circle(Point p, double r) : p(p), r(r) {;}
};

namespace std {
  ostream &operator<<(ostream &os, const Point &rhs) {
    assert(rhs.size() == 3);
    char str[200];
    snprintf(str, 199, "(%.2f, %.2f, %.2f)", rhs[0], rhs[1], rhs[2]);
    os << str;
    return os;
  }
};

inline double dot(const Point &a, const Point &b) {
  return  (a * b).sum();
}

inline Point Cross(const Point &a, const Point &b) {
  return a.cshift(+1) * b.cshift(-1)
       - a.cshift(-1) * b.cshift(+1);
}
inline double Norm(const Point &p) {
  return dot(p, p);
}
inline double Abs(const Point &p) {
  return sqrt(dot(p, p));
}

Point projection(const Line &l, const Point &p) {
  double t = dot(p - l[0], l[0] - l[1]) / Norm(l[0] - l[1]);
  return l[0] + t * (l[0] - l[1]);
}

bool intersectSP(const Line &s, const Point &p) {
  return Abs(s[0] - p) + Abs(s[1] - p) - Abs(s[1] - s[0]) < EPS;
}

double distanceSP(const Line &s, const Point &p) {
  const Point r = projection(s, p);
  if (intersectSP(s, r)) { return Abs(r - p); }
  return min(Abs(s[0] - p), Abs(s[1] - p));
}
