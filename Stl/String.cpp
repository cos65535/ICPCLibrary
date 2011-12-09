void memcpy(void *dest, const void *src, unsigned int n) {
  unsigned char *D = (unsigned char*)dest;
  unsigned char *S = (unsigned char*)src;
  while (n--) {
    *D = *S;
    D++;
    S++;
  }
}
void memset(void *dest, int v, unsigned int n) {
  unsigned char *D = (unsigned char*)dest;
  while (n--) {
    *D = v;
    D++;
  }
}
