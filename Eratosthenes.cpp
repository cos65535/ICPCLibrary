#include <bitset>
const int PRIME_SIZE = 1000000;
int psize;
bitset<PRIME_SIZE + 1> bprime;
int prime[PRIME_SIZE / 10 + 1000];
int Eratosthenes(int n) {
  psize = 0;
  bprime.set();
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
