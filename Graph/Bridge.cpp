void BridgeDfs(
    const Graph &g,
    int from,
    int parent,
    Edges &bridge,
    vector<vector<int> > &connect,
    vector<int> &roots,
    vector<int> &st,
    vector<int> &order,
    int &cnt) {
  order[from] = cnt++;
  st.push_back(from);
  roots.push_back(from);
  for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
    int to = it->dest;
    if (to == parent) { continue; }
    if (order[to] == -1) {
      BridgeDfs(g, to, from, bridge, connect, roots, st, order, cnt);
    } else {
      while (order[roots.back()] > order[to]) { roots.pop_back(); }
    }
  }
  if (from == roots.back()) {
    bridge.push_back(Edge(parent, from, 0));
    connect.push_back(vector<int>());
    while (true) {
      int w = st.back();
      st.pop_back();
      connect.back().push_back(w);
      if (from == w) { break; }
    }
    roots.pop_back();
  }
}

Edges Bridge(const Graph &g) {
  const int n = g.size();
  Edges bridge;
  vector<int> order(n, -1);
  vector<int> roots, st;
  vector<vector<int> > connect;
  int cnt = 0;
  for (int i = 0; i < n; i++) {
    if (order[i] != -1) { continue; }
    BridgeDfs(g, i, i, bridge, connect, roots, st, order, cnt);
    bridge.pop_back();
  }
  return bridge;
}
