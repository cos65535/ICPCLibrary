#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

using namespace std;
typedef long long ll;
static const double EPS = 1e-9;
static const double PI = acos(-1.0);

#define REP(i, n) for (int i = 0; i < (int)(n); i++)
#define FOR(i, s, n) for (int i = (s); i < (int)(n); i++)
#define FOREQ(i, s, n) for (int i = (s); i <= (int)(n); i++)
#define FORIT(it, c) for (__typeof((c).begin())it = (c).begin(); it != (c).end(); it++)
#define DEC(i, s) for (int i = (s); i >= 0; i--)

#define SIZE(v) (int)((v).size())
#define MEMSET(v, h) memset((v), h, sizeof(v))
#define FIND(m, w) ((m).find(w) != (m).end())

int main(int argc, char *argv[]) {
  srand(time(NULL));
  int test = rand() % 10 + 1;
  if (argc > 1) { test = atoi(argv[1]); }
  while (test--) {
  }
}
