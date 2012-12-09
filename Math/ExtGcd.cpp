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
