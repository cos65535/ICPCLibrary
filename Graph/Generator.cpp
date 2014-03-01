#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
static const double EPS = 1e-9;

#define REP(i, n) for (int i = 0; i < (int)(n); i++)
#define FOR(i, s, n) for (int i = (s); i < (int)(n); i++)
#define FOREQ(i, s, n) for (int i = (s); i <= (int)(n); i++)
#define FORIT(it, c) for (__typeof((c).begin())it = (c).begin(); it != (c).end(); it++)
#define MEMSET(v, h) memset((v), h, sizeof(v))

// if use testlib, please delete it

// Code from http://ja.wikipedia.org/wiki/Xorshift .
// Initial seeds are overwrited by hand.
uint Xor128(void) { 
  static uint x = 0x34fb2383;
  static uint y = 0x327328fa;
  static uint z = 0xabd4b54a;
  static uint w = 0xa9dba8d1; 
  uint t;
       
  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
}
struct Random {
  int next(int r) { return Xor128() % r; }
  int next(int l, int r) { return next(r - l + 1) + l; }
  ll next(ll r) { return Xor128() % r; }
  ll next(ll l, ll r) { return next(r - l + 1) + l; }
  double next(double r) { return (double)Xor128() / 0xffffffff * r; }
  double next(double l, double r) { return next(r - l) + l; }
};
Random rnd;
template<class T>
void shuffle(T begin, T end) { random_shuffle(begin, end); }
//=================================================================
// Parameter & Variance
//=================================================================
typedef ll Weight;
const bool UNIDIRECT = false;
const bool EXIST_WEIGHT = true;
const bool EXIST_TWICE_EDGE = false;
const int ORIGIN = 0; // if 0 is zero origin, 1 is one origin
const int VERTEX_MAX = 0xdeadbeaf;
const Weight NOP_VALUE = 0xdeadbeaf;
vector<int> mapto; // updated by Rename
double POWER_RANDOM_P = 5.0; // used by PowerRandomTre
int SKEW_T = 1; // used by SkewTree
double SKEW_U = 1.0;  // used by SkewTree
double SKEW_P; // used by SkewTree

