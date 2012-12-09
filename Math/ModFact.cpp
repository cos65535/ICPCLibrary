const int MAX_P = 100000;
int modFact[MAX_P];
void CalcModFact(int p) {
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

// need InvMod
int ModCombi(int n, int k, int p) {
  if (n < 0 || k < 0 || n < k) { return 0; }
  int e1, e2, e3;
  int a1 = ModFact(n, p, e1);
  int a2 = ModFact(k, p, e2);
  int a3 = ModFact(n - k, p, e3);
  if (e1 > e2 + e3) { return 0; }
  return a1 * InvMod(a2 * a3 % p, p) % p;
}
