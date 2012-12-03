typedef int Node;
struct SkewHeap {
  Node v;
  SkewHeap *l;
  SkewHeap *r;
  SkewHeap() : l(NULL), r(NULL) {;}
  SkewHeap(Node v) : v(v), l(NULL), r(NULL) {;}
};
SkewHeap _pool[1000000];
SkewHeap *pool = NULL;

SkewHeap *Meld(SkewHeap *a, SkewHeap *b) {
  if (a == NULL) { return b; }
  if (b == NULL) { return a; }
  if (a->v < b->v) { swap(a, b); }
  a->r = Meld(a->r, b);
  swap(a->l, a->r);
  return a;
}

SkewHeap *Push(SkewHeap *a, const Node &v) {
  SkewHeap *b = pool++;
  *b = SkewHeap(v);
  return Meld(a, b);
}

SkewHeap *Pop(SkewHeap *a) {
  return Meld(a->l, a->r);
}
