template <class T>
struct Vector {
  vector<T> vect;
  Vector() {;}
  explicit Vector(int length) : vect(length) {;}
  Vector(T x, T y) : vect(2) { vect[0] = x; vect[1] = y; }
  Vector(T x, T y, T z) : vect(3) { vect[0] = x; vect[1] = y; vect[2] = z; }
  Vector(T x, T y, T z, T w) : vect(4) { vect[0] = x; vect[1] = y; vect[2] = z; vect[3] = w; }
  int size() const { return vect.size(); }

  T operator[](int index) const {
    assert(index >= 0 && index < size());
    return vect[index];
  }
  T &operator[](int index) {
    assert(index >= 0 && index < size());
    return vect[index];
  }

  Vector operator+(const Vector &rhs) const {
    assert(size() == rhs.size());
    Vector ret = *this;
    for (int i = 0; i < size(); i++) { ret.vect[i] += rhs.vect[i]; }
    return ret;
  }
  Vector operator-(const Vector &rhs) const {
    assert(size() == rhs.size());
    Vector ret = *this;
    for (int i = 0; i < size(); i++) { ret.vect[i] -= rhs.vect[i]; }
    return ret;
  }
  Vector operator+(const T &rhs) const {
    Vector ret = *this;
    for (int i = 0; i < size(); i++) { ret.vect[i] += rhs; }
    return ret;
  }
  Vector operator-(const T &rhs) const {
    Vector ret = *this;
    for (int i = 0; i < size(); i++) { ret.vect[i] -= rhs; }
    return ret;
  }
  Vector operator*(const T &rhs) const {
    Vector ret = *this;
    for (int i = 0; i < size(); i++) { ret.vect[i] *= rhs; }
    return ret;
  }
  Vector operator/(const T &rhs) const {
    Vector ret = *this;
    for (int i = 0; i < size(); i++) { ret.vect[i] /= rhs; }
    return ret;
  }
  Vector operator%(const T &rhs) const {
    Vector ret = *this;
    for (int i = 0; i < size(); i++) { ret.vect[i] %= rhs; }
    return ret;
  }
  Vector &operator+=(const Vector &rhs) { return *this = *this + rhs; }
  Vector &operator-=(const Vector &rhs) { return *this = *this - rhs; }
  Vector &operator+=(const T &rhs) { return *this = *this + rhs; }
  Vector &operator-=(const T &rhs) { return *this = *this - rhs; }
  Vector &operator*=(const T &rhs) { return *this = *this * rhs; }
  Vector &operator/=(const T &rhs) { return *this = *this / rhs; }
  Vector &operator%=(const T &rhs) { return *this = *this % rhs; }
  Vector operator-() const { return *this * -1; }

  T X() const { assert(size() >= 1); return vect[0]; }
  T Y() const { assert(size() >= 2); return vect[1]; }
  T Z() const { assert(size() >= 3); return vect[2]; }
  T W() const { assert(size() >= 4); return vect[3]; }
  T &X() { assert(size() >= 1); return vect[0]; }
  T &Y() { assert(size() >= 2); return vect[1]; }
  T &Z() { assert(size() >= 3); return vect[2]; }
  T &W() { assert(size() >= 4); return vect[3]; }

  T dot(const Vector &rhs) const {
    assert(size() == rhs.size());
    T ret = 0;
    for (int i = 0; i < size(); i++) { ret += vect[i] * rhs.vect[i]; }
    return ret;
  }
  Vector cross(const Vector &rhs) const {
    assert(size() == 3);
    assert(rhs.size() == 3);
    Vector ret = Vector(3);
    ret.X() = this->Y() * rhs.Z() - this->Z() * rhs.Y();
    ret.Y() = this->Z() * rhs.X() - this->X() * rhs.Z();
    ret.Z() = this->X() * rhs.Y() - this->Y() * rhs.X();
    return ret;
  }
  T norm() const { return dot(*this); }
  double abs() const { return sqrt(norm()); }
  Vector &normalize() { return *this/ abs(*this); }

