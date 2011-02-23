struct Dice {
  int face[6];
  // +Z
  int Top() const { return face[0]; }
  int &Top() { return face[0]; }
  // -Z
  int Bottom() const { return face[1]; }
  int &Bottom() { return face[1]; }
  // +X
  int Right() const { return face[2]; }
  int &Right() { return face[2]; }
  // -X
  int Left() const { return face[3]; }
  int &Left() { return face[3]; }
  // +Y
  int Front() const { return face[4]; }
  int &Front() { return face[4]; }
  // -Y
  int Back() const { return face[5]; }
  int &Back() { return face[5]; }

  Dice Rotate(int cnt, int dir) const {
    if (cnt == 0) { return *this; }
    if (cnt < 0) { cnt = cnt % 4 + 4; }
    else { cnt %= 4; }
    Dice ret;
    if (dir == 0) {
      // +X rotate(Y axis rotate)
      ret.face[0] = face[3];
      ret.face[1] = face[2];
      ret.face[2] = face[0];
      ret.face[3] = face[1];
      ret.face[4] = face[4];
      ret.face[5] = face[5];
    } else if (dir == 1) {
      // +Y rotate(X axis rotate)
      ret.face[0] = face[4];
      ret.face[1] = face[5];
      ret.face[2] = face[2];
      ret.face[3] = face[3];
      ret.face[4] = face[1];
      ret.face[5] = face[0];
    } else if (dir == 2) {
      // Z rotate(Z axis rotate)
      ret.face[0] = face[0];
      ret.face[1] = face[1];
      ret.face[2] = face[4];
      ret.face[3] = face[5];
      ret.face[4] = face[3];
      ret.face[5] = face[2];
    } else {
      assert(false);
    }
    return ret.Rotate(cnt - 1, dir);
  }
  vector<Dice> AllRotate() const {
    vector<Dice> ret;
    Dice temp = *this;
    REP(i, 6) {
      REP(j, 4) {
        ret.push_back(temp);
        temp = temp.Rotate(1, 2);
      }
      temp = temp.Rotate(1, i & 1);
    }
    return ret;
  }
  bool operator==(const Dice &rhs) const {
    REP(i, 6) { if (face[i] != rhs.face[i]) { return false; } }
    return true;
  }
  bool operator<(const Dice &rhs) const {
    REP(i, 6) {
      if (face[i] != rhs.face[i]) { return face[i] < rhs.face[i]; }
    }
    return false;
  }
};
