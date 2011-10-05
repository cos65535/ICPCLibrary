#include <iomanip>
#include <complex>
typedef complex<double> Complex;

struct Long {
  static const long long BASE = 10000;
  static const int BW = 4;
  vector<long long> digits;
  Long(long long a = 0) {
    digits.push_back(a);
    Normalize();
  }
  void resize(int s) { digits.resize(s); }
  int size() const { return digits.size(); }
  int sign() const { return digits[digits.size() - 1] > 0 ? 1 : (digits[digits.size() - 1] < 0 ? -1 : 0); }
  Long abs() const {
    Long ret = *this;
    return ret.sign() >= 0 ? ret : -ret;;
  }
  long long operator[](int index) const { return digits[index]; }
  long long &operator[](int index) { return digits[index]; }
  bool operator<(const Long &rhs) const {
    const Long &lhs = *this;
    if (lhs.sign() != rhs.sign()) { return lhs.sign() < rhs.sign(); }
    if (lhs.size() != rhs.size()) { return lhs.sign() > 0 ? lhs.size() < rhs.size() : lhs.size() > rhs.size(); }
    for (int i = lhs.size() - 1; i >= 0; i--) {
      if (lhs[i] != rhs[i]) { return lhs[i] < rhs[i]; }
    }
    return false;
  }
  bool operator>(const Long &rhs) const { return rhs < *this; }
  bool operator<=(const Long &rhs) const { return !(rhs < *this); }
  bool operator>=(const Long &rhs) const { return !(*this < rhs); }
  bool operator!=(const Long &rhs) const { return *this < rhs || rhs < *this; }
  bool operator==(const Long &rhs) const { return !(*this < rhs) && !(rhs < *this); }

