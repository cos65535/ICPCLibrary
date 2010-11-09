#include <sys/time.h>
long long get_time() {
  timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000LL + t.tv_usec / 1000;
}
bool time_over(long long start, long long limit) {
  if (get_time() - start >= limit) { return true; }
  return false;
}

