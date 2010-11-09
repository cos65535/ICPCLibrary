inline int bsf(long long n)
{
  int z;
  if ((n & 0xffffffff) == 0) {
    __asm__("bsf %1, %0;" :"=r"(z) :"r"((int)(n >> 32)));
    return 32 + z;
  }
  __asm__("bsf %1, %0;" :"=r"(z) :"r"((int)n));
  return z;
}

