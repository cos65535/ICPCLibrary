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

// need extgcd
ll ChineseRemainderTherom(const vector<ll> &anss, const vector<ll> &mods) {
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
