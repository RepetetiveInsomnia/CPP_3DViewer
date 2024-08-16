#include "tests.h"

TEST(Parcer, read) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    ASSERT_FLOAT_EQ(data.vertices_[X], data_2.vertices_[X]);
    ASSERT_FLOAT_EQ(data.vertices_[Y], data_2.vertices_[Y]);
    ASSERT_FLOAT_EQ(data.vertices_[Z], data_2.vertices_[Z]);
  }
}

TEST(Parcer, clean) {
  ParcedObj data;
  data.parseObjFile("./objFiles/test.obj");
  data.clearData();
  ASSERT_EQ(data.vertices_.empty(), true);
  ASSERT_EQ(data.faces_.empty(), true);
  ASSERT_EQ((int)data.vert_count_, 0);
  ASSERT_EQ((int)data.face_count_, 0);
}

TEST(Affine, move_x) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float cof = 0.1;
  data.move_x(cof);
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    ASSERT_FLOAT_EQ(data.vertices_[X], data_2.vertices_[X] + cof);
  }
}
TEST(Affine, move_y) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float cof = 0.1;
  data.move_y(cof);
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    ASSERT_FLOAT_EQ(data.vertices_[Y], data_2.vertices_[Y] + cof);
  }
}
TEST(Affine, move_z) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float cof = 0.1;
  data.move_z(cof);
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {

    ASSERT_FLOAT_EQ(data.vertices_[Z], data_2.vertices_[Z] + cof);
  }
}
TEST(Affine, rotate_x) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float cof = 0.1;
  float temp_y;
  float temp_z;
  data.rotation_by_ox(cof);
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    temp_y = data_2.vertices_[Y];
    temp_z = data_2.vertices_[Z];
    ASSERT_FLOAT_EQ(data.vertices_[Y], cos(cof) * temp_y - sin(cof) * temp_z);
    ASSERT_FLOAT_EQ(data.vertices_[Z], sin(cof) * temp_y + cos(cof) * temp_z);
  }
}
TEST(Affine, rotate_y) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float cof = 0.1;
  float temp_x;
  float temp_z;
  data.rotation_by_oy(cof);
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    temp_x = data_2.vertices_[X];
    temp_z = data_2.vertices_[Z];
    ASSERT_FLOAT_EQ(data.vertices_[X], sin(cof) * temp_z + cos(cof) * temp_x);
    ASSERT_FLOAT_EQ(data.vertices_[Z], cos(cof) * temp_z - sin(cof) * temp_x);
  }
}
TEST(Affine, rotate_z) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float cof = 0.1;
  float temp_x;
  float temp_y;
  data.rotation_by_oz(cof);
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    temp_x = data_2.vertices_[X];
    temp_y = data_2.vertices_[Y];
    ASSERT_FLOAT_EQ(data.vertices_[X], cos(cof) * temp_x - sin(cof) * temp_y);
    ASSERT_FLOAT_EQ(data.vertices_[Y], sin(cof) * temp_x + cos(cof) * temp_y);
  }
}
TEST(Affine, zoom) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float cof = 0.1;
  data.zoom_in_out(cof);
  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    ASSERT_FLOAT_EQ(data.vertices_[X], data_2.vertices_[X] * cof);
    ASSERT_FLOAT_EQ(data.vertices_[Y], data_2.vertices_[Y] * cof);
    ASSERT_FLOAT_EQ(data.vertices_[Z], data_2.vertices_[Z] * cof);
  }
}
TEST(Affine, middle) {
  ParcedObj data;
  ParcedObj data_2;
  data.parseObjFile("./objFiles/test.obj");
  data_2.parseObjFile("./objFiles/test.obj");
  float x_mid;
  float y_mid;
  float z_mid;
  data.find_middle(&x_mid, &y_mid, &z_mid);
  data.minus_middle(x_mid, y_mid, z_mid);
  data.plus_middle(x_mid, y_mid, z_mid);

  for (unsigned int m = 0; m < data.vert_count_ * 3; m += 3) {
    ASSERT_NEAR(data.vertices_[X], data_2.vertices_[X], 1e-4);
    ASSERT_NEAR(data.vertices_[Y], data_2.vertices_[Y], 1e-4);
    ASSERT_NEAR(data.vertices_[Z], data_2.vertices_[Z], 1e-4);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}