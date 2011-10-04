struct AhoCorasick {
  static const int SIZE = 128;
  struct State {
    int index;
    int next[SIZE]; // next[0] is failuer link
    vector<int> accept;
    State(int index) : index(index) {
      memset(next, -1, sizeof(next));
    }
  };

  bool build;
  vector<State> pma;
  vector<int> lens;
  //vector<string. strs;

  AhoCorasick() {
    build = false;
    pma.clear();
    pma.push_back(State(0));
    lens.clear();
    //strs.clear();
  }

  void AddString(const char *str) {
    assert(!build);
    int t = 0;
    for (int i = 0; str[i]; i++) {
      int c = str[i];
      if (pma[t].next[c] == -1) {
        int m = pma.size();
        pma[t].next[c] = m;
        pma.push_back(State(m));
      }
      t = pma[t].next[c];
    }
    pma[t].accept.push_back(lens.size());
    lens.push_back(strlen(str));
    //strs.push_back(str);
  }

  void BuildPMA() {
    assert(!build);
    queue<int> que;  // make failure link using bfs
    for (int c = 1; c < SIZE; c++) {
      if (pma[0].next[c] != -1) {
        pma[pma[0].next[c]].next[0] = 0;
        que.push(pma[0].next[c]);
      } else {
        pma[0].next[c] = 0;
      }
    }
    while (!que.empty()) {
      int t = que.front();
      que.pop();
      for (int c = 1; c < SIZE; c++) {
        if (pma[t].next[c] != -1) {
          que.push(pma[t].next[c]);
          int r = pma[t].next[0];
          while (pma[r].next[c] == -1) {
            r = pma[r].next[0];
          }
          pma[pma[t].next[c]].next[0] = pma[r].next[c];
          for (vector<int>::iterator it = pma[pma[r].next[c]].accept.begin(); it != pma[pma[r].next[c]].accept.end(); it++) {
            pma[pma[t].next[c]].accept.push_back(*it);
          }
        }
      }
    }
    build = true;
  }

  int OneMove(int index, int c) {
    return pma[index].next[c] != -1 ?
      pma[index].next[c] :
      pma[index].next[c] = OneMove(pma[index].next[0], c);
    //while (pma[index].next[c] == -1) { index = pma[index].next[0]; }
    //return pma[index].next[c];
  }

  // return first match indices
  vector<int> Match(const char *t) {
    assert(build);
    int index = 0;
    vector<int> ret(lens.size(), -1);
    int n = strlen(t);
    for (int i = 0; i < n; i++) {
      int c = t[i];
      index = OneMove(index, c);
      for (vector<int>::const_iterator it = pma[index].accept.begin(); it != pma[index].accept.end(); it++) {
        if (ret[*it] != -1) { continue; }
        ret[*it] = i - lens[*it] + 1;
      }
    }
    return ret;
  }
};
