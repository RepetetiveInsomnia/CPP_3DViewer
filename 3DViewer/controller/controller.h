#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#pragma once

#include <cmath>
#include <cstdarg>
#include <map>
#include <string>

#include "../model/viewfieldgl.h"
//#include "../view/mainwindow.h"
namespace s21 {

class Controller {
public:
  friend class ViewFieldGL;

  void clean(ViewFieldGL *widget);
  // get
  float get_scale(ViewFieldGL *widget);
  float get_x_offset(ViewFieldGL *widget);
  float get_y_offset(ViewFieldGL *widget);
  float get_z_offset(ViewFieldGL *widget);
  float get_x_rot(ViewFieldGL *widget);
  float get_y_rot(ViewFieldGL *widget);
  float get_z_rot(ViewFieldGL *widget);
  float get_vertex_size(ViewFieldGL *widget);
  float get_edge_width(ViewFieldGL *widget);
  eProjType get_project_type(ViewFieldGL *widget);
  eVertexType get_vertex_type(ViewFieldGL *widget);
  eEdgeType get_edge_type(ViewFieldGL *widget);
  QColor get_bg_colour(ViewFieldGL *widget);
  QColor get_vx_colour(ViewFieldGL *widget);
  QColor get_eg_colour(ViewFieldGL *widget);
  // set
  void set_scale(ViewFieldGL *widget, int pos);
  void set_x_offset(ViewFieldGL *widget, double pos);
  void set_y_offset(ViewFieldGL *widget, double pos);
  void set_z_offset(ViewFieldGL *widget, double pos);
  void set_x_rot(ViewFieldGL *widget, double pos);
  void set_y_rot(ViewFieldGL *widget, double pos);
  void set_z_rot(ViewFieldGL *widget, double pos);
  void set_project_type(ViewFieldGL *widget, QString type);
  void set_vertex_type(ViewFieldGL *widget, QString type);
  void set_edge_type(ViewFieldGL *widget, QString type);
  void set_vertex_size(ViewFieldGL *widget, int pos);
  void set_edge_width(ViewFieldGL *widget, int pos);
  void set_bg_color(ViewFieldGL *widget);
  void set_vx_color(ViewFieldGL *widget);
  void set_eg_color(ViewFieldGL *widget);
  void set_default(ViewFieldGL *widget);
  void clean_data(ViewFieldGL *widget);
  void parse_file(char *filename, ViewFieldGL *widget);
  void normilize(ViewFieldGL *widget);

  void update(ViewFieldGL *widget);
  double normal_value(ViewFieldGL *widget) const;
  void to_json(ViewFieldGL *widget);
  void from_json(const QJsonObject &json, ViewFieldGL *widget);

private:
  // friend class ViewFieldGL;
};
} // namespace s21

#endif // CONTROLLER_H_
