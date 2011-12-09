template <class T>
class stack {
  static const int SIZE = 10000;
  T st[SIZE];
  int s;
public:
  stack() { Init(); }
  void Init() {
    s = 0;
  }
  bool empty() { return s == 0; }
  T top() {
    assert(!empty());
    return st[s - 1];
  }
  void push(T x) {
    st[s++] = x;
    assert(s < SIZE);
  }
  void pop() {
    assert(!empty());
    s--;
  }
};
