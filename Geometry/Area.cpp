double Area(const Polygon &p) {
  double ret = 0;
  for (int i = 0; i < (int)p.size(); i++) {
    ret += cross(CURR(p, i), NEXT(p, i));
  }
  return ret / 2.0;
}
