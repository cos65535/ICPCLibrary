typedef int Weight;
struct Edge {
  int src;
  int dest;
  Weight weight;
  Edge(int src, int dest, Weight weight) : src(src), dest(dest), weight(weight) {;}
  bool operator<(const Edge &rhs) const {
    if (weight != rhs.weight) { return weight > rhs.weight; }
    if (src != rhs.src) { return src < rhs.src; }
    return dest < rhs.dest;
  }
};
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;
typedef vector<Weight> Array;
typedef vector<Array> Matrix;

//=====================================================================
// Bridge

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

//================================================================
// Art

void ArtDfs(
    const Graph &g,
    int from,
    int parent,
    set<int> &art,
    vector<set<Edge> > &connect,
    vector<Edge> &st,
    vector<int> &order,
    vector<int> &low,
    int &cnt) {
  low[from] = order[from] = cnt++;
  for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
    int to = it->dest;
    if (to != parent && order[to] < order[from]) { st.push_back(*it); }
    if (order[to] == -1) {
      ArtDfs(g, to, from, art, connect, st, order, low, cnt);
      low[from] = min(low[from], low[to]);
      if ((order[from] == 0 && order[to] != 1) ||
          (order[from] != 0 && low[to] >= order[from])) {
        art.insert(from);
      }
      if (low[to] >= order[from]) {
        connect.push_back(set<Edge>());
        while (true) {
          Edge edge = st.back();
          st.pop_back();
          connect.back().insert(edge);
          if (edge.src == from && edge.dest == to) { break; }
        }
      }
    } else {
      low[from] = min(low[from], order[to]);
    }
  }
}

set<int> Art(const Graph &g) {
  const int n = g.size();
  set<int> art;
  vector<set<Edge> > connect;
  vector<Edge> st;
  vector<int> low(n, -1);
  vector<int> order(n, -1);
  for (int i = 0; i < n; i++) {
    if (order[i] != -1) { continue; }
    int cnt = 0;
    ArtDfs(g, i, i, art, connect, st, order, low, cnt);
  }
  return art;
}

//======================================================================
// SCC

void SccDfs(const Graph &g, int from, vector<int> &visit, vector<int> &st) {
  visit[from] = 1;
  for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
    if (visit[it->dest]) { continue; }
    SccDfs(g, it->dest, visit, st);
  }
  st.push_back(from);
}

vector<vector<int> > Scc(const Graph &g) {
  const int n = g.size();
  vector<vector<int> > ret;
  Graph revg(n);
  for (int i = 0; i < n; i++) {
    for (Edges::const_iterator it = g[i].begin(); it != g[i].end(); it++) {
      revg[it->dest].push_back(Edge(it->dest, i, it->weight));
    }
  }
  vector<int> st;
  vector<int> visit(n, 0);
  for (int i = 0; i < n; i++) {
    if (visit[i]) { continue; }
    SccDfs(g, i, visit, st);
  }
  visit = vector<int>(n, 0);
  for (int i = n - 1; i >= 0; i--) {
    int index = st[i];
    if (visit[index]) { continue; }
    vector<int> nret;
    SccDfs(revg, index, visit, nret);
    ret.push_back(nret);
  }
  return ret;
}

// select min weight edge
Graph SccGraph(const Graph &g) {
  vector<vector<int> > scc = Scc(g);
  const int n = g.size();
  const int m = scc.size();
  vector<int> mapto(n);
  for (int i = 0; i < (int)scc.size(); i++) {
    for (int j = 0; j < (int)scc[i].size(); j++) {
      mapto[scc[i][j]] = i;
    }
  }
  Graph ret(m);
  vector<int> indexs(m, -1);
  for (int from = 0; from < m; from++) {
    int e = 0;
    for (int i = 0; i < (int)scc[from].size(); i++) {
      for (Edges::const_iterator it = g[scc[from][i]].begin(); it != g[scc[from][i]].end(); it++) {
        int to = mapto[it->dest];
        if (from == to) { continue; } // loopback
        if (indexs[to] == -1) {
          ret[from].push_back(Edge(from, to, it->weight));
          indexs[to] = e++;
        } else {
          // select edge
          ret[from][indexs[to]].weight = min(ret[from][indexs[to]].weight, it->weight);
        }
      }
    }
    for (Edges::const_iterator it = ret[from].begin(); it != ret[from].end(); it++) {
      indexs[it->dest] = -1;
    }
  }
  return ret;
}


