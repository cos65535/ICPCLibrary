struct Node {
  int num;
  Node() {;}
  Node(int n) : num(n) {;}
};

inline Node merge(Node left, Node right) {
  return Node(left.num < right.num ? left.num : right.num);
}

struct SegmentTree {
  static const int MAX_DEPTH = 17;
  static const int SIZE = 1 << (MAX_DEPTH + 1);  // 2^18 = 262144

  Node data[SIZE];
  void change(int index, Node value) {
    int target = (1 << MAX_DEPTH) + index;
    data[target] = value;
    for (int i = 1; i <= MAX_DEPTH; i++) {
      target >>= 1;
      data[target] = merge(data[target * 2], data[target * 2 + 1]);
    }
  }
  Node get(int left, int right) {
    assert(left <= right);
    return in_get(0, 1, left, right);
  }
private:
  Node in_get(int depth, int node, int left, int right) {
    int width = 1 << (MAX_DEPTH - depth);
    int index = node - (1 << depth);
    int node_left = index * width;
    int node_mid = node_left + (width >> 1);
    if (right - left + 1 == width && left == node_left) {
      return data[node];
    } else if (right < node_mid) {
      return in_get(depth + 1, node * 2, left, right);
    } else if (left >= node_mid) {
      return in_get(depth + 1, node * 2 + 1, left, right);
    }
    return merge(in_get(depth + 1, node * 2, left, node_mid - 1), in_get(depth + 1, node * 2 + 1, node_mid, right));
  }
};
