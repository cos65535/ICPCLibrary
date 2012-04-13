int DreyfusWanger(const Graph &g, const vector<int> &terminal) {
  const int n = g.size();
  const int s = terminal.size();

  // init dist
  Matrix dist(n, Array(n, 1LL << 30));
  for (int i = 0; i < n; i++) { dist[i][i] = 0; }
  for (int from = 0; from < n; from++) {
    for (Edges::iterator it = g[from].begin(); it != g[from].end(); it++) {
      dist[it->src][it->dest] = min(dist[it->src][it->dest], it->cost);
    }
  }
  for (int k = 0; k < n; k++)
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

  // init opt
  vector<vector<int> > opt(1 << s, vector<int>(n, 1LL << 30));
  for (int i = 0; i < s; i++) {
    for (int q = 0; q < n; q++) {
      opt[1 << i][q] = matrix[terminal[i]][q];
      opt[0][q] = 0;
    }
  }

  // calc opt
  for (int code = 1; code < (1 << s); code++) {
    for (int div = code & -code; div < code; div = (div - code) & code) {
      for (int q = 0; q < n; q++) {
        opt[code][q] = min(opt[q][code], opt[div][q] + opt[div ^ code][q]);
      }
    }
    for (int q = 0; q < n; q++) {
      for (int end = 0; end < n; end++) {
        opt[code][end] = min(opt[code][end], opt[code][q] + dist[q][end]);
      }
    }
  }

  int ret = 1 << 30;
  REP(i, n) { ret = min(ret, opt[(1 << s) - 1][i]); }
  return ret;
}
