// usage:
// g++ -Wall -Dvisualize hoge.cpp
// ./a.out 2> data.js
// firefox Visualize.html
int zoom = 10;
#ifdef visualize
void ChangeColor(int r, int g, int b) {
  fprintf(stderr, "c.strokeStyle = 'rgb(%d, %d, %d)';\n", r, g, b);
}
void DrawPoint(const Point &p) {
  fprintf(stderr, "fillRect(%d, %d, %d, %d)\n",
      (int)(zoom*(p.real()-2)), 1980-(int)(zoom*(p.imag() - 2)),
      (int)(zoom*(p.real()+2)), 1980-(int)(zoom*(p.imag() + 2)));
}
void DrawLine(const Line &l) {
  fprintf(stderr, "line(%d, %d, %d, %d)\n",
      (int)(zoom*l[0].real()), 1980-(int)(zoom*l[0].imag()),
      (int)(zoom*l[1].real()), 1980-(int)(zoom*l[1].imag()));
}
void DrawPolygon(const Polygon &p) {
  const int n = p.size();
  for (int i = 0; i < n; i++) {
    DrawLine(Line(p[i], p[(i + 1) % n]));
  }
}
void DrawCircle(const Circle &c) {
  fprintf(stderr, "circle(%d, %d, %d)\n",
      (int)(zoom * c.p.real()), 1980 - (int)(zoom * c.p.imag()), (int)(zoom * c.r));
}
#else
void DrawPoint(const Point &p) {;}
void ChangeColor(int r, int g, int b) {;}
void DrawLine(Line l) {;}
void DrawPolygon(const Polygon &p) {;}
void DrawCircle(Circle c) {;}
#endif
int main(int argc, char *argv[]) {
  if (argc > 1) { zoom = atoi(argv[1]); }
}
