Weight augment(const Graph &g, Matrix &capacity, const vector<int> &level, vector<bool> &finished, int from, int t, Weight cur) {
  if (from == t || cur == 0) { return cur; }
  if (finished[from]) { return 0; }
  for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
    int to = it->dest;
    if (level[to] <= level[from]) { continue; }
    Weight f = augment(g, capacity, level, finished, to, t, min(cur, capacity[from][to]));
    if (f > 0) {
      capacity[from][to] -= f;
      capacity[to][from] += f;
      return f;
    }
  }
  finished[from] = true;
  return 0;
}

Weight MaxFlow(const Graph &g, int s, int t) {
  int n = g.size();
  Matrix capacity(n, Array(n));
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      int to = it->dest;
      capacity[from][to] += it->weight;
    }
  }
  int ans = 0;
  while (true) {
    vector<int> level(n, -1);
    level[s] = 0;
    queue<int> que;
    que.push(s);
    for (int d = n; !que.empty() && level[que.front()] < d; ) {
      int from = que.front();
      que.pop();
      if (from == t) { d = level[from]; }
      for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
        int to = it->dest;
        if (capacity[from][to] > 0 && level[to] == -1) {
          que.push(to);
          level[to] = level[from] + 1;
        }
      }
    }
    vector<bool> finished(n);
    bool end = true;
    while (true) {
      Weight f = augment(g, capacity, level, finished, s, t, 2000000000LL);
      if (f == 0) { break; }
      ans += f;
      end = false;
    }
    if (end) { break; }
  }
  return ans;
}

void AddEdge(Graph &g, int from, int to, Weight capacity) {
  g[from].push_back(Edge(from, to, capacity));
  g[to].push_back(Edge(to, from, 0));
}
