void ArtDfs(
    const Graph &g,
    int from,
    int parent,
    set<int> &art,
    vector<set<Edge> > &connect,
    vector<Edge> &st,
    vector<int> &order,
    vector<int> &low,
    int &cnt) {
  low[from] = order[from] = cnt++;
  for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
    int to = it->dest;
    if (to != parent && order[to] < order[from]) { st.push_back(*it); }
    if (order[to] == -1) {
      ArtDfs(g, to, from, art, connect, st, order, low, cnt);
      low[from] = min(low[from], low[to]);
      if ((order[from] == 0 && order[to] != 1) ||
          (order[from] != 0 && low[to] >= order[from])) {
        art.insert(from);
      }
      if (low[to] >= order[from]) {
        connect.push_back(set<Edge>());
        while (true) {
          Edge edge = st.back();
          st.pop_back();
          connect.back().insert(edge);
          if (edge.src == from && edge.dest == to) { break; }
        }
      }
    } else {
      low[from] = min(low[from], order[to]);
    }
  }
}

set<int> Art(const Graph &g) {
  const int n = g.size();
  set<int> art;
  vector<set<Edge> > connect;
  vector<Edge> st;
  vector<int> low(n, -1);
  vector<int> order(n, -1);
  for (int i = 0; i < n; i++) {
    if (order[i] != -1) { continue; }
    int cnt = 0;
    ArtDfs(g, i, i, art, connect, st, order, low, cnt);
  }
  return art;
}
