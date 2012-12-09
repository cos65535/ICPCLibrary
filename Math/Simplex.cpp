//max{cx | Ax <= b, x >= 0}
Array simplex(Matrix A, Array b, Array c) {
  int n = A.size();
  int m = A[0].size() + 1;
  int r = n;
  int s = m - 1;
  Matrix D(n + 2, Array(m + 1, 0.0));
  Array ix(n + m, 0.0);
  for (int i = 0; i < n + m; i++) { ix[i] = i; }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m - 1; j++) { D[i][j] = -A[i][j]; }
    D[i][m - 1] = 1;
    D[i][m] = b[i];
    if (D[r][m] > D[i][m]) { r = i; }
  }
  for (int j = 0; j < m - 1; j++) { D[n][j] = c[j]; }
  D[n + 1][m - 1] = -1;
  for (double d = 0.0;;) {
    if (r < n) {
      swap(ix[s], ix[r + m]);
      D[r][s] = 1.0 / D[r][s];
      for (int j = 0; j <= m; j++) { if (j != s) D[r][j] *= -D[r][s]; }
      for (int i = 0; i <= n + 1; i++) {
        if (i != r) {
          for (int j = 0; j <= m; j++) { if (j != s) { D[i][j] += D[r][j] * D[i][s]; } }
          D[i][s] *= D[r][s];
        }
      }
    }
    r = -1; s = -1;
    for (int j = 0; j < m; j++) {
      if (s < 0 || ix[s] > ix[j]) {
        if (D[n + 1][j] > EPS || D[n + 1][j] > -EPS && D[n][j] > EPS) { s = j; }
      }
    }
    if (s < 0) { break; }
    for (int i = 0; i < n; i++) {
      if (D[i][s] < -EPS) {
        if (r < 0 || (d = D[r][m] / D[r][s] - D[i][m] / D[i][s]) < -EPS
            || d < EPS && ix[r + m] > ix[i + m]) { r = i; }
      }
    }
    if (r < 0) { return Array(0); } //unbounded
  }
  if (D[n + 1][m] < -EPS) { return Array(0); } //unenforceability
  Array x(m - 1);
  for (int i = m; i < n + m; i++) {
    if (ix[i] < m - 1) { x[ix[i]] = D[i - m][m]; }
  }
  return x;  //value is D[n][m]
}
