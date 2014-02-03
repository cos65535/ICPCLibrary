struct LCA {
  static const int LOG_SIZE = 16;
  static const int SIZE = 1 << LOG_SIZE;
  int parent[SIZE][LOG_SIZE];
  int depth[SIZE];
  LCA() {;}
  LCA(const Graph &g, int root) {
    init(g, root);
  }
  void init(const Graph &g, int root) {
    int n = g.size();
    assert(n <= SIZE);
    bfs(g, root);
    for (int iter = 0; iter < LOG_SIZE - 1; iter++) {
      for (int i = 0; i < n; i++) {
        parent[i][iter + 1] = parent[parent[i][iter]][iter];
      }
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
private:
  void bfs(const Graph &g, int root) {
    depth[root] = 0;
    parent[root][0] = root;
    queue<int> que;
    que.push(root);
    while (!que.empty()) {
      int node = que.front();
      que.pop();
      for (Edges::const_iterator it = g[node].begin(); it != g[node].end(); it++) {
        int next = it->dest;
        if (next == parent[node][0]) { continue; }
        parent[next][0] = node;
        depth[next] = depth[node] + 1;
        que.push(next);
      }
    }
  }
};
