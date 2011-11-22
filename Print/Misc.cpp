//================================================================
// FFT & Convolution

#include <complex>
typedef complex<double> Complex;
// a.size must 2^x
vector<Complex> FFT(double theta, const vector<Complex> &a) {
  int n = a.size();
  vector<Complex> ret = a;
  for (int m = n; m >= 2; m >>= 1) {
    int mh = m >> 1;
    for (int i = 0; i < mh; i++) {
      Complex w = exp(i * theta * Complex(0, 1));
      for (int j = i; j < n; j += m) {
        int k = j + mh;
        Complex x = ret[j] - ret[k];
        ret[j] += ret[k];
        ret[k] = w * x;
      }
    }
    theta *= 2;
  }
  int i = 0;
  for (int j = 1; j < n - 1; j++) {
    for (int k = n >> 1; k > (i ^= k); k >>= 1) {;}
    if (j < i) { swap(ret[i], ret[j]); }
  }
  return ret;
}

// ret[x] = sum_{i+j=x}(lhs[i]*rhs[j])
vector<ll> Convolution(const vector<ll> &lhs, const vector<ll> &rhs) {
  int n = 1;
  while (n < (int)max(lhs.size(), rhs.size()) * 2) { n <<= 1; }
  vector<Complex> temp1(n);
  vector<Complex> temp2(n);
  for (int i = 0; i < n / 2; i++) {
    if (i < (int)lhs.size()) {
      temp1[i] = Complex(lhs[i], 0);
    }
    if (i < (int)rhs.size()) {
      temp2[i] = Complex(rhs[i], 0);
    }
  }
  temp1 = FFT(2.0 * PI / n, temp1);
  temp2 = FFT(2.0 * PI / n, temp2);
  for (int i = 0; i < n; i++) { temp1[i] *= temp2[i]; }
  temp1 = FFT(-2.0 * PI / n, temp1);
  vector<ll> ret;
  ret.resize(n);
  for (int i = 0; i < n; i++) {
    ret[i] = temp1[i].real() / n + 0.5;
  }
  return ret;
}

//================================================================
// Segment Tree

struct Node {
  int num;
  Node() {;}
  Node(int n) : num(n) {;}
};

inline Node merge(Node left, Node right) {
  return Node(left.num < right.num ? left.num : right.num);
}

struct SegmentTree {
  static const int MAX_DEPTH = 17;
  static const int SIZE = 1 << (MAX_DEPTH + 1);  // 2^18 = 262144

  Node data[SIZE];
  void change(int index, Node value) {
    int target = (1 << MAX_DEPTH) + index;
    data[target] = value;
    for (int i = 1; i <= MAX_DEPTH; i++) {
      target >>= 1;
      data[target] = merge(data[target * 2], data[target * 2 + 1]);
    }
  }
  Node get(int left, int right) {
    assert(left <= right);
    return in_get(0, 1, left, right);
  }
private:
  Node in_get(int depth, int node, int left, int right) {
    int width = 1 << (MAX_DEPTH - depth);
    int index = node - (1 << depth);
    int node_left = index * width;
    int node_mid = node_left + (width >> 1);
    if (right - left + 1 == width && left == node_left) {
      return data[node];
    } else if (right < node_mid) {
      return in_get(depth + 1, node * 2, left, right);
    } else if (left >= node_mid) {
      return in_get(depth + 1, node * 2 + 1, left, right);
    }
    return merge(in_get(depth + 1, node * 2, left, node_mid - 1), in_get(depth + 1, node * 2 + 1, node_mid, right));
  }
};

//================================================================
// Math

//a x + b y = gcd(a, b)
long long extgcd(long long a, long long b, long long &x, long long &y) {
  long long g = a; x = 1; y = 0;
  if (b != 0) g = extgcd(b, a % b, y, x), y -= (a / b) * x;
  return g;
}

long long InvMod(long long a, long long mod) {
  long long x, y;
  if (extgcd(a, mod, x, y) == 1) { return (x + mod) % mod; }
  return 0;
}

const int MAX_P = 100000;
int modFact[MAX_P];
void calcModFact(int p) {
  assert(p <= MAX_P);
  modFact[0] = 1;
  for (int i = 1; i < p; i++) {
    modFact[i] = (modFact[i - 1] * i) % p;
  }
}

int ModFact(int n, int p, int &e) {
  assert(p <= MAX_P);
  e = 0;
  if (n == 0) { return 1; }
  int res = ModFact(n / p, p, e);
  e += n / p;
  if (n / p % 2 != 0) { return res * (p - modFact[n % p]) % p; }
  return res * modFact[n % p] % p;
}

int ModCombi(int n, int k, int p) {
  if (n < 0 || k < 0 || n < k) { return 0; }
  int e1, e2, e3;
  int a1 = ModFact(n, p, e1);
  int a2 = ModFact(k, p, e2);
  int a3 = ModFact(n - k, p, e3);
  if (e1 > e2 + e3) { return 0; }
  return a1 * InvMod(a2 * a3 % p, p) % p;
}

// need extgcd
ll ChineseRemaiderTherom(const vector<ll> &anss, const vector<ll> &mods) {
  assert(anss.size() == mods.size());
  ll all = 1;
  for (int i = 0; i < (int)mods.size(); i++) {
    all *= mods[i];
  }
  ll ret = 0;
  for (int i = 0; i < (int)anss.size(); i++) {
    ll x, y;
    extgcd(mods[i], all / mods[i], x, y); 
    y = (y + all) % all;
    ret = (ret + (anss[i] + all) % all * y * (all / mods[i]) % all) % all;
    assert(ret >= 0); 
  }
  return ret;
}

