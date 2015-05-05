// Need Area
struct Angle {
  double rad;
  int src;
  int src_e;
  int dest;
  Angle() {;}
  Angle(double rad, int src, int src_e, int dest) : rad(rad), src(src), src_e(src_e), dest(dest) {;}
  bool operator<(const Angle &rhs) const {
    return rad < rhs.rad;
  }
};
vector<Polygon> PlainGraphToPolygon(const Graph &g, const vector<Point> &ps, bool remove_2edge = true, bool remove_clockwize = true) {
  const int n = g.size();
  assert((int)ps.size() == n);
  vector<vector<char> > visit(n);
  vector<vector<Angle> > angles(n);
  REP(from, n) {
    visit[from] = vector<char>(g[from].size(), false);
    int index = 0;
    FORIT(it, g[from]) {
      int to = it->dest;
      double rad = arg(ps[to] - ps[from]);
      angles[from].push_back(Angle(rad, from, index, to));
      angles[from].push_back(Angle(rad + 2 * PI, from, index, to));
      index++;
    }
    sort(angles[from].begin(), angles[from].end());
  }

  vector<Polygon> ret;
  REP(start_node, n) {
    REP(start_edge, g[start_node].size()) {
      if (visit[start_node][start_edge]) { continue; }
      Polygon poly;
      Edge e = g[start_node][start_edge];
      double rad = arg(ps[e.dest] - ps[e.src]);
      Angle angle(rad, start_node, start_edge, e.dest);
      while (true) {
        int from = angle.dest;
        angle.rad += PI + EPS;
        while (angle.rad > PI) { angle.rad -= 2 * PI; }
        angle = *upper_bound(angles[from].begin(), angles[from].end(), angle);
        if (visit[angle.src][angle.src_e]) { break; }
        visit[angle.src][angle.src_e] = 1;
        poly.push_back(ps[from]);
      }
      if (remove_2edge && poly.size() == 2) { continue; }
      if (remove_clockwize && Area(poly) > 0) { continue; }
      ret.push_back(poly);
    }
  }
  return ret;
}
