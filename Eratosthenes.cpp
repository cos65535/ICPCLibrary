const int PRIME_SIZE = 1000000;
int psize;
bool bprime[PRIME_SIZE+1];
int prime[PRIME_SIZE+1];
int Eratosthenes(int n) {
  psize = 0;
  memset(bprime, true, sizeof(bprime));
  bprime[0] = bprime[1] = false;
  for (int i = 2; i <= n; i++) {
    if (!bprime[i]) { continue; }
    prime[psize++] = i;
    if ((long long)i * i > n) { continue; }
    for (int j = i * i; j <= n; j += i) {
      bprime[j] = false;
    }
  }
  return psize;
}
