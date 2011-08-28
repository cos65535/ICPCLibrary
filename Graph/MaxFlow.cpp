int MaxFlow(const Graph &g, int s, int t) {
  const int n = g.size();
  Matrix capacity(n, Array(n, 0));
  for (int i = 0; i < n; i++) {
    for (Edges::const_iterator it = g[i].begin(); it != g[i].end(); it++) {
      capacity[it->src][it->dest] = it->weight;
    }
  }
  int ret = 0;
  vector<int> parent(n);
  while (true) {
    fill(parent.begin(), parent.end(), -1);
    queue<Edge> que;
    que.push(Edge(s, s, 0));
    while (!que.empty()) {
      Edge node = que.front();
      que.pop();
      if (parent[node.dest] != -1) { continue; }
      parent[node.dest] = node.src;
      if (node.dest == t) { break; }
      int from = node.dest;
      for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
        int to = it->dest;
        if (capacity[from][to] == 0 || parent[to] != -1) { continue; }
        que.push(Edge(from, to, 0));
      }
    }
    if (parent[t] == -1) { break; }
    Weight flow = 2000000000;
    int from = parent[t];
    int to = t;
    while (from != to) {
      flow = min(flow, capacity[from][to]);
      from = parent[from];
      to = parent[to];
    }
    from = parent[t];
    to = t;
    while (from != to) {
      capacity[from][to] -= flow;
      capacity[to][from] += flow;
      from = parent[from];
      to = parent[to];
    }
    ret += flow;
  }
  return ret;
}

void AddEdge(Graph &g, int from, int to, Weight capacity) {
  g[from].push_back(Edge(from, to, capacity));
  g[to].push_back(Edge(to, from, 0));
}
