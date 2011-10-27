// need Scc
Weight Arborescence(const Graph &g, int root) {
  const int n = g.size();
  Weight ret = 0;
  vector<Weight> inCost(n, 1000000000LL);
  vector<int> parent(n, -1);
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      if (it->dest == from) { continue; }
      if (it->weight < inCost[it->dest]) {
        parent[it->dest] = from;
        inCost[it->dest] = it->weight;
      }
      inCost[it->dest] = min(inCost[it->dest], it->weight);
    }
  }
  Graph ng(n);
  for (int i = 0; i < n; i++) {
    if (i == root) { continue; }
    if (parent[i] == -1) { return 1000000000LL; }
    ng[parent[i]].push_back(Edge(parent[i], i, 0));
    ret += inCost[i];
  }
  vector<vector<int> > connect = Scc(ng);
  int m = connect.size();
  if (m == n) { return ret; }
  vector<int> mapto(n, -1);
  vector<int> cycle(n, 0);
  ret = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < (int)connect[i].size(); j++) {
      mapto[connect[i][j]] = i;
      if (connect[i].size() != 1) {
        cycle[connect[i][j]] = 1;
        ret += inCost[connect[i][j]];
      }
    }
  }
  ng = Graph(m);
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      int to = it->dest;
      if (to == root || mapto[from] == mapto[to]) { continue; }
      Weight cost = it->weight;
      if (cycle[to]) {
        cost -= inCost[to];
      }
      ng[mapto[from]].push_back(Edge(mapto[from], mapto[to], cost));
    }
  }
  return min(1000000000LL, (ll)ret + Arborescence(ng, mapto[root]));
}
