void OutputSVGHeader(FILE *fp, double minx, double miny, double maxx, double maxy) {
  double w = (maxx - minx + 2);
  double h = (maxy - miny + 2);
  fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
  fprintf(fp, "<svg width=\"20cm\" height=\"20cm\" viewBox=\"%.3lf %3.lf %.3lf %.3lf\"\n", minx - 1, miny - 1 , w, h);
  fprintf(fp, "  xmlns=\"http://www.w3.org/2000/svg\"\n");
  fprintf(fp, "  xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
}
void OutputSVGFooter(FILE *fp) {
  fprintf(fp, "</svg>\n");
}

void OutputSVGPolygon(FILE *fp, const Polygon &poly, int color = -1) {
  const int n = poly.size();
  char color_name[100];
  if (color == -1) {
    snprintf(color_name, 99, "none");
  } else {
    int c1 = (color >> 0)  & 0xff;
    int c2 = (color >> 8)  & 0xff;
    int c3 = (color >> 16) & 0xff;
    snprintf(color_name, 99, "#%02x%02x%02x", c1, c2, c3);
  }
  fprintf(fp, "<polygon fill=\"%s\" stroke=\"#000000\" stroke-width=\"0.300\" points=\"", color_name);
  REP(i, n) {
    fprintf(fp, "%.3lf,%.3lf ", poly[i].real(), poly[i].imag());
  }
  fprintf(fp, "\"/>\n");
}
