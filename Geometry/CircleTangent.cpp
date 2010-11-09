vector<Line> tangentCP(const Circle &c, const Point &p) {
  vector<Line> ret;
  Point vect = c.p - p;
  double d = abs(vect);
  double l = sqrt(norm(vect) - c.r * c.r);
  if (isnan(l)) { return ret; }
  Point v1 = vect * Point(l / d,  c.r / d); 
  Point v2 = vect * Point(l / d, -c.r / d); 
  ret.push_back(Line(p - v1 * INF, p + v1 * INF));
  if (l > EPS) {
    ret.push_back(Line(p - v2 * INF, p + v2 * INF));
  }
  return ret;
}

vector<Line> tangentCC(const Circle &c1, const Circle &c2) {
  vector<Line> ret;
  if (abs(c1.p - c2.p) - (c1.r + c2.r) > -EPS) {
    Point center = (c1.p * c2.r + c2.p * c1.r) / (c1.r + c2.r);
    ret = tangentCP(c1, center);
  }
  if (fabs(c1.r - c2.r) > EPS) {
    Point out = (-c1.p * c2.r + c2.p * c1.r) / (c1.r - c2.r);
    vector<Line> nret = tangentCP(c1, out);
    ret.insert(ret.end(), nret.begin(), nret.end());
  } else {
    Point vect = c2.p - c1.p;
    vect /= abs(vect);
    Point q1 = c1.p + vect * Point(0, 1) * c1.r;
    Point q2 = c1.p + vect * Point(0, -1) * c1.r;
    ret.push_back(Line(q1 - vect * INF, q1 + vect * INF));
    ret.push_back(Line(q2 - vect * INF, q2 + vect * INF));
  }
  return ret;
}
