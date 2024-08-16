#include "affine_ts.h"

void s21::ParcedObj::clearData() {
  if (!vertices_.empty()) {
    vertices_.clear();
  }
  if (!faces_.empty()) {
    faces_.clear();
  }

  vert_count_ = 0;
  face_count_ = 0;
}

void s21::ParcedObj::parseObjFile(const std::string &filename) {
  std::ifstream objFile(filename);

  if (!objFile.is_open()) {
    std::cerr << "Не удалось открыть файл: " << filename << std::endl;
    return;
  }

  std::string line;

  while (std::getline(objFile, line)) {
    std::istringstream iss(line);
    std::string token;

    iss >> token;

    if (token == "v") {
      std::array<float, 3> vertex;
      iss >> vertex[0] >> vertex[1] >> vertex[2];
      vertices_.push_back(vertex[0]);
      vertices_.push_back(vertex[1]);
      vertices_.push_back(vertex[2]);

      vert_count_++;
    } else if (token == "f") {
      std::vector<int> edge;

      while (iss >> token) {
        int vertexIndex = std::stoi(token);
        if (vertexIndex < 0) {
          vertexIndex = vert_count_ + 1 + vertexIndex;
        }
        if (vertexIndex < 0) {
          vertexIndex *= -1;
        }
        edge.push_back(vertexIndex);
      }

      for (auto it = edge.begin(); it != edge.end(); it++) {
        faces_.push_back(*it - 1);
        if (it + 1 != edge.end())
          faces_.push_back(*(it + 1) - 1);
        else
          faces_.push_back(*(edge.begin()) - 1);

        if (faces_.back() < 0) {
        }
      }
      face_count_++;
    }
  }

  objFile.close();
}

void s21::ParcedObj::move_x(float a) {
  for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
    this->vertices_[X] += a;
  }
}

void s21::ParcedObj::move_y(float a) {
  for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
    this->vertices_[Y] += a;
  }
}

void s21::ParcedObj::move_z(float a) {
  for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
    this->vertices_[Z] += a;
  }
}

void s21::ParcedObj::zoom_in_out(float a) {
  if (a > 0) {
    for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
      this->vertices_[X] *= a;
      this->vertices_[Y] *= a;
      this->vertices_[Z] *= a;
    }
  }
}
void s21::ParcedObj::find_middle(float *x_mid, float *y_mid, float *z_mid) {
  if (!this->vertices_.empty()) {
    double upper_x = this->vertices_[0];
    double lower_x = this->vertices_[0];
    double upper_y = this->vertices_[1];
    double lower_y = this->vertices_[1];
    double upper_z = this->vertices_[2];
    double lower_z = this->vertices_[2];
    for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
      if (this->vertices_[X] <= lower_x) {
        lower_x = this->vertices_[X];
      }
      if (this->vertices_[X] >= upper_x) {
        upper_x = this->vertices_[X];
      }
      if (this->vertices_[Y] <= lower_y) {
        lower_y = this->vertices_[Y];
      }
      if (this->vertices_[Y] >= upper_y) {
        upper_y = this->vertices_[Y];
      }
      if (this->vertices_[Z] <= lower_z) {
        lower_z = this->vertices_[Z];
      }
      if (this->vertices_[Z] >= upper_z) {
        upper_z = this->vertices_[Z];
      }
    }
    *x_mid = (upper_x + lower_x) / 2;
    *y_mid = (upper_y + lower_y) / 2;
    *z_mid = (upper_z + lower_z) / 2;
  }
}

void s21::ParcedObj::minus_middle(float x_mid, float y_mid, float z_mid) {
  for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
    this->vertices_[X] = this->vertices_[X] - x_mid;
    this->vertices_[Y] = this->vertices_[Y] - y_mid;
    this->vertices_[Z] = this->vertices_[Z] - z_mid;
  }
}

void s21::ParcedObj::plus_middle(float x_mid, float y_mid, float z_mid) {
  for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
    this->vertices_[X] = this->vertices_[X] + x_mid;
    this->vertices_[Y] = this->vertices_[Y] + y_mid;
    this->vertices_[Z] = this->vertices_[Z] + z_mid;
  }
}

void s21::ParcedObj::rotation_by_ox(float angle) {
  if (angle != 0) {
    for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
      float temp_y = this->vertices_[Y];
      float temp_z = this->vertices_[Z];
      this->vertices_[Y] = cos(angle) * temp_y - sin(angle) * temp_z;
      this->vertices_[Z] = sin(angle) * temp_y + cos(angle) * temp_z;
    }
  }
}

void s21::ParcedObj::rotation_by_oy(float angle) {
  if (angle != 0) {
    for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
      float temp_x = this->vertices_[X];
      float temp_z = this->vertices_[Z];
      this->vertices_[X] = sin(angle) * temp_z + cos(angle) * temp_x;
      this->vertices_[Z] = cos(angle) * temp_z - sin(angle) * temp_x;
    }
  }
}

void s21::ParcedObj::rotation_by_oz(float angle) {
  if (angle != 0) {
    for (unsigned int m = 0; m < this->vert_count_ * 3; m += 3) {
      float temp_x = this->vertices_[X];
      float temp_y = this->vertices_[Y];
      this->vertices_[X] = cos(angle) * temp_x - sin(angle) * temp_y;
      this->vertices_[Y] = sin(angle) * temp_x + cos(angle) * temp_y;
    }
  }
}
