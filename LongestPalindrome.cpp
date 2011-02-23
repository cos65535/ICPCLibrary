// rad[i] is longest palindrome radius if center is (i+1)/2
vector<int> LongestPalindrome(const char *str) {
  int n = strlen(str);
  vector<int> rad(2 * n); 
  int k;
  for (int i = 0, j = 0; i < 2 * n; i += k, j = max(j - k, 0)) {
    while (i - j >= 0 && i + j + 1 < 2 * n && str[(i - j) / 2] == str[(i + j + 1) / 2]) { j++; }
    rad[i] = j;
    for (k = 1; i - k >= 0 && rad[i] - k >= 0 && rad[i - k] != rad[i] - k; k++) {
      rad[i + k] = min(rad[i - k], rad[i] - k); 
    }   
  }
  for (int i = 0; i < 2 * n; i++) { rad[i] = (rad[i] + 1) / 2; }
  return rad;
}
