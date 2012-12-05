typedef int Node;
struct TreapNode {
  Node v;
  TreapNode *child[2];
  int pri;
  int size;
  TreapNode() : pri(rand()), size(0) {
    child[0] = child[1] = NULL;
  }
  TreapNode(Node v) : v(v), pri(rand()), size(1) {
    child[0] = child[1] = NULL;
  }
  TreapNode *Update() {
    size = 1 + (child[0] ? child[0]->size : 0) + (child[1] ? child[1]->size : 0);
    return this;
  }
};
TreapNode _pool[1000000];
TreapNode *pool = NULL;

int Count(const TreapNode *t) { return t ? t->size : 0; }

TreapNode *Rotate(TreapNode *t, int lr) {
  TreapNode *s = t->child[1 - lr];
  t->child[1 - lr] = s->child[lr];
  s->child[lr] = t;
  t->Update();
  s->Update();
  return s;
}

TreapNode *Find(TreapNode *t, const Node &v) {
  if (t == NULL) { return NULL; }
  if (!(v < t->v) && !(t->v < v)) { return t; }
  else if (v < t->v) { return Find(t->child[0], v); }
  else { return Find(t->child[1], v); }
  assert(false);
  return NULL;
}

TreapNode *InInsert(TreapNode *t, TreapNode *v) {
  if (t == NULL) { return v; }
  int lr = t->v < v->v;
  t->child[lr] = InInsert(t->child[lr], v);
  t->Update();
  if (t->pri > t->child[lr]->pri) {
    t = Rotate(t, 1 - lr);
  }
  return t;
}

TreapNode *Insert(TreapNode *t, const Node &v) {
  assert(pool != NULL);
  TreapNode *node = pool++;
  *node = TreapNode(v);
  return InInsert(t, node);
}

TreapNode *InErase(TreapNode *t) {
  assert(t != NULL);
  int lr = 0;
  if (t->child[0] == NULL && t->child[1] == NULL) { return NULL; }
  else if (t->child[0] == NULL) { lr = 1; }
  else if (t->child[1] == NULL) { lr = 0; }
  else { lr = t->child[0]->pri > t->child[1]->pri; }
  t = Rotate(t, 1 - lr);
  t->child[1 - lr] = InErase(t->child[1 - lr]);
  t->Update();
  return t;
}

TreapNode *Erase(TreapNode *t, const Node &v) {
  assert(t != NULL);
  if (!(v < t->v) && !(t->v < v)) {
    t = InErase(t);
  } else if (v < t->v) {
    t->child[0] = Erase(t->child[0], v);
  } else {
    t->child[1] = Erase(t->child[1], v);
  }
  if (t != NULL) { t->Update(); }
  return t;
}

TreapNode *At(TreapNode *t, int index) {
  assert(t != NULL);
  if (index < Count(t->child[0])) {
    return At(t->child[0], index);
  } else {
    index -= Count(t->child[0]);
  }
  if (index == 0) { return t; }
  index--;
  return At(t->child[1], index);
}