  void Print() const {
    for (int i = 0; i < size(); i++) { cout << vect[i] << " "; }
    cout << endl;
  }
};
template<class T>
ostream &operator<<(ostream &os, const Vector<T> &rhs) {
  for (int i = 0; i < rhs.size(); i++) { os << rhs.vect[i] << " "; }
  //os << endl;
  return os;
}
template<class T>
T dot(const Vector<T> &lhs, const Vector<T> &rhs) {
  return lhs.dot(rhs);
}
template<class T>
Vector<T> cross(const Vector<T> &lhs, const Vector<T> &rhs) {
  return lhs.cross(rhs);
}


template <class T>
struct Matrix {
  vector<Vector<T> > vects;
  Matrix() {;}
  Matrix(int height, int width) : vects(height, Vector<T>(width)) {;}
  Matrix(const Matrix &A, const Matrix &B, const Matrix &C, const Matrix &D) {
    assert(A.width() == C.width());
    assert(A.height() == B.height());
    assert(B.width() == D.width());
    assert(C.height() == D.height());
    vects = vector<Vector<T> >(A.height() + C.height(), Vector<T>(A.width() + B.width()));
    SetMatrix(0, 0, A);
    SetMatrix(0, A.width(), B);
    SetMatrix(A.height(), 0, C);
    SetMatrix(A.height(), A.width(), D);
  }
  static Matrix I(int s) {
    Matrix ret(s, s);
    for (int i = 0; i < s; i++) { ret[i][i] = 1; }
    return ret;
  }
  int size() const { return vects.size(); }
  int height() const { return size(); }
  int width() const { return vects[0].size(); }
  void SetMatrix(int sy, int sx, const Matrix &matrix) {
    assert(sx >= 0 && sx + matrix.width() <= width());
    assert(sy >= 0 && sy + matrix.height() <= height());
    for (int y = 0; y < matrix.height(); y++) {
      for (int x = 0; x < matrix.width(); x++) {
        vects[sy + y][sx + x] = matrix.vects[y][x];
      }
    }
  }
  Matrix GetMatrix(int sy, int sx, int w, int h) const {
    assert(sx >= 0 && sx + w <= width());
    assert(sy >= 0 && sy + h <= height());
    Matrix ret(h, w);
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        ret.vects[y][x] = vects[sy + y][sx + x];
      }
    }
    return ret;
  }

  Vector<T> operator[](int index) const {
    assert(index >= 0 && index < size());
    return vects[index];
  }
  Vector<T> &operator[](int index) {
    assert(index >= 0 && index < size());
    return vects[index];
  }

  Vector<T> operator*(const Vector<T> &rhs) const {
    assert(size() == rhs.size());
    Vector<T> ret(size());
    for (int y = 0; y < size(); y++) { ret[y] = dot(vects[y], rhs); }
    return ret;
  }
  Matrix operator+(const Matrix &rhs) const {
    Matrix ret = *this;
    for (int i = 0; i < ret.size(); i++) { ret.vects[i] += rhs.vects[i]; }
    return ret;
  }
  Matrix operator-(const Matrix &rhs) const {
    Matrix ret = *this;
    for (int i = 0; i < ret.size(); i++) { ret.vects[i] -= rhs.vects[i]; }
    return ret;
  }
  Matrix operator*(const Matrix &rhs) const {
    assert(width() == rhs.height());
    Matrix ret(height(), rhs.width());
    for (int i = 0; i < height(); i++) {
      for (int k = 0; k < width(); k++) {
        for (int j = 0; j < rhs.width(); j++) {
          T prev = ret[i][j];
          T mul = vects[i][k] * rhs.vects[k][j];
          assert("overflow!" && (rhs.vects[k][j] == 0 || mul / rhs.vects[k][j] == vects[i][k]));
          assert("overflow!" && mul > 0 ? prev + mul >= prev : prev + mul <= prev);
          ret[i][j] += vects[i][k] * rhs.vects[k][j];
        }
      }
    }
    return ret;
  }
  Matrix operator+(const T &rhs) const {
    Matrix ret = *this;
    for (int i = 0; i < size(); i++) { ret.vects[i] += rhs; }
    return ret;
  }
  Matrix operator-(const T &rhs) const {
    Matrix ret = *this;
    for (int i = 0; i < size(); i++) { ret.vects[i] -= rhs; }
    return ret;
  }
  Matrix operator*(const T  &rhs) const {
    Matrix ret = *this;
    for (int i = 0; i < size(); i++) { ret.vects[i] *= rhs; }
    return ret;
  }
  Matrix operator/(const T  &rhs) const {
    Matrix ret = *this;
    for (int i = 0; i < size(); i++) { ret.vects[i] /= rhs; }
    return ret;
  }
  Matrix operator%(const T  &rhs) const {
    Matrix ret = *this;
    for (int i = 0; i < size(); i++) { ret.vects[i] %= rhs; }
    return ret;
  }
  Matrix &operator+=(const Matrix &rhs) { return *this = *this + rhs; }
  Matrix &operator-=(const Matrix &rhs) { return *this = *this - rhs; }
  Matrix &operator*=(const Matrix &rhs) { return *this = *this * rhs; }
  Matrix &operator+=(const T &rhs) { return *this = *this + rhs; }
  Matrix &operator-=(const T &rhs) { return *this = *this - rhs; }
  Matrix &operator*=(const T &rhs) { return *this = *this * rhs; }
  Matrix &operator/=(const T &rhs) { return *this = *this / rhs; }
  Matrix &operator%=(const T &rhs) { return *this = *this % rhs; }
  Matrix operator-() const { return *this * -1; }

  Vector<T> colum(int x) {
    assert(x >= 0 && x < width());
    Vector<T> ret(height());
    for (int y = 0; y < height(); y++) { ret[y] = vects[y][x]; }
    return ret;
  }
  T trace() const {
    T ret = 0;
    for (int i = 0; i < size(); i++) { ret += vects[i][i]; }
    return ret;
  }
  Matrix transpose() const {
    Matrix ret(width(), height());
    for (int y = 0; y < height(); y++) {
      for (int x = 0; x < width(); y++) {
        ret[x][y] = vects[y][x];
      }
    }
    return ret;
  }

  Matrix adjoint() const
  {
    assert(size() == 3);
    Matrix ret(3, 3);
    ret[0] = cross(vects[1], vects[2]);
    ret[1] = cross(vects[2], vects[0]);
    ret[2] = cross(vects[0], vects[1]);
    return ret;
  }
  Matrix inverse() const {
    assert(size() == 3);
    Matrix ret(3, 3);
    Matrix A = adjoint();
    T d = dot(A[0], vects[0]);
    if (d == 0.0) { return ret; }
    return A.transpose() / d;
  }
  T det() const {
    assert(size() == 3);
    return dot(vects[0], cross(vects[1], vects[2]));
  }
  void Print() {
    for (int i = 0; i < size(); i++) { vects[i].Print(); }
  }
};
template <class T>
ostream &operator<<(ostream &os, const Matrix<T> &rhs) {
  for (int i = 0; i < rhs.size(); i++) {
    os << rhs.vects[i];
    if (i != rhs.size() - 1) { os << endl; }
  }
  return os;
}

