#include <deque>
template <class T>
ostream &operator<<(ostream &os, const vector<T> &rhs) {
  os << "[ ";
  FORIT(it, rhs) {
    if (it != rhs.begin()) { os << ", "; }
    os << *it;
  }
  os << " ]";
  return os;
}
template <class T>
ostream &operator<<(ostream &os, const deque<T> &rhs) {
  os << "[ ";
  FORIT(it, rhs) {
    if (it != rhs.begin()) { os << ", "; }
    os << *it;
  }
  os << " ]";
  return os;
}
template <class T, class U>
ostream &operator<<(ostream &os, const map<T, U> &rhs) {
  os << "{" << endl;
  FORIT(it, rhs) {
    if (it != rhs.begin()) { os << "," << endl; }
    os << "  " << it->first << " : " << it->second;
  }
  os << endl << "}";
  return os;
}
template <class T>
ostream &operator<<(ostream &os, const set<T> &rhs) {
  os << "{ ";
  FORIT(it, rhs) {
    if (it != rhs.begin()) { os << ", "; }
    os << *it;
  }
  os << " }";
  return os;
}
