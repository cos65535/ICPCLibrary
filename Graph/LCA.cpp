struct LCA {
  static const int LOG_SIZE = 16;
  static const int SIZE = 1 << LOG_SIZE;
  int parent[SIZE][LOG_SIZE];
  int depth[SIZE];
  LCA() {;}
  LCA(const Graph &g, int root) {
    int n = g.size();
    assert(n <= SIZE);
    dfs(g, root, root, 0);
    for (int iter = 0; iter < LOG_SIZE - 1; iter++) {
      for (int i = 0; i < n; i++) {
        parent[i][iter + 1] = parent[parent[i][iter]][iter];
      }
    }
  }
  void dfs(const Graph &g, int v, int p, int d) {
    depth[v] = d;
    parent[v][0] = p;
    for (Edges::const_iterator it = g[v].begin(); it != g[v].end(); it++) {
      if (it->dest == p) { continue; }
      dfs(g, it->dest, v, d + 1);
    }
  }
  int lca(int u, int v) {
    if (depth[u] > depth[v]) { swap(u, v); }
    for (int i = 0; i < LOG_SIZE; i++) {
      if (((depth[v] ^ depth[u]) >> i) & 1) { v = parent[v][i]; }
    }
    if (u == v) { return u; }
    for (int i = LOG_SIZE - 1; i >= 0; i--) {
      if (parent[u][i] != parent[v][i]) {
        u = parent[u][i];
        v = parent[v][i];
      }
    }
    return parent[u][0];
  }
};