Matrix<ll> modmul(const Matrix<ll> &lhs, const Matrix<ll> &rhs, const ll mod) {
  Matrix<ll> ret(lhs.height(), rhs.width());
  for (int i = 0; i < lhs.height(); i++) {
    for (int k = 0; k < lhs.width(); k++) {
      for (int j = 0; j < rhs.width(); j++) {
        ll prev = ret[i][j];
        ll mul = lhs.vects[i][k] * rhs.vects[k][j];
        assert("overflow!" && (rhs.vects[k][j] == 0 || mul / rhs.vects[k][j] == lhs.vects[i][k]));
        assert("overflow!" && mul > 0 ? prev + mul >= prev : prev + mul <= prev);
        ret[i][j] = (ret[i][j] + lhs[i][k] * rhs[k][j] % mod) % mod;
        assert(ret[i][j] >= 0 && ret[i][j] < mod);
      }
    }
  }
  return ret;
}

Matrix<ll> powmod(Matrix<ll> base, ll power, ll mod) {
  Matrix<ll> ans(base.size(), base.size());
  REP(i, base.size()) { ans[i][i] = 1; }
  while (power > 0) {
    if (power & 1) { ans = modmul(ans, base, mod); }
    power >>= 1;
    base = modmul(base, base, mod);
  }
  return ans;
}
