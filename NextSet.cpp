inline unsigned nextSet(unsigned x)
{
  unsigned smallest = x & -x;
  unsigned ripple = x + smallest;
  unsigned new_smallest = ripple & -ripple;
  unsigned ones = ((new_smallest / smallest) >> 1) - 1;
  return ripple | ones;
}

inline unsigned nextSubset(unsigned S, unsigned U)
{
  return (S - U) & U;
}
