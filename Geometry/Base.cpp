#include <complex>
#include <vector>

typedef complex<double> Point;
typedef vector<Point> Polygon;

//static const double EPS = 1e-8;
static const double INF = 1e+10;

#define CURR(P, i) (P[i])
#define NEXT(P, i) (P[(i + 1) % P.size()])

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
  bool operator<(const Point &lhs, const Point &rhs) {
    return lhs.real() == rhs.real() ? lhs.imag() < rhs.imag() : lhs.real() < rhs.real();
  }
}

inline double cross(const Point &a, const Point &b) {
  return imag(conj(a) * b);
}

inline double dot(const Point &a, const Point &b) {
  return real(conj(a) * b);
}

inline int ccw(Point a, Point b, Point c) {
  b -= a;
  c -= a;
  if (cross(b, c) > 0) { return 1; }
  if (cross(b, c) < 0) { return -1; }
  if (dot(b, c) < 0) { return 2; }
  if (norm(b) < norm(c)) { return -2; }
  return 0;
}

