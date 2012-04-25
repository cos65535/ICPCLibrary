//==============================================================
// Suffix Array

struct SuffixArray {
  struct SAComp {
    const int h, *g;
    SAComp(int h, int *g) : h(h), g(g) {;}
    bool operator() (int a, int b) {
      return a == b ? false : g[a] != g[b] ? g[a] < g[b] : g[a + h] < g[b + h];
    }
  };

  static const int SIZE = 100000;
  int n;
  char str[SIZE];
  vector<int> sa;
  vector<int> lcp;

  SuffixArray() {;}
  SuffixArray(char *t) {
    strcpy(str, t);
    Init();
  }
  void Init() {
    n = strlen(str);
    buildSA();
    buildLCP();
  }

  // O(mlogn)
  int find(char *p) {
    int m = strlen(p);
    int left = 0;
    int right = n;
    while (left < right) {
      int mid = (left + right) / 2;
      if (strncmp(str + sa[mid], p, m) < 0) { left = mid + 1; }
      else { right = mid; }
    }
    return strncmp(str + sa[left], p, m) == 0 ? sa[left] : -1;
  }
private:
  //O(nlog^2n)
  void buildSA() {
    sa = vector<int>(n + 1);
    int g[n + 1];
    int b[n + 1];
    REP(i, n + 1) { sa[i] = i; g[i] = str[i]; }
    b[0] = 0;
    b[n] = 0;

    sort(sa.begin(), sa.end(), SAComp(0, g));
    for (int h = 1; b[n] != n; h *= 2) {
      SAComp comp(h, g);
      sort(sa.begin(), sa.end(), comp);
      REP(i, n) b[i + 1] = b[i] + comp(sa[i], sa[i + 1]);
      REP(i, n + 1) g[sa[i]] = b[i];
    }
  }
  void buildLCP() {
    lcp = vector<int>(n + 1);
    int h = 0;
    int b[n + 1];
    REP(i, n + 1) { b[sa[i]] = i; }
    REP(i, n + 1) {
      if (b[i]) {
        for (int j = sa[b[i] - 1]; j + h < n && i + h < n && str[j + h] == str[i + h]; h++) {;}
        lcp[b[i]] = h;
      } else {
        lcp[b[i]] = -1;
      }
      if (h > 0) { h--; }
    }
  }
};

//===============================================================-
// Aho Corasick

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
  //vector<string> strs;

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

//================================================================
// Longest Palindrome

// rad[i] is longest palindrome radius if center is (i+1)/2
vector<int> LongestPalindrome(const char *str) {
  int n = strlen(str);
  vector<int> rad(2 * n); 
  int k;
  for (int i = 0, j = 0; i < 2 * n; i += k, j = max(j - k, 0)) {
    while (i - j >= 0 && i + j + 1 < 2 * n && str[(i - j) / 2] == str[(i + j + 1) / 2]) { j++; }
    rad[i] = j;
    for (k = 1; i - k >= 0 && rad[i] - k >= 0 && rad[i - k] != rad[i] - k; k++) {
      rad[i + k] = min(rad[i - k], rad[i] - k); 
    }   
  }
  for (int i = 0; i < 2 * n; i++) { rad[i] = (rad[i] + 1) / 2; }
  return rad;
}
