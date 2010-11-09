struct FenwickTree {
  int tree[10000];
  FenwickTree() { memset(data, 0, sizeof(data)); }
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
