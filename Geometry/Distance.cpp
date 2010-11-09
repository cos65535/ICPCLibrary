Point projection(const Line &l, const Point &p) {
  double t = dot(p - l[0], l[0] - l[1]) / norm(l[0] - l[1]);
  return l[0] + t * (l[0] - l[1]);
}

Point reflection(const Line &l, const Point &p) {
  return p + 2.0 * (projection(l, p) - p);
}

double distanceLP(const Line &l, const Point &p) {
  return abs(p - projection(l, p));
}

double distanceLL(const Line &l, const Line &m) {
  return intersectLL(l, m) ? 0 : distanceLP(l, m[0]);
}

double distanceLS(const Line &l, const Line &s) {
  if (intersectLS(l, s)) { return 0; }
  return min(distanceLP(l, s[0]), distanceLP(l, s[1]));
}

double distanceSP(const Line &s, const Point &p) {
  const Point r = projection(s, p);
  if (intersectSP(s, r)) { return abs(r - p); }
  return min(abs(s[0] - p), abs(s[1] - p));
}

double distanceSS(const Line &s, const Line &t) {
  if (intersectSS(s, t)) { return 0; }
  return min(min(distanceSP(s, t[0]), distanceSP(s, t[1])),
             min(distanceSP(t, s[0]), distanceSP(t, s[1])));
}