  Long &Normalize() {
    int s = 1;
    while (digits[size() - 1] <= -BASE) {
      ll v = digits[size() - 1] / BASE;
      digits.push_back(v);
      digits[size() - 2] -= v * BASE;
    }     
    for (int i = 0; i < size(); i++) {
      if (i == size() - 1 && digits[i] < 0) {
        goto minus;
      } else if (digits[i] < 0) {
        long long a = -((digits[i] + 1) / BASE) + 1;
        digits[i] = a * BASE + digits[i];
        digits[i + 1] -= a;
      } else if (digits[i] >= BASE) {
        if (i == size() - 1) { digits.push_back(0); }
        long long a = digits[i] / BASE;
        digits[i] = digits[i] % BASE;
        digits[i + 1] += a;
      }
      assert(0 <= digits[i] && digits[i] < BASE);
      if (digits[i] != 0) { s = i + 1; }
    }
    assert(0 <= digits[s - 1] && digits[s - 1] < BASE);
    assert(s == 1 || digits[s - 1] != 0);
    resize(s);
    return *this;
minus:
    s = 1;
    for (int i = size() - 2; i >= 0; i--) {
      digits[i + 1] += 1;
      digits[i] -= BASE;
      if (digits[i + 1] != 0) { s = max(s, i + 2); }
      assert(digits[i + 1] <= 0);
      assert(digits[i] <= 0);
    }
    resize(s);
    return *this;
  }
  Long &convert(const string &str) {
    if (str.length() == 0) { return *this; }
    int s = str[0] != '-' ? 1 : -1;
    int sw = s == -1 ? 1 : 0;
    digits = vector<long long>((str.size() - 1 - sw) / BW + 1);
    int i = (str.size() - sw) % BW;
    int index = digits.size() - 1;
    if (i > 0) { i -= BW; }
    for (; i < (int)str.size() - sw; i+= BW) {
      long long a = 0;
      for (int j = 0; j < BW; j++) {
        a = 10 * a + (i + j + sw >= sw ? str[i + j + sw] - '0' : 0);
      }
      digits[index--] = a * s;
    }
    return Normalize();
  }
  void Print() {
    int s = sign();
    printf("%lld", digits[size() - 1]);
    for (int i = size() - 2; i >= 0; i--) {
      assert(BW == 4);
      printf("%04lld", digits[i] * s);
    }
  }
  Long operator+(const Long &rhs) const {
    Long ret = *this;
    if (ret.size() < rhs.size()) { ret.resize(rhs.size()); }
    for (int i = 0; i < rhs.size(); i++) { ret[i] += rhs[i]; }
    return ret.Normalize();
  }
  Long operator-(const Long &rhs) const {
    //assert(*this >= rhs);
    Long ret = *this;
    if (ret.size() < rhs.size()) { ret.resize(rhs.size()); }
    for (int i = 0; i < rhs.size(); i++) { ret[i] -= rhs[i]; }
    return ret.Normalize();
  }
  Long operator*(const Long &rhs) const {
    const Long &lhs = *this;
    Long ret;
    ret.resize(this->size() + rhs.size());
    for (int i = 0; i < lhs.size(); i++) {
      for (int j = 0; j < rhs.size(); j++) {
        ret[i + j] += lhs[i] * rhs[j];
      }
    }
    return ret.Normalize();
  }
  Long operator/(const Long &rhs) const {
    return this->divmod(rhs).first;
  }
  Long operator%(const Long &rhs) const {
    return this->divmod(rhs).second;
  }
  Long operator-() const {
    return *this * -1;
  }
  Long &operator+=(const Long &rhs) {
    return *this = *this + rhs;
  }
  Long &operator-=(const Long &rhs) {
    return *this = *this - rhs;
  }
  Long &operator*=(const Long &rhs) {
    return *this = *this * rhs;
  }
  Long &operator/=(const Long &rhs) {
    return *this = *this / rhs;
  }
  Long &operator%=(const Long &rhs) {
    return *this = *this % rhs;
  }
  pair<Long, long long> divmodll(long long rhs) const {
    Long ret = *this;
    long long c = 0;
    long long t;
    for (int i = ret.size() - 1; i >= 0; i--) {
      t = BASE * c + ret[i];
      ret[i] = t / rhs;
      c = t % rhs;
    }
    return make_pair(ret.Normalize(), c);
  }
  pair<Long, Long> divmod(Long rhs) const {
    Long lhs = *this;
    if (lhs.size() < rhs.size()) { return pair<Long, Long>(0, lhs); }
    int s = lhs.sign() * rhs.sign();
    lhs = lhs.abs(); rhs = rhs.abs();
    long long F = BASE / (rhs[rhs.size() - 1] + 1);  // multiplying good-factor
    lhs = lhs * F; rhs = rhs * F;
    Long ret;
    ret.resize(lhs.size() - rhs.size() + 1);
    for (int k = ret.size() - 1, i = lhs.size() - 1; k >= 0; k--, i--) {
      ret[k] = (i + 1 < lhs.size() ? lhs[i + 1] : 0) * BASE + lhs[i];
      ret[k] /= rhs[rhs.size() - 1];
      Long t;
      t.resize(k + rhs.size());
      for (int m = 0; m < rhs.size(); m++) {
        t[k + m] = ret[k] * rhs[m];
      }
      t.Normalize();
      while (lhs < t) {
        ret[k] -= 1;
        for (int m = 0; m < rhs.size(); m++) {
          t[k + m] -= rhs[m];
        }
        t.Normalize();
      }
      lhs = lhs - t;
    }
    return make_pair(ret.Normalize() * s, lhs.divmodll(F).first * s);
  }
};
ostream &operator<<(ostream &os, const Long &rhs) {
  int s = rhs.sign();
  os << rhs[rhs.size() - 1];
  for (int i = rhs.size() - 2; i >= 0; i--) {
    os << setw(Long::BW) << setfill('0') << rhs[i] * s;
  }
  return os;
}
istream &operator>>(istream &is, Long &rhs) {
  string s;
  is >> s;
  rhs.convert(s);
  return is;
}
pair<Long, long long> divmodll(const Long &lhs, long long &rhs) {
  return lhs.divmodll(rhs);
}
pair<Long, Long> divmod(const Long &lhs, const Long &rhs) {
  return lhs.divmod(rhs);
}