//=================================================================
// DirectedMinimumSpanningTree

// need Scc
Weight Arborescence(const Graph &g, int root) {
  const int n = g.size();
  Weight ret = 0;
  vector<Weight> inCost(n, 1000000000LL);
  vector<int> parent(n, -1);
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      if (it->dest == from) { continue; }
      if (it->weight < inCost[it->dest]) {
        parent[it->dest] = from;
        inCost[it->dest] = it->weight;
      }
      inCost[it->dest] = min(inCost[it->dest], it->weight);
    }
  }
  Graph ng(n);
  for (int i = 0; i < n; i++) {
    if (i == root) { continue; }
    if (parent[i] == -1) { return 1000000000LL; }
    ng[parent[i]].push_back(Edge(parent[i], i, 0));
    ret += inCost[i];
  }
  vector<vector<int> > connect = Scc(ng);
  int m = connect.size();
  if (m == n) { return ret; }
  vector<int> mapto(n, -1);
  vector<int> cycle(n, 0);
  ret = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < (int)connect[i].size(); j++) {
      mapto[connect[i][j]] = i;
      if (connect[i].size() != 1) {
        cycle[connect[i][j]] = 1;
        ret += inCost[connect[i][j]];
      }
    }
  }
  ng = Graph(m);
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      int to = it->dest;
      if (to == root || mapto[from] == mapto[to]) { continue; }
      Weight cost = it->weight;
      if (cycle[to]) {
        cost -= inCost[to];
      }
      ng[mapto[from]].push_back(Edge(mapto[from], mapto[to], cost));
    }
  }
  return min(1000000000LL, (ll)ret + Arborescence(ng, mapto[root]));
}

//==============================================================
// Steiner Tree

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

  REP(k, n) REP(i, n) REP(j, n)
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
        opt[code][q] = min(opt[code][q], opt[div][q] + opt[div ^ code][q]);
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
// int matrix[20][20];
// int opt[20][1 << 10];
// pair<int, int> opt_parent[20][1 << 10];
// int source[10];
// void DreyfusWanger() {
//   REP(k, n) REP(i, n) REP(j, n)
//     matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
//   memset(opt, 0x01, sizeof(opt));
//   memset(opt_parent, -1, sizeof(opt));
//   for (int i = 0; i < size; i++) {
//     for (int q = 0; q < n; q++) {
//       opt[q][1 << i] = matrix[q][source[i]];
//       opt[q][0] = 0;
//     }
//   }
//   for (int i = 2; i <= size; i++) {
//     for (int start = 0; start < n; start++) {
//       for (int code = 0; code < (1 << size); code++) {
//         if (__bultin_popcount(code) != i) { continue; }
//         for (int div = 1; div < code; div++) {
//           if (__builtin_popcount(div | code) != i) { continue; }
//           for (int q = 0; q < n; q++) {
//             //opt[start][code] = min(opt[start][code], matrix[q][start] + opt[q][div] + opt[q][div ^ code]);
//             int cost = matrix[q][start] + opt[q][div] + opt[q][div ^ code];
//             if (cost < opt[start][code]) {
//               opt[start][code] = cost;
//               opt_parent[start][code].first = q;
//               opt_parent[start][code].second = div;
//             }
//           }
//         }
//       }
//     }
//   }
// }

//=========================================================
// Sparse Dinic

typedef int Weight;
struct Edge {
  int index;
  int src;
  int dest;
  Weight weight;
  Edge(int index, int src, int dest, Weight weight) : index(index), src(src), dest(dest), weight(weight) {;}
  bool operator<(const Edge &rhs) const {
    if (weight != rhs.weight) { return weight > rhs.weight; }
    if (src != rhs.src) { return src < rhs.src; }
    return dest < rhs.dest;
  }
};
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;
typedef vector<Weight> Array;
typedef vector<Array> Matrix;

void PrintMatrix(const Matrix &matrix) {
  REP(y, matrix.size()) {
    REP(x, matrix[y].size()) {
      printf("%d ", matrix[y][x]);
    }
    puts("");
  }
}

