long long gcd(long long a, long long b) {
  if (b == 0) { return a; }
  return gcd(b, a % b);
}

long long lcm(long long a, long long b) {
  return a / gcd(a, b) * b;
}
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

ll Mul(ll a, ll b, ll mod) {
  a %= mod;
  b %= mod;
  if (mod < 2e+9) { return a * b % mod; }
  ll ret = 0;
  while (b > 0) {
    if (b & 1) { ret = (ret + a) % mod; }
    a = (a + a) % mod;
    b >>= 1;
  }
  return ret;
}

// need extgcd
pair<ll, ll> PairChineseRemainderTherom(ll ans1, ll mod1, ll ans2, ll mod2) {
  ll g = gcd(mod1, mod2);
  if (ans1 % g != ans2 % g) { return make_pair(-1, -1); }
  const ll anss[2] = { ans1 / g, ans2 / g };
  const ll mods[2] = { mod1 / g, mod2 / g };
  ll all = mods[0] * mods[1];
  assert(all * g / g / mods[0] == mods[1]);
  ll ret = 0;
  for (int i = 0; i < 2; i++) {
    ll x, y;
    extgcd(mods[i], all / mods[i], x, y); 
    y = (y + all) % all;
    ll v = Mul(y, anss[i], all);
    v = Mul(v, all / mods[i], all);
    ret = (ret + v) % all;
    assert(ret >= 0); 
  }
  ret = ret * g + ans1 % g;
  return make_pair(ret, all * g);
}

pair<ll, ll> ChineseRemainderTherom(const vector<ll> &anss, const vector<ll> &mods) {
  assert(anss.size() == mods.size());
  pair<ll, ll> ret(anss[0], mods[0]);
  for (int i = 1; i < (int)anss.size(); i++) {
    ret = PairChineseRemainderTherom(ret.first, ret.second, anss[i], mods[i]);
    if (ret.first == -1) { return ret; }
  }
  return ret;
}

// solve A[i] x == B[i] (mod M[i])
pair<ll, ll> LinearCongruence(const vector<ll> &A, const vector<ll> &B, const vector<ll> &M) {
  ll x = 0;
  ll m = 1;
  for (ll i = 0; i < (ll)A.size(); i++) {
    ll a = A[i] * m;
    ll b = B[i] - A[i] * x;
    ll d = gcd(M[i], a);
    if (b % d != 0) { return make_pair(0, -1); }
    if (a == 0) { continue; }
    ll t = b / d * InvMod(a / d, M[i] / d) % (M[i] / d);
    x = x + m * t;
    m *= M[i] / d;
  }
  x %= m;
  if (x < 0) { x += m; }
  return make_pair(x % m, m);
}
