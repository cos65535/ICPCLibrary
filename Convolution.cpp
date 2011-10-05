#include <complex>
typedef complex<double> Complex;
// a.size must 2^x
vector<Complex> FFT(double theta, const vector<Complex> &a) {
  int n = a.size();
  vector<Complex> ret = a;
  for (int m = n; m >= 2; m >>= 1) {
    int mh = m >> 1;
    for (int i = 0; i < mh; i++) {
      Complex w = exp(i * theta * Complex(0, 1));
      for (int j = i; j < n; j += m) {
        int k = j + mh;
        Complex x = ret[j] - ret[k];
        ret[j] += ret[k];
        ret[k] = w * x;
      }
    }
    theta *= 2;
  }
  int i = 0;
  for (int j = 1; j < n - 1; j++) {
    for (int k = n >> 1; k > (i ^= k); k >>= 1) {;}
    if (j < i) { swap(ret[i], ret[j]); }
  }
  return ret;
}

// ret[x] = sum_{i+j=x}(lhs[i]*rhs[j])
vector<ll> Convolution(const vector<ll> &lhs, const vector<ll> &rhs) {
  int n = 1;
  while (n < (int)max(lhs.size(), rhs.size()) * 2) { n <<= 1; }
  vector<Complex> temp1(n);
  vector<Complex> temp2(n);
  for (int i = 0; i < n / 2; i++) {
    if (i < (int)lhs.size()) {
      temp1[i] = Complex(lhs[i], 0);
    }
    if (i < (int)rhs.size()) {
      temp2[i] = Complex(rhs[i], 0);
    }
  }
  temp1 = FFT(2.0 * PI / n, temp1);
  temp2 = FFT(2.0 * PI / n, temp2);
  for (int i = 0; i < n; i++) { temp1[i] *= temp2[i]; }
  temp1 = FFT(-2.0 * PI / n, temp1);
  vector<ll> ret;
  ret.resize(n);
  for (int i = 0; i < n; i++) {
    ret[i] = temp1[i].real() / n + 0.5;
  }
  return ret;
}
