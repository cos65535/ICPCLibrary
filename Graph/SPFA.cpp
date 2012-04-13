static const Weight INF = 2000000000LL;
vector<Weight> SPFA(const Graph &g, int s) {
  const int n = g.size();
  vector<Weight> dist = vector<Weight>(n, 1 << 28);
  dist[s] = 0;
  vector<int> updated(n, 0);
  vector<bool> inque(n, false);
  queue<int> que;
  que.push(s);
  while (!que.empty()) {
    int from = que.front();
    que.pop();
    inque[from] = false;
    updated[from]++;
    if (updated[from] == n) {
      // exist negative cycle
      fill(dist.begin(), dist.end(), -INF);
      break;
    }
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      int to = it->dest;
      Weight cost = dist[from] + it->weight;
      if (cost < dist[to]) {
        dist[to] = cost;
        if (!inque[to]) {
          que.push(to);
          inque[to] = true;
        }
      }
    }
  }
  return dist;
}
