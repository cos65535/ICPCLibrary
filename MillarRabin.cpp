long long powmod(long long base, int power, int mod) {
  long long ans = 1;
  while (power > 0) {
    if (power & 1) { ans = ans * base % mod; }
    power >>= 1;
    base = base * base % mod;
  }
  return ans;
}

bool suspect(int t, int s, int d, int n) {
  long long x = powmod(t, d, n);
  if (x == 1) { return true; }
  while (s--) {
    if (x == n - 1) { return true; }
    x = x * x % n;
  }
  return false;
}

bool isPrime(int n) {
  if (n <= 1 || (n > 2 && !(n & 1))) { return false; }
  int d = n - 1;
  int s = 0;
  while (!(d & 1)) {
    s++;
    d >>= 1;
  }
  int test[4] = { 2, 7, 61, 0x7fffffff };
  for (int i = 0; test[i] < n; i++) {
    if (!(suspect(test[i], s, d, n))) { return false; }
  }
  return true;
}

