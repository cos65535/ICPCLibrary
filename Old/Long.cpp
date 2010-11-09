#include <iomanip>

static const long long BASE = 10000;
static const int  BW = 4;
static const int MAXDIGIT = 100;

struct Long {
  long long digit[MAXDIGIT];
  int size;
  explicit Long(int size = 1, long long a = 0) : size(size) {
    memset(digit, 0, sizeof(digit));
    digit[0] = a;
  }
};
const Long ZERO(1, 0), ONE(1, 1);

// Comparators
bool operator<(const Long &x, const Long &y) {
  if (x.size != y.size) { return x.size < y.size; }
  for (int i = x.size - 1; i >= 0; --i) {
    if (x.digit[i] != y.digit[i]) { return x.digit[i] < y.digit[i]; }
  }
  return false;
}
bool operator> (const Long &x, const Long &y) { return y < x; }
bool operator<=(const Long &x, const Long &y) { return !(y < x); }
bool operator>=(const Long &x, const Long &y) { return !(x < y); }
bool operator!=(const Long &x, const Long &y) { return x < y || y < x; }
bool operator==(const Long &x, const Long &y) { return !(x < y) && !(y < x); }

// Utilities
Long normal(Long x) {
  long long c = 0;
  for (int i = 0; i < x.size; i++) {
    while (x.digit[i] < 0) { x.digit[i + 1] -= 1; x.digit[i] += BASE; }
    long long a = x.digit[i] + c;
    x.digit[i] = a % BASE;
    c = a / BASE;
  }
  for (; c > 0; c /= BASE) { x.digit[x.size++] = c % BASE; }
  while (x.size > 1 && x.digit[x.size - 1] == 0) { --x.size; }
  return x;
}

Long convert(long long a) {
  return normal(Long(1, a));
}

Long convert(const string &s) {
  Long x;
  int i = s.size() % BW;
  if (i > 0) i -= BW;
  for (; i < (int)s.size(); i += BW) {
    long long a = 0;
    for (int j = 0; j < BW; j++) {
      a = 10 * a + (i + j >= 0 ? s[i + j] - '0' : 0);
    }
    x.digit[x.size++] = a;
  }
  reverse(x.digit, x.digit + x.size);
  return normal(x);
}
// Input/Output
ostream &operator<<(ostream &os, const Long &x) {
  os << x.digit[x.size - 1];
  for (int i = x.size - 2; i >= 0; i--) {
    os << setw(BW) << setfill('0') << x.digit[i];
  }
  return os;
}
void print(const Long &x) {
  printf("%lld", x.digit[x.size - 1]);
  for (int i = x.size - 2; i >= 0; --i) {
    printf("%04lld", x.digit[i]);
  }
}
istream &operator>>(istream &is, Long &x) {
  string s;
  is >> s;
  x = convert(s);
  return is;
}

// Basic Operations
Long operator+(Long x, const Long &y) {
  if (x.size < y.size) { x.size = y.size; }
  for (int i = 0; i < y.size; i++) { x.digit[i] += y.digit[i]; }
  return normal(x);
}

Long operator-(Long x, const Long &y) {
  assert(x >= y);
  for (int i = 0; i < y.size; i++) { x.digit[i] -= y.digit[i]; }
  return normal(x);
}

Long operator*(const Long &x, const Long &y) {
  Long z(x.size + y.size);
  for (int i = 0; i < x.size; i++) {
    for (int j = 0; j < y.size; j++) {
      z.digit[i+j] += x.digit[i] * y.digit[j];
    }
  }
  return normal(z);
}

Long operator*(Long x, long long a) {
  for (int i = 0; i < x.size; i++) { x.digit[i] *= a; }
  return normal(x);
}

pair<Long, long long> divmod(Long x, const long long &a) {
  long long c = 0, t;
  for (int i = x.size - 1; i>= 0; --i) {
    t = BASE * c + x.digit[i];
    x.digit[i] = t / a;
    c = t % a;
  }
  return make_pair(normal(x), c);
}

Long operator/(const Long &x, const long long &a) {
  return divmod(x, a).first;
}
long long operator%(const Long &x, const long long &a) {
  return divmod(x, a).second;
}

pair<Long, Long> divmod(Long x, Long y) {
  if (x.size < y.size) { return pair<Long, Long>(ZERO, x); }
  int F = BASE / (y.digit[y.size - 1] + 1);  // multiplying good-factor
  x = x * F; y = y * F;
  Long z(x.size - y.size + 1);
  for (int k = z.size - 1, i = x.size - 1; k >= 0; k--, i--) {
    z.digit[k] = (i + 1 < x.size ? x.digit[i + 1] : 0) * BASE + x.digit[i];
    z.digit[k] /= y.digit[y.size - 1];
    Long t(k + y.size);
    for (int m = 0; m < y.size; m++) {
      t.digit[k + m] = z.digit[k] * y.digit[m];
    }
    t = normal(t);
    while (x < t) {
      z.digit[k] -= 1;
      for (int m = 0; m < y.size; m++) {
        t.digit[k + m] -= y.digit[m];
      }
      t = normal(t);
    }
    x = x - t;
  }
  return make_pair(normal(z), x / F);
}
Long operator/(Long x, Long y) {
  return divmod(x, y).first;
}
Long operator%(Long x, Long y) {
  return divmod(x, y).second;
}
// Advanced Operations
Long shift(Long x, int k) {
  if (x.size == 1 && x.digit[0] == 0) { return x; }
  x.size += k;
  for (int i = x.size - 1; i >= k; i--) { x.digit[i] = x.digit[i - k]; }
  for (int i = k - 1; i >= 0; i--) { x.digit[i] = 0; }
  return x;
}

Long sqrt(Long x) {
  const Long _20 = convert(2 * BASE);
  Long odd = ZERO;
  Long rem(2, 0);
  Long ans = ZERO;
  for (int i = 2 * ((x.size - 1) / 2); i >= 0; i -= 2) {
    int group = (i + 1 < x.size ? x.digit[i + 1] : 0) * BASE + x.digit[i];
    odd = _20 * ans + ONE;
    rem = shift(rem, 2) + convert(group);
    int count = 0;
    while (rem >= odd) {
      count = count + 1;
      rem = rem - odd;
      odd.digit[0] += 2;
      odd = normal(odd);
    }
    ans = shift(ans, 1) + convert(count);
  }
  return ans;
}
