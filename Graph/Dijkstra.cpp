Weight Dijkstra(const Graph &g, int s, int t) {
  const int n = g.size();
  vector<bool> visit(n, false);
  Array dist(n, 2000000000LL);
  priority_queue<Edge> que;
  que.push(Edge(s, s, 0));
  Weight ans = -1;
  dist[s] = 0;
  while (!que.empty()) {
    Edge edge = que.top();
    que.pop();
    int from = edge.dest;
    if (visit[from]) { continue; }
    visit[from] = true;
    if (from == t) {
      ans = edge.weight;
      break;
    }   
    for (int i = 0; i < (int)g[from].size(); i++) {
      int to = g[from][i].dest;
      Weight ncost = edge.weight + g[from][i].weight;
      if (visit[to] || ncost >= dist[to]) { continue; }
      dist[to] = ncost;
      que.push(Edge(from, to, ncost));
    }   
  }
  return ans;
}
