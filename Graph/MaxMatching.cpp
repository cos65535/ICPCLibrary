// It is no checked!
// But it solved AOJ 2347 (Sunny Graph)

// calc rank
int GaussElimination(vector<vector<double> > matrix) {
  const int n = matrix.size();
  int ret = 0;
  for (int x = 0; x < n; x++) {
    int pivot = x;
    for (int i = x + 1; i < n; i++) {
      if (fabs(matrix[i][x]) - fabs(matrix[pivot][x]) > EPS) {
        pivot = i;
      }
    }
    swap(matrix[x], matrix[pivot]);
    if (fabs(matrix[x][x]) < EPS) { continue; }
    for (int y = 0; y < n; y++) {
      if (y == x) { continue; }
      double ratio = -matrix[y][x] /  matrix[x][x];
      matrix[y][x] = 0.0;
      for (int i = x + 1; i < n; i++) {
        matrix[y][i] += matrix[x][i] * ratio;
      }
    }
    ret++;
  }
  return ret / 2;
}

const int ITER_CNT = 10;
int MaxMatching(const Graph &g) {
  const int n = g.size();
  int ret = 0;
  for (int iter = 0; iter < ITER_CNT; iter++) {
    vector<vector<double> > tutte(n, vector<double>(n, 0));
    for (int from = 0; from < n; from++) {
      for (Edges::const_iterator it = g[from].begin(); it != g[from].end(); it++) {
        int to = it->dest;
        if (to <= from) { continue; }
        double r = (double)(rand() + 1) / RAND_MAX;
        tutte[from][to] = r;
        tutte[to][from] = -r;
      }
    }
    ret = max(ret, GaussElimination(tutte));
  }
  return ret;
}
