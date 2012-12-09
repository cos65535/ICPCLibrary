vector<double> GaussElimination(vector<vector<double> > matrix, vector<double> vect) {
  const int n = vect.size();
  vector<double> ret(n, 0.0);
  for (int x = 0; x < n; x++) {
    int pivot = x;
    for (int i = x + 1; i < n; i++) {
      if (fabs(matrix[i][x]) - fabs(matrix[pivot][x]) > EPS) {
        pivot = i;
      }
    }
    swap(matrix[x], matrix[pivot]);
    swap(vect[x], vect[pivot]);
    if (fabs(matrix[x][x]) < EPS) { continue; }
    for (int y = x + 1; y < n; y++) {
      double ratio = -matrix[y][x] /  matrix[x][x];
      matrix[y][x] = 0.0;
      for (int i = x + 1; i < n; i++) {
        matrix[y][i] += matrix[x][i] * ratio;
      }
      vect[y] += vect[x] * ratio;
    }
  }
  for (int x = n - 1; x >= 0; x--) {
    double sum = vect[x];
    for (int i = n - 1; i > x; i--) {
      sum -= ret[i] * matrix[x][i];
    }
    ret[x] = sum / matrix[x][x];
  }
  return ret;
}
