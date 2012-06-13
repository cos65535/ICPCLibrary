struct Node {
  int lower;
  int upper;
  Node() {;}
  Node(int lower, int upper)
    : lower(lower), upper(upper) {;}
};

inline Node Merge(Node left, Node right) {
  return Node(min(left.lower, right.lower), max(left.upper, right.upper));
}

struct SegmentTree {
  static const int MAX_DEPTH = 18;
  static const int SIZE = 1 << (MAX_DEPTH + 1);  // 2^18 = 262144

  bool updated[SIZE];
  Node data[SIZE];
  SegmentTree() { memset(updated, false, sizeof(updated)); }
  void set(int left, int right, Node v) {
    assert(left <= right);
    return in_set(v, 0, 1, left, right);
  }
  Node get(int left, int right) {
    assert(left <= right);
    return in_get(0, 1, left, right);
  }

private:
  void Divide(int node) {
    if (!updated[node] || node >= (1 << MAX_DEPTH)) { return; }
    updated[node] = false;
    updated[node * 2] = true;
    updated[node * 2 + 1] = true;
    data[node * 2] = data[node];
    data[node * 2 + 1] = data[node];
  }

  void in_set(Node v, int depth, int node, int left, int right) {
    int width = 1 << (MAX_DEPTH - depth);
    int index = node - (1 << depth);
    int node_left = index * width;
    int node_mid = node_left + (width >> 1);
    Divide(node);
    if (right - left + 1 == width && left == node_left) {
      updated[node] = true;
      data[node] = v;
    } else {
      if (right < node_mid) {
        in_set(v, depth + 1, node * 2, left, right);
      } else if (left >= node_mid) {
        in_set(v, depth + 1, node * 2 + 1, left, right);
      } else {
        in_set(v, depth + 1, node * 2, left, node_mid - 1);
        in_set(v, depth + 1, node * 2 + 1, node_mid, right);
      }
      data[node] = Merge(data[node * 2], data[node * 2 + 1]);
    }
  }

  Node in_get(int depth, int node, int left, int right) {
    int width = 1 << (MAX_DEPTH - depth);
    int index = node - (1 << depth);
    int node_left = index * width;
    int node_mid = node_left + (width >> 1);
    Divide(node);
    if (right - left + 1 == width && left == node_left) {
      return data[node];
    } else if (right < node_mid) {
      return in_get(depth + 1, node * 2, left, right);
    } else if (left >= node_mid) {
      return in_get(depth + 1, node * 2 + 1, left, right);
    }
    return Merge(in_get(depth + 1, node * 2, left, node_mid - 1), in_get(depth + 1, node * 2 + 1, node_mid, right));
  }
};
