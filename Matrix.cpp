typedef double Weight;
typedef vector<Weight> Array;
typedef vector<Array> Matrix;

Weight dot(const Array &array1, const Array &array2) {
  const int h = array1.size();
  assert((int)array2.size() == h);
  Weight ret = 0;
  for (int y = 0; y < h; y++) {
    ret += array1[y] * array2[y];
  }
  return ret;
}

Array plus(const Array &array1, const Array &array2) {
  const int h = array1.size();
  assert((int)array2.size() == h);
  Array ret(h, 0);
  for (int y = 0; y < h; y++) {
    ret[y] = array1[y] + array2[y];
  }
  return ret;
}

Matrix plus(const Matrix &matrix1, const Matrix &matrix2) {
  const int h = matrix1.size();
  const int w = matrix1[0].size();
  assert((int)matrix2.size() == h);
  assert((int)matrix2[0].size() == w);
  Matrix ret(h, Array(w, 0));
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      ret[y][x] = matrix1[y][x] + matrix2[y][x];
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

Matrix mul(const Matrix &matrix1, const Matrix &matrix2) {
  const int in = matrix1[0].size();
  const int h = matrix1.size();
  const int w = matrix2[0].size();
  assert((int)matrix2.size() == in);
  Matrix ret(h, Array(w, 0));
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      for (int i = 0; i < in; i++) {
        ret[y][x] += matrix1[y][i] * matrix2[i][x];
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
