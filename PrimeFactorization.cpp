// need Eratosthenes
vector<pair<int, int> > PrimeFactorization(int n) {
  vector<pair<int, int> > ret;
  for (int i = 0; i < psize && i * i <= n; i++) {
    if (prime[i] * prime[i] > n) { break; }
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

