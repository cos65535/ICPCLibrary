double CircleCircleArea(double r1, double r2, double d) {
  if (r1 + r2 - d < EPS) { return 0; }
  if (r1 < r2) { swap(r1, r2); }
  if (r1 - r2 - d > -EPS || r2 < EPS) {
    return r2 * r2 * PI; 
  }
  double t1 = acos((d * d + r1 * r1 - r2 * r2) / (2 * d * r1));
  double t2 = acos((d * d + r2 * r2 - r1 * r1) / (2 * d * r2));
  double sq = (r1 * r1 * sin(t1 * 2) + r2 * r2 * sin(t2 * 2)) / 2.0;
  double s = r1 * r1 * t1  
    + r2 * r2 * t2
    - sq; 
  assert(s >= -EPS);
  assert(!isnan(s));
  return s;
}
