template <class T, class U>
struct pair {
  T first;
  U second;
  pair(T f, U s) : first(f), second(s) {;}
};
template <class T, class U>
pair<T, U> make_pair(T f, U s) { return pair<T, U>(f, s); }

template <class T>
T abs(T x) { return x < 0 ? -x : x; }
template <class T>
bool max(const T &lhs, const T &rhs) { return lhs < rhs ? rhs : lhs; }
template <class T>
bool min(const T &lhs, const T &rhs) { return lhs < rhs ? lhs : rhs; }
template <class T>
void swap(T &lhs, T &rhs) {
  T temp(lhs);
  lhs = rhs;
  rhs = temp;
}

/*
class priority_queue {
  static const int SIZE = 10000;
  T que[SIZE];
  int s;
  void PushTop() {
    int index = 1;
    while (index * 2 <= s) {
      int child = HeapSwap(index);
      if (child == 0) { break; }
      if (child == 1) { index = index * 2; }
      if (child == 2) { index = index * 2 + 1; }
    }
  }
  void PushBottom() {
    int index = s;
    while (index > 1) {
      index /= 2;
      if (HeapSwap(index) == 0) { break; }
    }
  }
  int HeapSwap(int index) {
    int ret = -1;
    if (index * 2 == s) {
      if (que[index * 2] < que[index]) {
        ret = 0;
      } else {
        swap(que[index], que[index * 2]);
        ret = 1;
      }
    } else if (index * 2 + 1 <= s) {
      if (que[index * 2] < que[index] && que[index * 2 + 1] < que[index]) {
        ret = 0;
      } else if (que[index] < que[index * 2] && que[index * 2 + 1] < que[index * 2]) {
        swap(que[index], que[index * 2]);
        ret = 1;
      } else if (que[index] < que[index * 2 + 1] && que[index * 2] < que[index * 2 + 1]) {
        swap(que[index], que[index * 2 + 1]);
        ret = 2;
      } else if (que[index] < que[index * 2]) {
        swap(que[index], que[index * 2]);
        ret = 1;
      } else {
        ret = 0;
      }
    }
    return ret;
  }
  public:
  priority_queue() { Init(); }
  void Init() {
    s = 0;
  }
  T top() {
    assert(s != 0);
    return que[1];
  }
  bool empty() { return s == 0; }
  void push(T x) {
    assert(s < SIZE - 1);
    que[s + 1] = x;
    s++;
    PushBottom();
  }
  void pop() {
    assert(s != 0);
    swap(que[1], que[s]);
    s--;
    PushTop();
  }
};

template <class Iterator>
struct iterator_traits {
  typedef typename Iterator::value_type value_type;
};
template <class T>
struct iterator_traits<T*> {
  typedef T value_type;
};
template <class RandomAccessIterator>
void sort(RandomAccessIterator begin, RandomAccessIterator end) {
  typedef typename iterator_traits<RandomAccessIterator>::value_type ValueType;
  if (begin == end) { return; }
  priority_queue<ValueType> que;
  RandomAccessIterator it = begin;
  while (it != end) {
    que.push(*it);
    it++;
  }
  it = end;
  it--;
  while (true) {
    *it = que.top();
    que.pop();
    if (it == begin) { break; }
    it--;
  }
}
*/
