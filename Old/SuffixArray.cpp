struct SAComp {
  const int h, *g;
  SAComp(int h, int *g) : h(h), g(g) {;}
  bool operator() (int a, int b) {
    return a == b ? false : g[a] != g[b] ?  g[a] < g[b] : g[a + h] < g[b + h];
  }
};

vector<int> firstBuildSA(char *t, int n) {
  vector<int> si(n + 1);
  int is[n + 1];
  for (int i = 0; i <= n; i++) { si[i] = i; is[i] = t[i]; }
  sort(si.begin(), si.end(), SAComp(0, is));
  vector<int> a(n + 1);
  int b[n + 1];
  b[0] = 0;
  for (int h = 0; ; ) {
    for (int i = 0; i < n; i++) {
      int x = si[i + 1];
      int y = si[i];
      b[i + 1] = b[i];
      if (is[x] > is[y] || is[x + h] > is[y + h]) { b[i + 1]++; }
    }
    for (int i = 0; i <= n; i++) { is[si[i]] = b[i]; }
    if (b[n] == n) { break; }
    h = max(1, h << 1);
    for (int k = h; k >= 0; k -= h) {
      fill(b, b + n + 1, 0);
      b[0] = k;
      for (int i = k; i <= n; i++) { b[is[i]]++; }
      for (int i = 0; i < n; i++) { b[i + 1] += b[i]; }
      for (int i = n; i >= 0; i--) {
        a[--b[si[i] + k > n ? 0 : is[si[i] + k]]] = si[i];
      }
      swap(si, a);
    }
  }
  return si;
}

vector<int> buildSA(char *t, int n) {
  vector<int> v(n+1);
  int g[n + 1];
  int b[n + 1];
  REP(i, n + 1) { v[i] = i; g[i] = t[i]; }
  b[0] = 0;
  b[n] = 0;

  sort(v.begin(), v.end(), SAComp(0, g));
  for (int h = 1; b[n] != n; h *= 2) {
    SAComp comp(h, g);
    sort(v.begin(), v.end(), comp);
    REP(i, n) b[i + 1] = b[i] + comp(v[i], v[i + 1]);
    REP(i, n + 1) g[v[i]] = b[i];
  }
  return v;
}

int find(char *t, int n, char *p, int m, const vector<int> &sa) {
  int left = 0;
  int right = n;
  while (left < right) {
    int mid = (left + right) / 2;
    if (strncmp(t + sa[mid], p, m) < 0) { left = mid + 1; }
    else { right = mid; }
  }
  return strncmp(t + sa[left], p, m) == 0 ? sa[left] : -1;
}

vector<int> buildLCP(char *t, int n, const vector<int> &sa) {
  vector<int> lcp(n + 1);
  int h = 0;
  int b[n + 1];
  REP(i, n + 1) { b[sa[i]] = i; }
  REP(i, n + 1) {
    if (b[i]) {
      for (int j = sa[b[i] - 1]; j + h < n && i + h < n && t[j + h] == t[i + h]; h++) {;}
      lcp[b[i]] = h;
    } else {
      lcp[b[i]] = -1;
    }
    if (h > 0) { h--; }
  }
  return lcp;
}
