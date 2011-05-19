Line BisectorLine(const Point &p, const Point &q) {
  Point center = (p + q) / 2.0;
  Point vect = (q - p) * Point(0, 1);
  vect /= abs(vect);
  return Line(center - vect * INF, center + vect * INF);
}

Circle ThreePointCircle(const Point &a, const Point &b, const Point &c) {
  Circle ret;
  Line line1 = BisectorLine(a, b);
  Line line2 = BisectorLine(a, c);
  if (!intersectLL(line1, line2)) {
    ret.p = Point(0.0/0.0, 0.0/0.0);
    ret.r = 0.0/0.0;
  } else {
    ret.p = crosspointSS(line1, line2);
    ret.r = abs(ret.p - a);
  }
  return ret;
}
