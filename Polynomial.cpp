template <class T>
struct Polynomial {
  vector<T> coefficient;
  Polynomial() : coefficient(1, 1) {;}
  Polynomial(int s, T ini) : coefficient(s, ini) {;}
  int size() const { return coefficient.size(); }
  void normalize() {
    int s = 1;
    for (int i = size() - 1; i >= 0; i--) {
      if (coefficient[i] != 0) { s = i + 1; break; }
    }
    coefficient.resize(s);
  }
  T operator[](int index) const {
    assert(index >= 0 && index < size());
    return coefficient[index];
  }
  T &operator[](int index) {
    assert(index >= 0 && index < size());
    return coefficient[index];
  }
  Polynomial operator+(const Polynomial &rhs) const {
    Polynomial ret = *this;
    ret.coefficient.resize(max(size(), rhs.size()));
    for (int i = 0; i < rhs.size(); i++) {
      ret[i] += rhs[i];
    }
    ret.normalize();
    return ret;
  }
  Polynomial operator-(const Polynomial &rhs) const {
    Polynomial ret = *this;
    ret.coefficient.resize(max(size(), rhs.size()));
    for (int i = 0; i < rhs.size(); i++) {
      ret[i] -= rhs[i];
    }
    ret.normalize();
    return ret;
  }
  Polynomial operator*(const Polynomial &rhs) const {
    Polynomial ret(size() + rhs.size(), 0);
    for (int i = 0; i < size(); i++) {
      for (int j = 0; j < rhs.size(); j++) {
        ret[i + j] += coefficient[i] * rhs[j];
      }
    }
    ret.normalize();
    return ret;
  }
  Polynomial operator*(const T &rhs) const {
    Polynomial ret = *this;
    for (int i = 0; i < size(); i++) {
      ret[i] *= rhs;
    }
    ret.normalize();
    return ret;
  }
  Polynomial operator/(const Polynomial &rhs) const {
    return divmod(rhs).first;
  }
  Polynomial operator%(const Polynomial &rhs) const {
    return divmod(rhs).second;
  }
  Polynomial operator/(const T &rhs) const {
    assert(rhs != 0);
    return *this * (1.0 / rhs);
  }
  Polynomial operator+=(const Polynomial &rhs) {
    return *this = *this + rhs;
  }
  Polynomial operator-=(const Polynomial &rhs) {
    return *this = *this - rhs;
  }
  Polynomial operator*=(const Polynomial &rhs) {
    return *this = *this * rhs;
  }
  Polynomial operator/=(const Polynomial &rhs) {
    return *this = *this / rhs;
  }
  Polynomial operator%=(const Polynomial &rhs) {
    return *this = *this % rhs;
  }
  Polynomial operator*=(const T &rhs) {
    return *this = *this * rhs;
  }
  Polynomial operator/=(const T &rhs) {
    return *this = *this / rhs;
  }
  Polynomial operator-() const {
    return *this * Polynomial(1, -1);
  }
  pair<Polynomial, Polynomial> divmod(const Polynomial &rhs) const {
    int ls = size();
    int rs = rhs.size();
    int s = ls - rs + 1;
    if (s < 0) { return make_pair(Polynomial(1, 0), *this); }
    Polynomial div(s, 0);
    Polynomial rest = *this;
    assert(rhs[rs - 1] != 0);
    for (int i = 0; i < s; i++) {
      T d = rest[ls - i - 1] / rhs[rs - 1];
      div[s - i - 1] = d;
      for (int j = 0; j < rs; j++) {
        rest[ls - i - j - 1] -= rhs[rs - j - 1] * d;
      }
    }
    div.normalize();
    rest.normalize();
    return make_pair(div, rest);
  }
  Polynomial pow(int power) const {
    Polynomial base = *this;
    Polynomial ret;
    while (power > 0) {
      if (power & 1) { ret *= base; }
      base *= base;
      power >>= 1;
    }
    return ret;
  }
  Polynomial differential() const {
    if (coefficient.size() == 1) { return Polynomial(1, 0); }
    Polynomial ret(coefficient.size() - 1, 0);
    FOR(i, 1, coefficient.size()) {
      ret[i - 1] = coefficient[i] * i;
    }
    return ret;
  }
  T calc(T x) const {
    T ans = 0;
    for (int i = coefficient.size() - 1; i >= 0; i--) {
      ans *= x;
      ans += coefficient[i];
    }
    return ans;
  }
  void Print() const {
    REP(i, coefficient.size()) { cout << coefficient[i] << " "; }
    cout << endl;
  }
};
template <class T>
ostream &operator<<(ostream &os, const Polynomial<T> &rhs) {
  for (int i = 0; i < rhs.size(); i++) { os << rhs.coefficient[i] << " "; }
  os << endl;
  return os;
}

template <class T>
Polynomial<T> PolynomialGCD(Polynomial<T> a, Polynomial<T> b) {
  if (b.size() == 1 && b[0] == 0) { return a; }
  return PolynomialGCD(b, a % b);
}

template <class T>
Polynomial<T> pow(const Polynomial<T> &base, int power) {
  return base.pow(power);
}

template <class T>
Polynomial<T> divmod(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
  return lhs.divmod(rhs);
}
