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
