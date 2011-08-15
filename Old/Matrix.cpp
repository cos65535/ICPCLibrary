typedef double Weight;
typedef vector<Weight> Array;
typedef vector<Array> Matrix;

Weight dot(const Array &lhs, const Array &rhs) {
  const int h = lhs.size();
  assert((int)rhs.size() == h);
  Weight ret = 0;
  for (int y = 0; y < h; y++) {
    ret += lhs[y] * rhs[y];
  }
  return ret;
}

Array plus(const Array &lhs, const Array &rhs) {
  const int h = lhs.size();
  assert((int)rhs.size() == h);
  Array ret(h, 0);
  for (int y = 0; y < h; y++) {
    ret[y] = lhs[y] + rhs[y];
  }
  return ret;
}

Matrix plus(const Matrix &lhs, const Matrix &rhs) {
  const int h = lhs.size();
  const int w = lhs[0].size();
  assert((int)rhs.size() == h);
  assert((int)rhs[0].size() == w);
  Matrix ret(h, Array(w, 0));
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      ret[y][x] = lhs[y][x] + rhs[y][x];
    }
  }
  return ret;
}

Array mul(const Matrix &matrix, const Array &vect) {
  const int w = matrix[0].size();
  const int h = matrix.size();
  assert((int)vect.size() == w);
  Array ret(h, 0);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      ret[y] += matrix[y][x] * vect[x];
    }
  }
  return ret;
}

Matrix mul(const Matrix &lhs, const Matrix &rhs) {
  const int in = lhs[0].size();
  const int h = lhs.size();
  const int w = rhs[0].size();
  assert((int)rhs.size() == in);
  Matrix ret(h, Array(w, 0));
  for (int y = 0; y < h; y++) {
    for (int i = 0; i < in; i++) {
      for (int x = 0; x < w; x++) {
        ret[y][x] += lhs[y][i] * rhs[i][x];
      }
    }
  }
  return ret;
}

Matrix pow(Matrix base, long long power) {
  const int h = base.size();
  const int w = base[0].size();
  assert(h == w);
  Matrix ret(h, Array(w, 0));
  for (int i = 0; i < h; i++) {
    ret[i][i] = 1;
  }
  while (power > 0) {
    if (power & 1) {
      ret = mul(ret, base);
    }
    base = mul(base, base);
    power >>= 1;
  }
  return ret;
}

Array normalize(const Array vect) {
  const int n = vect.size();
  double sum = 0;
  Array ret(n, 0);
  for (int i = 0; i < n; i++) {
    sum += vect[i] * vect[i];
  }
  sum = sqrt(sum);
  for (int i = 0; i < n; i++) {
    ret[i] = vect[i] / sum;
  }
  return ret;
}
