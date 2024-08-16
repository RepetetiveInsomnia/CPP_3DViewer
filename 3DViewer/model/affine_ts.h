#ifndef AFFINE_TS_H
#define AFFINE_TS_H
#include <math.h>

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#define X m
#define Y m + 1
#define Z m + 2
namespace s21 {
class Controller;
class ParcedObj {
public:
  void clearData();
  void parseObjFile(const std::string &filename);
  std::vector<float> vertices_;
  std::vector<int> faces_;
  unsigned int vert_count_ = 0;
  unsigned int face_count_ = 0;
  void move_x(float a);
  void move_y(float a);
  void move_z(float a);
  void find_middle(float *x_mid, float *y_mid, float *z_mid);
  void minus_middle(float x_mid, float y_mid, float z_mid);
  void plus_middle(float x_mid, float y_mid, float z_mid);
  void zoom_in_out(float a);
  void rotation_by_ox(float angle);
  void rotation_by_oy(float angle);
  void rotation_by_oz(float angle);
};
} // namespace s21
#endif // AFFINE_TS_H
