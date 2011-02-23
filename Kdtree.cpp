// average
// insert: O(logn)
// search: O(logn + k)
#include <complex>
typedef complex<double> Point;
struct Kdtree {
  struct Node {
    Point p;
    Node *l;
    Node *r;
    Node(const Point &p) : p(p), l(NULL), r(NULL) {;}
  };
  Node *root;
  Kdtree() : root(NULL) {;}
  ~Kdtree() { release(); }
  Kdtree(const Kdtree&);
  Kdtree operator=(const Kdtree&);

  void insert(const Point &p) {
    root = insert(root, 0, p);
  }
  vector<Point> search(double left, double top, double right, double bottom) {
    if (left > right) { swap(left, right); }
    if (top > bottom) { swap(top, bottom); }
    vector<Point> ret;
    search(root, 0, Point(left, top), Point(right, bottom), ret);
    return ret;
  }
  void release() {
    release(root);
    root = NULL;
  }
private:
  bool compare(int d, const Point &p, const Point &q) {
    return d ? real(p) < real(q) : imag(p) < imag(q);
  }
  Node *insert(Node *t, int d, const Point &p) {
    if (t == NULL) { return new Node(p); }
    if (compare(d, p, t->p)) { t->l = insert(t->l, !d, p); }
    else                     { t->r = insert(t->r, !d, p); }
    return t;
  }
  void search(Node *t, int d, const Point &ld, const Point &ru, vector<Point> &out) {
    if (t == NULL) { return; }
    const Point &p = t->p;
    if (real(ld) <= real(p) && real(p) <= real(ru) &&
        imag(ld) <= imag(p) && imag(p) <= imag(ru)) { out.push_back(p); }
    if (!compare(d, p, ld)) { search(t->l, !d, ld, ru, out); }
    if (!compare(d, ru, p)) { search(t->r, !d, ld, ru, out); }
  }
  void release(Node *t) {
    if (t == NULL) { return; }
    release(t->l);
    release(t->r);
    delete t;
  }
};
