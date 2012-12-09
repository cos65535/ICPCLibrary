long long combi(long long n, long long r) {
  if (r < 0 || n < r) { return 0; }
  r = min(r, n - r);
  ll ret = 1;
  for (int i = 1; i <= r; i++) {
    ret = ret * (n - i + 1) / i;
  }
  return ret;
}
