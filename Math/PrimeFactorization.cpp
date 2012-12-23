// need Eratosthenes
vector<pair<long long, int> > PrimeFactorization(long long n) {
  assert(n > 1);
  assert(psize != 0);
  vector<pair<long long, int> > ret;
  for (int i = 0; i < psize && (long long)prime[i] * prime[i] <= n; i++) {
    int size = 0;
    while (n % prime[i] == 0) {
      n /= prime[i];
      size++;
    }
    if (size != 0) { ret.push_back(make_pair(prime[i], size)); }
  }
  if (n != 1) { ret.push_back(make_pair(n, 1)); }
  return ret;
}

