Point crosspointSS(const Line &l, const Line &m) {
  double A = cross(l[1] - l[0], m[1] - m[0]);
  double B = cross(l[1] - l[0], l[1] - m[0]);
  if (abs(A) < EPS && abs(B) < EPS) {
    assert(false);
    if (intersectSP(l, m[0])) { return m[0]; }
    if (intersectSP(l, m[1])) { return m[1]; }
    if (intersectSP(m, l[0])) { return l[0]; }
    if (intersectSP(m, l[1])) { return l[1]; }
    return m[0];
  }
  if (abs(A) < EPS) { assert(false); }
  return m[0] + B / A * (m[1] - m[0]);
}

vector<Point> crosspointLC(const Line &l, const Circle &c) {
  vector<Point> ret;
  Point center = projection(l, c.p);
  double d = abs(center - c.p);
  double t = sqrt(c.r * c.r - d * d);
  if (isnan(t)) { return ret; }
  Point vect = (l[1] - l[0]);
  vect /= abs(vect);
  ret.push_back(center - vect * t);
  if (t > EPS) {
    ret.push_back(center + vect * t);
  }
  return ret;
}

vector<Point> crosspointSC(const Line &s, const Circle &c) {
  vector<Point> ret;
  vector<Point> nret = crosspointLC(s, c);
  for (int i = 0; i < (int)nret.size(); i++) {
    if (intersectSP(s, nret[i])) { ret.push_back(nret[i]); }
  }
  return ret;
}

vector<Point> crosspointCC(const Circle &c1, const Circle &c2) {
  vector<Point> ret;
  double d = abs(c1.p - c2.p);
  if (max(c1.r, c2.r) - min(c1.r, c2.r) - d> -EPS) { return ret; }
  double x = (d * d + c1.r * c1.r - c2.r * c2.r) / (2 * d);
  Point start = c1.p + (c2.p - c1.p) / d * x;
  Point vect = (c1.p - c2.p) * Point(0.0, 1.0);
  return crosspointLC(Line(start, start + vect), c1);
}