//=================================================================
// Base
//=================================================================
struct Edge {
  int src;
  int dest;
  Weight weight;
  Edge() {;}
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

void PrintMatrix(const Matrix &matrix) {
  for (int y = 0; y < (int)matrix.size(); y++) {
    for (int x = 0; x < (int)matrix[y].size(); x++) {
      printf("%lld ", matrix[y][x]);
    }
    puts("");
  }
}

void AddEdge(Graph &g, int f, int t, Weight w) {
  if (UNIDIRECT) {
    g[f].push_back(Edge(f, t, w));
  } else {
    g[f].push_back(Edge(f, t, w));
    g[t].push_back(Edge(t, f, w));
  }
}
//=================================================================
// Converter
//=================================================================
Graph EdgesToGraph(const Edges &edges, int n) {
  Graph g(n);
  FORIT(it, edges) {
    int f = it->src;
    int t = it->dest;
    Weight w = it->weight;
    AddEdge(g, f, t, w);
  }
  return g;
}

Edges GraphToEdges(const Graph &g) {
  const int n = g.size();
  Edges edges;
  REP(from, n) {
    FORIT(it, g[from]) {
      int to = it->dest;
      if (!UNIDIRECT && to < from) { continue; }
      edges.push_back(*it);
    }
  }
  return edges;
}

Matrix GraphToMatrix(const Graph &g) {
  const int n = g.size();
  Matrix ret(n, Array(n, NOP_VALUE));
  REP(i, n) {
    FORIT(it, g[i]) {
      ret[it->src][it->dest] = it->weight;
    }
  }
  return ret;
}

Graph MatrixToGraph(const Matrix &matrix) {
  const int n = matrix.size();
  Graph g(n);
  REP(y, n) REP(x, n) {
    if (matrix[y][x] != NOP_VALUE) {
      g[y].push_back(Edge(y, x, matrix[y][x]));
    }
  }
  return g;
}

//=================================================================
// Utility
//=================================================================
struct UnionFind {
  vector<int> parent;
  UnionFind() : parent(VERTEX_MAX, -1) {;}
  bool unionSet(int x, int y) {
    x = root(x); y = root(y);
    if (x == y) { return false; }
    if (parent[y] < parent[x]) { swap(x, y); }
    parent[x] += parent[y];
    parent[y] = x;
    return true;
  }
  bool findSet(int x, int y) {
    return root(x) == root(y);
  }
  int root(int x) { return parent[x] < 0 ? x : parent[x] = root(parent[x]); }
};

inline int MaxEdge(int n) {
  if (n != 0 && EXIST_TWICE_EDGE) { return 1 << 30; }
  ll ret = (ll)n * (n - 1) / 2;
  if (UNIDIRECT) { ret *= 2; }
  return min(ret, 1LL << 30);
}

int CountEdge(const Graph &g) {
  const int n = g.size();
  int m = 0;
  REP(i, n) { m += g[i].size(); }
  assert(UNIDIRECT || m % 2 == 0);
  if (!UNIDIRECT) { m /= 2; }
  return m;
}

int MaxDegree(const Graph &g) {
  int max_degree = 0;
  REP(i, g.size()) {
    max_degree = max(max_degree, (int)g[i].size());
  }
  return max_degree;
}

// for Tree
int Diameter(const Graph &g) {
  const int n = g.size();
  int start = 0;
  int last = -1;
  int max_depth = 0;
  REP(iter, 2) {
    vector<int> depth(n, -1);
    vector<int> parent(n, -1);
    queue<int> que;
    que.push(start);
    depth[start] = 0;
    parent[start] = start;
    while (!que.empty()) {
      int from = que.front();
      que.pop();
      last = from;
      max_depth = max(max_depth, depth[from]);
      FORIT(it, g[from]) {
        int to = it->dest;
        if (to == parent[from]) { continue; }
        depth[to] = depth[from] + 1;
        parent[to] = from;
        que.push(to);
      }
    }
    start = last;
  }
  return max_depth;
}

Graph Merge(const vector<Graph> &gs) {
  int n = 0;
  int m = gs.size();
  REP(i, m) { n += gs[i].size(); }
  Graph retG(n);
  int offset = 0;
  REP(i, m) {
    int ln = gs[i].size();
    REP(from, ln) {
      FORIT(it, gs[i][from]) {
        int to = it->dest;
        Weight w = it->weight;
        retG[offset + from].push_back(Edge(offset + from, offset + to, w));
      }
    }
    offset += ln;
  }
  return retG;
}

Graph Merge(const Graph &l, const Graph &r) {
  vector<Graph> gs;
  gs.push_back(l);
  gs.push_back(r);
  return Merge(gs);
}

Graph SetRandomWeight(const Graph &g, Weight low, Weight high) {
  if (low > high) { swap(low, high); }
  Edges es = GraphToEdges(g);
  FORIT(it, es) {
    if (high - low == 0) {
      it->weight = 0;
    } else {
      it->weight = rnd.next(low, high);
    }
  }
  return EdgesToGraph(es, g.size());
}

bool Connected(const Graph &g) {
  const int n = g.size();
  UnionFind ufind;
  int cnt = 1;
  REP(from, n) {
    FORIT(it, g[from]) {
      int to = it->dest;
      if (ufind.unionSet(from, to)) {
        cnt++;
        if (cnt == n) { return true; }
      }
    }
  }
  return cnt == n;
}

bool TwiceEdge(const Graph &g) {
  const int n = g.size();
  set<pair<int, int> > open;
  REP(f, n) {
    FORIT(it, g[f]) {
      int t = it->dest;
      if (open.count(make_pair(f, t))) { return true; }
      open.insert(make_pair(f, t));
    }
  }
  return false;
}

Graph Rename(const Graph &g) {
  const int n = g.size();
  mapto = vector<int>(n);
  REP(i, n) { mapto[i] = i; }
  shuffle(mapto.begin(), mapto.end());

  Graph retG(n);
  REP(i, n) {
    int from = mapto[i];
    FORIT(it, g[i]) {
      int to = mapto[it->dest];
      retG[from].push_back(Edge(from, to, it->weight));
    }
  }
  return retG;
}

// Output Format is
// n m
// f1 t1 w1
// ...
// fm tm wm
Graph OutputGraph(FILE *fp, Graph g, bool renameVertex = true, bool shuffleEdge = true) {
  const int n = g.size();
  const int m = CountEdge(g);;
  assert(fp != NULL);
  assert(g.size() != 0);
  if (renameVertex) { g = Rename(g); }
  else {
    mapto = vector<int>(n);
    REP(i, n) { mapto[i] = i; }
  }
  fprintf(fp, "%d %d\n", n, m);
  Edges edges = GraphToEdges(g);
  if (shuffleEdge) {
    shuffle(edges.begin(), edges.end());
  }
  FORIT(it, edges) {
    if (!UNIDIRECT && shuffleEdge && rnd.next(0, 1)) {
      swap(it->src, it->dest);
    }
    if (EXIST_WEIGHT) {
      fprintf(fp, "%d %d %lld\n", it->src + ORIGIN, it->dest + ORIGIN, it->weight);
    } else {
      fprintf(fp, "%d %d\n", it->src + ORIGIN, it->dest + ORIGIN);
    }
  }
  return g;
}


void OutputGraphviz(const Graph &g, const char *filename) {
  const int n = g.size();
  if (n > 10000) { return; }
  FILE *fp = fopen(filename, "w");
  assert(fp);
  fprintf(fp, "digraph hoge {\n");
  string dir = UNIDIRECT ? "forward" : "none";
  REP(i, n) {
    REP(j, g[i].size()) {
      const Edge e = g[i][j];
      char weight[100] = "";
      if (!UNIDIRECT && e.dest < e.src) { continue; }
      if (EXIST_WEIGHT) { snprintf(weight, 99, ", label=\"%lf\"", (double)e.weight); }
      fprintf(fp, "  %d -> %d [dir = %s%s]\n", e.src, e.dest, dir.c_str(), weight);
    }
  }
  fprintf(fp, "}\n");
  fclose(fp);
}

//=================================================================
// Graph Generator
//=================================================================
Graph NoEdgeGraph(int n) {
  return Graph(n);
}

Graph LineGraph(int n) {
  Graph g(n);
  REP(i, n - 1) {
    AddEdge(g, i, i + 1, 0);
  }
  return g;
}

Graph RingGraph(int n) {
  assert(n > 2);
  Graph g(n);
  REP(i, n) {
    AddEdge(g, i, (i + 1) % n, 0);
  }
  return g;
}

Graph StarGraph(int n) {
  Graph g(n);
  FOR(i, 1, n) {
    AddEdge(g, 0, i, 0);
  }
  return g;
}

Graph CompleteGraph(int n) {
  Graph g(n);
  REP(i, n) {
    REP(j, i) {
      AddEdge(g, i, j, 0);
      if (UNIDIRECT) {
        AddEdge(g, j, i, 0);
      }
    }
  }
  return g;
}

Graph BinaryTree(int n) {
  Graph g(n);
  REP(i, n) {
    int f = i;
    int t1 = i * 2 + 1;
    int t2 = i * 2 + 2;
    if (t1 < n) { AddEdge(g, f, t1, 0); }
    if (t2 < n) { AddEdge(g, f, t2, 0); }
  }
  return g;
}

// maybe slow
Graph RandomTree(int n) {
  UnionFind ufind;
  Graph g(n);
  REP(i, n - 1) {
    int f = rnd.next(n);
    int t = rnd.next(n);
    if (!ufind.unionSet(f, t)) {
      i--;
      continue;
    }
    AddEdge(g, f, t, 0);
  }
  return g;
}

// maybe slow
Graph PowerRandomTree(int n) {
  UnionFind ufind;
  Graph g(n);
  REP(f, n - 1) {
    double r = rnd.next(pow(n, POWER_RANDOM_P));
    int t = pow(r, 1.0 / POWER_RANDOM_P);
    if (!ufind.unionSet(f, t)) {
      f--;
      continue;}
    AddEdge(g, f, t, 0);
  }
  return g;
}

// if rnd.next(1.0) < p, vertex i is connected to candidate vertex 
// else vertex i is connected to vertex [0, i - 1] with uniform distribution
// p == 1 && u = 1.0 then diameter is large
// p == 1 && u = 0.0 then there exist large degree vertex
Graph SkewTree(int n) {
  Graph g(n);
  vector<int> cand(SKEW_T);
  FOR(t, 1, n) {
    int f = -1;
    if (rnd.next(1.0) < SKEW_P) {
      f = rnd.next(t);
    } else {
      int r = rnd.next(SKEW_T);
      f = cand[r] != -1 ? cand[r] : t - 1;
      if (rnd.next(1.0) < SKEW_U) {
        cand[r] = t;
      }
    }
    AddEdge(g, f, t, 0);
  }
  return g;
}

Graph RandomGraph(int n, int m) {
  assert(m <= MaxEdge(n));
  set<pair<int, int> > used;
  Graph g(n);
  REP(i, m) {
    int f = rnd.next(n);
    int t = rnd.next(n);
    if (f == t ||
        (!EXIST_TWICE_EDGE && used.count(make_pair(f, t)))) {
      i--;
      continue;
    }
    AddEdge(g, f, t, 0);
    used.insert(make_pair(f, t));
    if (!UNIDIRECT) {
      used.insert(make_pair(t, f));
    }
  }
  return g;
}

Graph EraseEdge(const Graph &g, int m) {
  const int n = g.size();
  assert(m <= CountEdge(g));
  Edges edges = GraphToEdges(g);
  shuffle(edges.begin(), edges.end());
  edges.resize(m);
  return EdgesToGraph(edges, n);
}

Graph AddRandomEdge(Graph g, int m) {
  const int n = g.size();
  assert(EXIST_TWICE_EDGE || m <= MaxEdge(n));
  set<pair<int, int> > used;
  REP(from, n) {
    FORIT(it, g[from]) {
      int to = it->dest;
      used.insert(make_pair(from, to));
      if (!UNIDIRECT) {
        used.insert(make_pair(to, from));
      }
    }
  }
  int cnt = CountEdge(g);
  while (cnt < m) {
    int f = rnd.next(n);
    int t = rnd.next(n);
    if (f == t ||
        (!EXIST_TWICE_EDGE && used.count(make_pair(f, t)))) { continue; }
    AddEdge(g, f, t, 0);
    used.insert(make_pair(f, t));
    if (!UNIDIRECT) {
      used.insert(make_pair(t, f));
    }
    cnt++;
  }
  return g;
}

Graph ConnectedRandomGraph(int n, int m) {
  assert(n - 1 <= m && m <= MaxEdge(n));
  Graph g = RandomTree(n);
  return AddRandomEdge(g, m);
}

// No Checked
// O(m * n^0.5) if use Dinic
// it may have k vertex less than n
Graph DestroyDinicGraph(int n) {
  Matrix m(n, Array(n, 0));
  vector<int> level(n, -1);
  Edges target;
  Graph g(2);
  int cnt = 2;
  int last = 0;
  FOR(i, 1, n) {
    if (cnt + i > n) { break; }
    Graph ng = CompleteGraph(i);
    //Graph ng = NoEdge(i);
    g = Merge(g, ng);
    AddEdge(g, cnt, 1, 0);
    REP(j, i) {
      REP(k, i - 1) {
        AddEdge(g, cnt + j, cnt - 1 - k, 0);
      }
    }
    last = i;
    cnt += i;
  }
  REP(i, last) {
    AddEdge(g, 0, cnt - 1 - i, 0);
  }
  return g;
}
