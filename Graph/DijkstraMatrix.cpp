struct Point {
  int x;
  int y;
  Weight cost;
  Point() {;}
  Point(int x, int y, Weight c) : x(x), y(y), cost(c) {;}
  bool operator<(const Point &rhs) const { return cost > rhs.cost; }
};
Weight DijkstraMatrix(const Matrix &field, Point s, Point t) {
  const int h = field.size();
  assert(h > 0);
  const int w = field[0].size();
  vector<vector<bool> > visit(h, vector<bool>(w, false));
  Matrix dist(h, Array(w, 2000000000LL));
  priority_queue<Point> que;
  que.push(Point(s.x, s.y, 0));
  Weight ans = -1;
  dist[s.y][s.x] = 0;
  while (!que.empty()) {
    Point p = que.top();
    que.pop();
    if (visit[p.y][p.x]) { continue; }
    visit[p.y][p.x] = true;
    if (p.x == t.x && p.y == t.y) {
      ans = p.cost;
      break;
    }
    for (int i = 0; i < 4; i++) {
      const int dx[4] = { 1, 0, -1, 0 };
      const int dy[4] = { 0, 1, 0, -1 };
      int nx = p.x + dx[i];
      int ny = p.y + dy[i];
      if (nx < 0 || nx >= w || ny < 0 || ny >= h || visit[ny][nx]) { continue; }
      Weight ncost = p.cost + field[ny][nx];
      if (ncost >= dist[ny][nx]) { continue; }
      que.push(Point(nx, ny, ncost));
    }
  }
  return ans;
}
