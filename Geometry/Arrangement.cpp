// O(n^2)
struct ArrangementEdge {
  int src;
  int dest;
  Weight weight;
  ArrangementEdge(int src, int dest, Weight weight) : src(src), dest(dest), weight(weight) {;}
  bool operator<(const ArrangementEdge &rhs) const {
    return dest < rhs.dest;
  }
};
struct ArrangementData {
  vector<Point> ps;
  map<Point, int> mapto;
  Graph g;
};
ArrangementData Arrangement(const vector<Line> &ls, const vector<Point> &ps) {
  // Constraint Check
  {
    // Weight should be double
    Weight w = 0.5;
    assert(w == 0.5);
    // near Point should be used as same point
    Point p1(0, 0);
    Point p2(EPS, EPS);
    assert(!(p1 < p2) && !(p2 < p1));
  }

  int l = ls.size();
  int n = 0;
  ArrangementData ret;
  // Push Point
  for (int i = 0; i < l; i++) {
    REP(j, 2) {
      if (ret.mapto.count(ls[i][j])) { continue; }
      ret.ps.push_back(ls[i][j]);
      ret.mapto[ls[i][j]] = n++;
    }
  }
  for (int i = 0; i < (int)ps.size(); i++) {
    if (ret.mapto.count(ps[i])) { continue; }
    ret.ps.push_back(ps[i]);
    ret.mapto[ps[i]] = n++;
  }

  // Crosspoint Push
  for (int i = 0; i < l; i++) {
    for (int j = i + 1; j < l; j++) {
      Line s1 = ls[i];
      Line s2 = ls[j];
      if (intersectSS(s1, s2)) {
        Point p = crosspointSS(s1, s2);
        if (ret.mapto.count(p)) { continue; }
        ret.ps.push_back(p);
        ret.mapto[p] = n++;
      }
    }
  }

  // Make Graph
  vector<set<ArrangementEdge> > tempG(n);
  for (int i = 0; i < l; i++) {
    Line s = ls[i];
    vector<pair<double, int> > onLine;
    for (int j = 0; j < n; j++) {
      if (intersectSP(s, ret.ps[j])) {
        onLine.push_back(make_pair(abs(ret.ps[j] - s[0]), j));
      }
    }
    sort(onLine.begin(), onLine.end());
    for (int j = 0; j < (int)onLine.size() - 1; j++) {
      int from = onLine[j].second;
      int to = onLine[j + 1].second;
      double dist = fabs(onLine[j].first - onLine[j + 1].first);
      tempG[from].insert(ArrangementEdge(from, to, dist));
      tempG[to].insert(ArrangementEdge(to, from, dist));
    }
  }

  // Convert Graph
  ret.g = Graph(n);
  for (int i = 0; i < n; i++) {
    for (set<ArrangementEdge>::iterator it = tempG[i].begin(); it != tempG[i].end(); it++) {
      ret.g[i].push_back(Edge(it->src, it->dest, it->weight));
    }
  }
  return ret;
}
