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
  ans1 %= mod1; ans1 = (ans1 + mod1) % mod1;
  ans2 %= mod2; ans2 = (ans2 + mod2) % mod2;
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
  //if (ret == 0) { ret = all * g; }
  return make_pair(ret, all * g);
}

// solve x == anss[i] (mod  mods[i])
pair<ll, ll> ChineseRemainderTherom(const vector<ll> &anss, const vector<ll> &mods) {
  assert(anss.size() == mods.size());
  pair<ll, ll> ret(anss[0], mods[0]);
  for (int i = 1; i < (int)anss.size(); i++) {
    ret = PairChineseRemainderTherom(ret.first, ret.second, anss[i], mods[i]);
    if (ret.first == -1) { return ret; }
  }
  return ret;
}

// solve A x == B (mod M)
ll Divide(ll A, ll B, ll M) {
  A %= M; A = (A + M) % M;
  B %= M; B = (B + M) % M;
  ll tg = gcd(A, gcd(B, M));
  A /= tg; B /= tg; M /= tg;
  ll g = gcd(A, M);
  if (B % g != 0) { return -1; }
  ll x, y;
  extgcd(A, M, x, y);
  ll n = x * B / g;
  ll dn = M / g;
  n -= n / dn * dn;
  if (n < 0) { n += dn; }
  return n;
}


pair<ll, ll> PairLinearCongruence(ll A1, ll B1, ll M1, ll A2, ll B2, ll M2) {
  A1 %= M1; A1 = (A1 + M1) % M1;
  B1 %= M1; B1 = (B1 + M1) % M1;
  A2 %= M2; A2 = (A2 + M2) % M2;
  B2 %= M2; B2 = (B2 + M2) % M2;
  if (A1 == 0 && B1 != 0) { return make_pair(-1, -1); }
  if (A2 == 0 && B2 != 0) { return make_pair(-1, -1); }
  if (A1 == 0 && A2 == 0) { return make_pair(0, 1); }
  if (A1 == 0) {
    swap(A1, A2);
    swap(B1, B2);
    swap(M1, M2);
  }
  ll g1 = gcd(A1, gcd(B1, M1));
  A1 /= g1; B1 /= g1; M1 /= g1;
  ll v1 = Divide(A1, B1, M1);
  if (v1 == -1) { return make_pair(-1, -1); }
  if (A2 == 0) {
    //if (v1 == 0) { v1 = M1; }
    return make_pair(v1, M1);
  }
  ll g2 = gcd(A2, gcd(B2, M2));
  A2 /= g2; B2 /= g2; M2 /= g2;
  ll v2 = Divide(A2, B2, M2);
  if (v2 == -1) { return make_pair(-1, -1); }
  pair<ll, ll> ret = PairChineseRemainderTherom(v1, M1, v2, M2);
  //if (ret.first == 0) { ret.first = ret.second; }
  return ret;
}

// solve A[i] x == B[i] (mod M[i])
pair<ll, ll> LinearCongruence(const vector<ll> &A, const vector<ll> &B, const vector<ll> &M) {
  pair<ll, ll> ret(0, 1);
  for (int i = 0; i < (int)A.size(); i++) {
    ret = PairLinearCongruence(1, ret.first, ret.second, A[i], B[i], M[i]);
    if (ret.first == -1) { return ret; }
  }
  return ret;
}
