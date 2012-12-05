unsigned int Xor128(void) { 
  static unsigned int x = 0x34fb2383;
  static unsigned int y = 0x327328fa;
  static unsigned int z = 0xabd4b54a;
  static unsigned int w = 0xa9dba8d1; 
  unsigned int t;
       
  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
}
struct Random {
  int next(int r) { return Xor128() % r; }
  int next(int l, int r) { return next(r - l + 1) + l; }
  ll next(ll r) { return (ll)((((unsigned long long)Xor128() << 32) + (unsigned long long)Xor128()) % r); }
  ll next(ll l, ll r) { return next(r - l + 1) + l; }
  double next(double r) { return (double)Xor128() / 0xffffffff * r; }
  double next(double l, double r) { return next(r - l) + l; }
};
Random rnd;
