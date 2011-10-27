// O(n^2logn)
int MaxPointCircle(const vector<Point> &points, double r) {
  const int n = points.size();
  int ret = 1;
  for (int i = 0; i < n; i++) {
    vector<pair<double, int> > radians;
    for (int j = 0; j < n; j++) {
      double d = abs(points[i] - points[j]) / 2.0;
      if (i == j || d * 2.0 - r * 2.0 > EPS) { continue; }
      Point center = (points[i] + points[j]) / 2.0;
      Point vect = (points[j] - points[i]) * Point(0, -1);
      vect /= abs(vect);
      double l = sqrt(r * r - d * d);
      Point c1 = center + vect * l;
      Point c2 = center - vect * l;
      double arg1 = arg(c1 - points[i]) - EPS;
      double arg2 = arg(c2 - points[i]) + EPS;
      radians.push_back(make_pair(arg1, (j + 1)));
      radians.push_back(make_pair(arg2, -(j + 1)));
    }
    if ((int)radians.size() / 2 < ret - 1) { continue; }
    sort(radians.begin(), radians.end());
    vector<bool> used(points.size(), false);
    int lsum = 1;
    REP(iter, 2) {
      REP(j, radians.size()) {
        int index = abs(radians[j].second) - 1;
        bool start = radians[j].second > 0 ? true : false;
        if (used[index] && !start) {
          used[index] = false;
          lsum--;
        } else if (!used[index] && start) {
          used[index] = true;
          lsum++;
        }
        ret = max(ret, lsum);
      }
    }
  }
  return ret;
}
