struct FenwickTree {
  static const int SIZE = 10000;
  int tree[SIZE + 10];
  FenwickTree() { memset(tree, 0, sizeof(tree)); }
  void add(int index, int value) {
    for (index++; index < SIZE + 1; index += (index & -index)) {
      tree[index] += value;
    }
  }
  int sum(int index) {
    int ret = 0;
    for (index++; index > 0; index -= (index & -index)) {
      ret += tree[index];
    }
    return ret;
  }
};
