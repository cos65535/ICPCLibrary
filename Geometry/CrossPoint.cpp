Point crosspointSS(const Line &l, const Line &m) {
  double A = cross(l[1] - l[0], m[1] - m[0]);
  double B = cross(l[1] - l[0], l[1] - m[0]);
  if (abs(A) < EPS && abs(B) < EPS) { return m[0]; }
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
