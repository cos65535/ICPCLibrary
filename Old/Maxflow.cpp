const int NODE_SIZE = 100;
int parent[NODE_SIZE];
int maxflow(int capacity[NODE_SIZE][NODE_SIZE], int s, int e, int size) {
  int ans = 0;
  while (true) {
    memset(parent, -1, sizeof(parent));
    queue<int> que;
    que.push(s);
    parent[s] = s;
    while (!que.empty()) {
      int from = que.front();
      que.pop();
      if (from == e) { break; }
      for (int to = 0; to < size; to++) {
        if (capacity[from][to] == 0 || parent[to] != -1) { continue; }
        parent[to] = from;
        que.push(to);
      }
    }
    if (parent[e] == -1) { break; }
    int flow = 2000000000;
    int from = parent[e];
    int to = e;
    while (from != to) {
      flow = min(flow, capacity[from][to]);
      from = parent[from];
      to = parent[to];
    }
    from = parent[e];
    to = e;
    while (from != to) {
      capacity[from][to] -= flow;
      capacity[to][from] += flow;
      from = parent[from];
      to = parent[to];
    }
    ans += flow;
  }
  return ans;
}
