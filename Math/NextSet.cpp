// return v s.t. |v| == |x| && v > x
inline unsigned int NextSet(unsigned int x)
{
  unsigned int smallest = x & -x;
  unsigned int ripple = x + smallest;
  unsigned int new_smallest = ripple & -ripple;
  unsigned int ones = ((new_smallest / smallest) >> 1) - 1;
  return ripple | ones;
}

// Usage:
// for (int T = S & -S; T; T = NextSubset(T, S)) {
// }
inline unsigned NextSubset(unsigned S, unsigned U)
{
  return (S - U) & U;
}
