struct Rational {
  long long p, q;
  Rational(long long p = 0, long long q = 1) : p(p), q(q) {
    normalize();
  }
  long long gcd(long long a, long long b) {
    if (a <= 0) { return b; }
    return gcd(b % a, a);
  }
  void normalize() {
    if (q < 0) { p *= -1; q *= -1; }
    long long d = gcd(p > 0 ? p : -p, q);
    if (d == 0) {
      p = 0;
      q = 1;
    } else {
      p /= d;
      q /= d;
    }
  }
  Rational &operator-() {
    p *= -1;
    return *this;
  }
  Rational operator-() const {
    return Rational(-p, q);
  }
  Rational operator+(const Rational &rhs) const {
    return Rational(p * rhs.q + rhs.p * q, q * rhs.q);
  }
  Rational operator-(const Rational &rhs) const {
    return Rational(p * rhs.q - rhs.p * q, q * rhs.q);
  }
  Rational operator*(const Rational &rhs) const {
    return Rational(p * rhs.p, q * rhs.q);
  }
  Rational operator/(const Rational &rhs) const {
    return Rational(p * rhs.q, q * rhs.p);
  }
  Rational &operator+=(const Rational &rhs) { *this = *this + rhs; return *this; }
  Rational &operator-=(const Rational &rhs) { *this = *this - rhs; return *this; }
  Rational &operator*=(const Rational &rhs) { *this = *this * rhs; return *this; }
  Rational &operator/=(const Rational &rhs) { *this = *this / rhs; return *this; }
  bool operator<(const Rational &rhs) const {
    return p * rhs.q < rhs.p * q;
  }
  bool operator>(const Rational &rhs) const {
    return rhs < *this;
  }
  bool operator<=(const Rational &rhs) const {
    return !(*this > rhs);
  }
  bool operator>=(const Rational &rhs) const {
    return !(*this < rhs);
  }
  bool operator!=(const Rational &rhs) const {
    return (*this < rhs || *this > rhs);
  }
  bool operator==(const Rational &rhs) const {
    return !(*this != rhs);
  }
};
ostream &operator<<(ostream &os, const Rational &rhs) {
  os << '(' << rhs.p << "/" << rhs.q << ')';
  //os << (double)rhs.p / rhs.q;
  return os;
}
