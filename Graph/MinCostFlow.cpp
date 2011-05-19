typedef int Weight;
struct Edge {
  int src;
  int dest;
  Weight capacity;
  Weight cost;
  Edge(int src, int dest, Weight capacity, Weight cost) : src(src), dest(dest), capacity(capacity), cost(cost) {;}
  bool operator<(const Edge &rhs) const {
    if (cost != rhs.cost) { return cost > rhs.cost; }
    if (src != rhs.src) { return src < rhs.src; }
    return dest < rhs.dest;
  }
};
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;
typedef vector<Weight> Array;
typedef vector<Array> Matrix;

pair<Weight, Weight> MinCostFlow(const Graph &g, int s, int t) {
  const int n = g.size();
  Matrix capacity(n, Array(n, 0));
  Matrix cost(n, Array(n, 0));
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      capacity[it->src][it->dest] += it->capacity;
      cost[it->src][it->dest] += it->cost;
    }
  }
  pair<Weight, Weight> ret = make_pair(0, 0);
  vector<int> parent(n);
  vector<Weight> prev_dist(n, 0);
  vector<Weight> now_dist(n);

  // calc potential
  for (int iter = 0; iter < n; iter++) {
    bool end = true;
    for (int from = 0; from < n; from++) {
      for (int i = 0; i < (int)g[from].size(); i++) {
        if (g[from][i].capacity <= 0) { continue; }
        int to = g[from][i].dest;
        Weight ncost = prev_dist[from] + g[from][i].cost;
        if (ncost < prev_dist[to]) {
          end = false;
          prev_dist[to] = ncost;
        }
      }
    }
    if (end) { break; }
    if (iter == n - 1) {
      assert(false); // exist negative cycle
    }
  }

  while (true) {
    fill(parent.begin(), parent.end(), -1);
    fill(now_dist.begin(), now_dist.end(), 2000000000LL);
    priority_queue<Edge> que;
    que.push(Edge(s, s, 0, 0));
    now_dist[s] = 0;
    while (!que.empty()) {
      Edge node = que.top();
      que.pop();
      if (parent[node.dest] != -1) { continue; }
      parent[node.dest] = node.src;
      int from = node.dest;
      for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
        int to = it->dest;
        Weight ncost = node.cost + cost[from][to] + (prev_dist[from] - prev_dist[to]);
        if (capacity[from][to] == 0) { continue; }
        if (ncost >= now_dist[to]) { continue; }
        now_dist[to] = ncost;
        que.push(Edge(from, to, 0, ncost));
      }
    }
    if (parent[t] == -1) { break; }
    int from = parent[t];
    int to = t;
    Weight flow = 2000000000LL;
    while (from != to) {
      flow = min(flow, capacity[from][to]);
      from = parent[from];
      to = parent[to];
    }
    ret.first += flow;
    from = parent[t];
    to = t;
    while (from != to) {
      ret.second += flow * cost[from][to];
      capacity[from][to] -= flow;
      capacity[to][from] += flow;
      from = parent[from];
      to = parent[to];
    }
    for (int i = 0; i < n; i++) {
      prev_dist[i] += now_dist[i];
    }
  }
  return ret;
}

void AddEdge(Graph &g, int from, int to, Weight capacity, Weight cost) {
  g[from].push_back(Edge(from, to, capacity, cost));
  g[to].push_back(Edge(to, from, 0, -cost));
}
