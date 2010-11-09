struct PMA {
  PMA *next[128]; // next[0] is for fail
  vector<int> accept;
  PMA() { fill(next, next + 128, (PMA*)NULL); }
};

PMA *buildPMA(char p[1000][1000], int size) {
  PMA *root = new PMA;
  for (int i = 0; i < size; i++) { // make trie
    PMA *t = root;
    for (int j = 0; p[i][j]; j++) {
      int c = p[i][j];
      if (t->next[c] == NULL) {
        t->next[c] = new PMA;
      }
      t = t->next[c];
    }
    t->accept.push_back(i);
  }
  queue<PMA*> que; // make failure link using bfs
  for (int c = 1; c < 128; c++) {
    if (root->next[c]) {
      root->next[c]->next[0] = root;
      que.push(root->next[c]);
    } else {
      root->next[c] = root;
    }
  }
  while (!que.empty()) {
    PMA *t = que.front();
    que.pop();
    for (int c = 1; c < 128; c++) {
      if (t->next[c] != NULL) {
        que.push(t->next[c]);
        PMA *r = t->next[0];
        while (!r->next[c]) {
          r = r->next[0];
        }
        t->next[c]->next[0] = r->next[c];
        for (vector<int>::iterator it = r->next[c]->accept.begin(); it != r->next[c]->accept.end(); it++) {
          t->next[c]->accept.push_back(*it);
        }
      }
    }
  }
  return root;
}

void deletePMA(PMA *root) {
  set<PMA*> visit;
  queue<PMA*> que;
  que.push(root);
  visit.insert(root);
  while (!que.empty()) {
    PMA *t = que.front();
    que.pop();
    for (int c = 1; c < 128; c++) {
      if (t->next[c] == NULL) { continue; }
      if (visit.count(t->next[c])) { continue; }
      visit.insert(t->next[c]);
      que.push(t->next[c]);
    }
  }
  for (set<PMA*>::iterator it = visit.begin(); it != visit.end(); it++) {
    delete *it;
  }
}

// n = strlen(t)
// m[i] = strlen(p[i])
vector<int> match(const char *t, int n, const PMA *v, int m[], int size) {
  vector<int> ret(size, -1);
  //int n = strlen(t);
  for (int i = 0; i < n; i++) {
    int c = t[i];
    while (!v->next[c]) {
      v = v->next[0];
    }
    v = v->next[c];
    for (vector<int>::const_iterator it = v->accept.begin(); it != v->accept.end(); it++) {
      if (ret[*it] != -1) { continue; }
      ret[*it] = i - m[*it] + 1;
    }
  }
  return ret;
}
