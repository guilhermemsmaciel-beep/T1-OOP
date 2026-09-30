#ifndef GTOOLS_H
#define GTOOLS_H

struct Vector2 {
  float x;
  float y;

  Vector2() : x(0), y(0) {}
  Vector2(float n1, float n2) : x(n1), y(n2) {}
  void operator[](float n1, float n2) {
    x = n1;
    y = n2;
  }
}




#endif
