struct HeavyLightDecomposition {
  struct Chain {
    int depth;
    pair<int, int> parent; // chain number, index
    vector<pair<int, int> > child; // child chain number, parent index
    vector<int> mapfrom;
    //SegmentTree stree;
  };
  Graph baseG;
  vector<Chain> chains;
  vector<pair<int, int> > mapto; // raw index -> chain number & index
  vector<vector<int> > mapfrom; // chain number & index -> raw index

  HeavyLightDecomposition() {;}
  HeavyLightDecomposition(const Graph &g) {
    baseG = g;
    Init();
  }
  void Init() {
    const int n = baseG.size();
    mapto = vector<pair<int, int> >(n, make_pair(-1, -1));
    mapfrom.clear();
    vector<int> size(n, 0);
    int start = -1;
    for (int i = 0; i < n; i++) {
      if (baseG[i].size() <= 1) {
        start = i;
        break;
      }
    }
    assert(start != -1);
    SizeCheckBfs(start, size);
    Decomposition(start, start, 0, 0, 0, size);
  }

  int Depth(int t) {
    return chains[mapto[t].first].depth;
  }
  bool SameChain(int f, int t) {
    return mapto[f].first == mapto[t].first;
  }

private:
  int Decomposition(int from, int parent, int depth, int pnumber, int pindex, const vector<int> &size) {
    const int c = chains.size();
    chains.push_back(Chain());
    chains[c].depth = depth;
    chains[c].parent = make_pair(pnumber, pindex);
    vector<int> seq;
    Bfs(from, parent, seq, size);
    for (int i = 0; i < (int)seq.size(); i++) {
      mapto[seq[i]] = make_pair(c, i);
      chains[c].mapfrom.push_back(seq[i]);
    }
    //chains[c].stree = SegmentTree(seq.size());
    for (int i = 0; i < (int)seq.size(); i++) {
      for (Edges::const_iterator it = baseG[seq[i]].begin(); it != baseG[seq[i]].end(); it++) {
        if (mapto[it->dest].first != -1) { continue; }
        int nc = Decomposition(it->dest, seq[i], depth + 1, c, i, size);
        chains[c].child.push_back(make_pair(nc, i));
      }
    }
    return c;
  }
  void SizeCheckBfs(int start, vector<int> &size) {
    const int n = baseG.size();
    queue<pair<int, int> > que;
    que.push(make_pair(start, start));
    int cnt = 0;
    vector<int> order(n, -1);
    while (!que.empty()) {
      int from = que.front().first;
      int parent = que.front().second;
      que.pop();
      order[cnt++] = from;
      for (Edges::const_iterator it = baseG[from].begin(); it != baseG[from].end(); it++) {
        if (it->dest == parent) { continue; }
        que.push(make_pair(it->dest, from));
      }
    }
    assert(cnt == n);
    reverse(order.begin(), order.end());
    REP(i, n) {
      int from = order[i];
      size[from] = 1;
      for (Edges::const_iterator it = baseG[from].begin(); it != baseG[from].end(); it++) {
        size[from] += size[it->dest];
      }
    }
  }
  void Bfs(int from, int parent, vector<int> &seq, const vector<int> &size) {
    while (true) {
      seq.push_back(from);
      int best = -1;
      int next = -1;
      for (Edges::const_iterator it = baseG[from].begin(); it != baseG[from].end(); it++) {
        if (it->dest == parent) { continue; }
        if (best < size[it->dest]) {
          best = size[it->dest];
          next = it->dest;
        }
      }
      if (next == -1) { break; }
      parent = from;
      from = next;
    }
  }
};