Weight augment(const Graph &g, Array &capacity, const vector<int> &level, vector<bool> &finished, int from, int t, Weight cur) {
  if (from == t || cur == 0) { return cur; }
  if (finished[from]) { return 0; }
  for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
    int to = it->dest;
    if (level[to] != level[from] + 1) { continue; }
    Weight f = augment(g, capacity, level, finished, to, t, min(cur, capacity[it->index]));
    if (f > 0) {
      capacity[it->index] -= f;
      capacity[it->index^1] += f;
      return f;
    }
  }
  finished[from] = true;
  return 0;
}

// index^1 is reverse edge
Weight MaxFlow(const Graph &g, int e, int s, int t) {
  int n = g.size();
  Array capacity(e);
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      capacity[it->index] += it->weight;
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
        if (capacity[it->index] > 0 && level[to] == -1) {
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

void AddEdge(Graph &g, int &e, int from, int to, Weight capacity) {
  g[from].push_back(Edge(e++, from, to, capacity));
  g[to].push_back(Edge(e++, to, from, 0));
}


//==================================================================
// Sparse Min Cost Flow

typedef int Weight;
struct Edge {
  int index;
  int src;
  int dest;
  Weight capacity;
  Weight cost;
  Edge(int index, int src, int dest, Weight capacity, Weight cost) : index(index), src(src), dest(dest), capacity(capacity), cost(cost) {;}
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

void printMatrix(const Matrix &matrix) {
  for (int y = 0; y < (int)matrix.size(); y++) {
    for (int x = 0; x < (int)matrix[y].size(); x++) {
      printf("%d ", matrix[y][x]);
    }
    puts("");
  }
}

// index^1 is reverse edge
pair<Weight, Weight> MinCostFlow(const Graph &g, int e, int s, int t) {
  const int n = g.size();
  Array capacity(e, 0);
  for (int from = 0; from < n; from++) {
    for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
      capacity[it->index] += it->capacity;
    }
  }
  pair<Weight, Weight> ret = make_pair(0, 0);
  vector<pair<int, int> > parent(n);
  vector<Weight> prev_dist(n, 0);
  vector<Weight> now_dist(n);

  // calc potential
  for (int iter = 0; iter < n; iter++) {
    bool end = true;
    for (int from = 0; from < n; from++) {
      for (int i = 0; i < (int)g[from].size(); i++) {
        if (capacity[g[from][i].index] <= 0) { continue; }
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
    fill(parent.begin(), parent.end(), make_pair(-1, -1));
    fill(now_dist.begin(), now_dist.end(), 2000000000LL);
    priority_queue<Edge> que;
    que.push(Edge(e, s, s, 0, 0));
    now_dist[s] = 0;
    while (!que.empty()) {
      Edge node = que.top();
      que.pop();
      if (parent[node.dest].first != -1) { continue; }
      parent[node.dest] = make_pair(node.src, node.index);
      int from = node.dest;
      for (int i = 0; i < (int)g[from].size(); i++) {
        int to = g[from][i].dest;
        int index = g[from][i].index;
        Weight ncost = node.cost + g[from][i].cost + (prev_dist[from] - prev_dist[to]);
        if (capacity[index] <= 0) { continue; }
        if (ncost >= now_dist[to]) { continue; }
        now_dist[to] = ncost;
        que.push(Edge(i, from, to, 0, ncost));
      }
    }
    if (parent[t].first == -1) { break; }
    pair<int, int> index = parent[t];
    Weight flow = 2000000000LL;
    while (index.second != e) {
      flow = min(flow, capacity[g[index.first][index.second].index]);
      index = parent[index.first];
    }
    ret.first += flow;
    index = parent[t];
    while (index.second != e) {
      Edge edge = g[index.first][index.second];
      ret.second += flow * edge.cost;
      capacity[edge.index] -= flow;
      capacity[edge.index^1] += flow;
      index = parent[index.first];
    }
    for (int i = 0; i < n; i++) {
      prev_dist[i] += now_dist[i];
    }
  }
  return ret;
}

void AddEdge(Graph &g, int &e, int from, int to, Weight capacity, Weight cost) {
  g[from].push_back(Edge(e++, from, to, capacity, cost));
  g[to].push_back(Edge(e++, to, from, 0, -cost));
}
