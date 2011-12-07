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

int BridgeCompressDfs(const Graph &g, int from, int id, map<int, int> &mapto, set<Edge> &ban) {
  mapto[from] = id;
  int ret = 1;
  for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
    if (mapto.count(it->dest) || ban.count(*it)) { continue; }
    ret += BridgeCompressDfs(g, it->dest, id, mapto, ban);
  }
  return ret;
}

Graph BridgeCompress(const Graph &g) {
  const int n = g.size();
  Edges bridge = Bridge(g);
  set<Edge> ban;
  for (Edges::const_iterator it = bridge.begin(); it != bridge.end(); it++) {
    ban.insert(*it);
    ban.insert(Edge(it->dest, it->src, it->weight));
  }
  int m = 0;
  map<int, int> mapto;
  REP(i, n) {
    if (mapto.count(i)) { continue; }
    int w = BridgeCompressDfs(g, i, m, mapto, ban);
    m++;
  }
  Graph ret(m);
  for (set<Edge>::iterator it = ban.begin(); it != ban.end(); it++) {
    ret[mapto[it->src]].push_back(Edge(mapto[it->src], mapto[it->dest], it->weight));
  }
  return ret;
}
