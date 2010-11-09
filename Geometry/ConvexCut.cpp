Polygon convex_cut(const Polygon &P, const Line &l) {
  Polygon Q;
  for (int i = 0; i < (int)P.size(); i++) {
    Point A = CURR(P, i), B = NEXT(P, i);
    if (ccw(l[0], l[1], A) != -1) { Q.push_back(A); }
    if (ccw(l[0], l[1], A) * ccw(l[0], l[1], B) < 0) {
      Q.push_back(crosspoint(Line(A, B), l));
    }
  }
  return Q;
}