//================================================================
// Millar Rabin

long long LargeMulMod(long long lhs, long long rhs, long long mod) {
  long long ret = 0;
  while (rhs > 0) {
    if (rhs & 1) { ret = (ret + lhs) % mod; }
    rhs >>= 1;
    lhs = (lhs + lhs) % mod;
  }
  return ret;
}

long long powmod(long long base, long long power, long long mod) {
  long long ans = 1;
  if (mod < (1 << 30)) {
    while (power > 0) {
      if (power & 1) { ans = ans * base % mod; }
      power >>= 1;
      base = base * base % mod;
    }
  } else {
    while (power > 0) {
      if (power & 1) { ans = LargeMulMod(ans, base, mod); }
      power >>= 1;
      base = LargeMulMod(base, base, mod);
    }
  }
  return ans;
}

bool suspect(long long t, long long s, long long d, long long n) {
  long long x = powmod(t, d, n);
  if (x == 1) { return true; }
  while (s--) {
    if (x == n - 1) { return true; }
    if (n < (1 << 30)) {
      x = x * x % n;
    } else {
      x = LargeMulMod(x, x, n);
    }
  }
  return false;
}

bool isPrime(long long n) {
  if (n <= 1 || (n > 2 && !(n & 1))) { return false; }
  long long d = n - 1;
  long long s = 0;
  while (!(d & 1)) {
    s++;
    d >>= 1;
  }
  //long long test[4] = { 2, 7, 61, 1LL << 60 }; // is for n < 2^32
  long long test[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 1LL << 60 }; // is for n < 10^16 (at least)
  for (int i = 0; test[i] < n; i++) {
    if (!(suspect(test[i], s, d, n))) { return false; }
  }
  return true;
}

//=====================================================================
// Long
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

//==================================================================
// Geometry

// O(n^2logn)
int MaxPointCircle(const vector<Point> &points, double r) {
  const int n = points.size();
  int ret = 1;
  for (int i = 0; i < n; i++) {
    vector<pair<double, int> > radians;
    for (int j = 0; j < n; j++) {
      double d = abs(points[i] - points[j]) / 2.0;
      if (i == j || d * 2.0 - r * 2.0 > EPS) { continue; }
      Point center = (points[i] + points[j]) / 2.0;
      Point vect = (points[j] - points[i]) * Point(0, -1);
      vect /= abs(vect);
      double l = sqrt(r * r - d * d);
      Point c1 = center + vect * l;
      Point c2 = center - vect * l;
      double arg1 = arg(c1 - points[i]) - EPS;
      double arg2 = arg(c2 - points[i]) + EPS;
      radians.push_back(make_pair(arg1, (j + 1)));
      radians.push_back(make_pair(arg2, -(j + 1)));
    }
    if ((int)radians.size() / 2 < ret - 1) { continue; }
    sort(radians.begin(), radians.end());
    vector<bool> used(points.size(), false);
    int lsum = 1;
    REP(iter, 2) {
      REP(j, radians.size()) {
        int index = abs(radians[j].second) - 1;
        bool start = radians[j].second > 0 ? true : false;
        if (used[index] && !start) {
          used[index] = false;
          lsum--;
        } else if (!used[index] && start) {
          used[index] = true;
          lsum++;
        }
        ret = max(ret, lsum);
      }
    }
  }
  return ret;
}

//=====================================================================
// Visualizer
// usage:
// g++ -Wall -Dvisualize hoge.cpp
// ./a.out 2> data.js
// firefox Visualize.html
int zoom = 10;
#ifdef visualize
void ChangeColor(int r, int g, int b) {
  fprintf(stderr, "c.strokeStyle = 'rgb(%d, %d, %d)';\n", r, g, b);
}
void DrawPoint(const Point &p) {
  fprintf(stderr, "circle(%d, %d, %d)\n",
      (int)(zoom*p.real()), 1980-(int)(zoom*p.imag()), 2);
}
void DrawLine(const Line &l) {
  fprintf(stderr, "line(%d, %d, %d, %d)\n",
      (int)(zoom*l[0].real()), 1980-(int)(zoom*l[0].imag()),
      (int)(zoom*l[1].real()), 1980-(int)(zoom*l[1].imag()));
}
void DrawPolygon(const Polygon &p) {
  const int n = p.size();
  for (int i = 0; i < n; i++) {
    DrawLine(Line(p[i], p[(i + 1) % n]));
  }
}
void DrawCircle(const Circle &c) {
  fprintf(stderr, "circle(%d, %d, %d)\n",
      (int)(zoom * c.p.real()), 1980 - (int)(zoom * c.p.imag()), (int)(zoom * c.r));
}
#else
void DrawPoint(const Point &p) {;}
void ChangeColor(int r, int g, int b) {;}
void DrawLine(Line l) {;}
void DrawPolygon(const Polygon &p) {;}
void DrawCircle(Circle c) {;}
#endif
int main(int argc, char *argv[]) {
  if (argc > 1) { zoom = atoi(argv[1]); }
}

<html>
<head><title>visualkun</title></head>
<script>
function line(x,y,a,b){c.b();c.moveTo(x,y);c.lineTo(a,b);c.s();}
function circle(x,y,r){c.b();c.arc(x,y,r,0,7,0);c.s();}
window.onload=function(){d=document;d.i=d.getElementById;
 c=d.i('c').getContext('2d');c.b=c.beginPath;c.s=c.stroke;
 d.i('s').src='data.js?';};
</script>
<body><canvas id="c" width="2000" height="2000"
style="border:1px solid #000;"></canvas>
<script id="s"></script></body> 
</html>
