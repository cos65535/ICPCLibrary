#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: input1 input2 [accuracy]\n");
    return 0;
  }
  FILE *fp1 = fopen(argv[1], "r");
  FILE *fp2 = fopen(argv[2], "r");
  if (fp1 == NULL) {
    printf("can't open %s\n", argv[1]);
    return 0;
  }
  if (fp2 == NULL) {
    printf("can't open %s\n", argv[2]);
    return 0;
  }
  int accuracy = 6;
  if (argc >= 4) {
    accuracy = atoi(argv[3]);
    if (accuracy == 0) {
      printf("accuracy is greater than 0\n");
      return 0;
    }
  }
  double eps = 1.0;
  for (int i = 0; i < accuracy; i++) {
    eps *= 0.1;
  }
  double left, right;
  int number = 0;
  while (fscanf(fp1, "%lf", &left) > 0 && fscanf(fp2, "%lf", &right) > 0) {
    number++;
    if (fabs(left - right) > eps) {
      printf("Number %d is wrong: %.10lf %.10lf\n", number, left, right);
      return 0;
    }
  }
}
