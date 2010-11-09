static const Weight INF = 2000000000LL;
Weight BellmanFord(const Graph &g, int s, int t) {
  const int n = g.size();
  Array dist(n, 0x0f0f0f0f);
  dist[s] = 0;
  REP(iter, n) {
    bool end = true;
    REP(i, n) {
      REP(j, g[i].size()) {
        int to = g[i][j].dest;
        Weight ncost = dist[i] + g[i][j].weight;
        if (dist[to] > ncost) {
          dist[to] = ncost;
          end = false; 
        }
      }
    }     
    if (end) { return dist[t]; }
  }
  return -INF;
}
