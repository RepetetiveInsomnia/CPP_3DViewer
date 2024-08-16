#include "controller.h"

#include <sstream>
// namespace s21 {
void s21::Controller::clean(ViewFieldGL *widget) { widget->cleanData(); }
// get
float s21::Controller::get_scale(ViewFieldGL *widget) {
  return widget->scale();
}
float s21::Controller::get_x_offset(ViewFieldGL *widget) {
  return widget->xOffst();
}
float s21::Controller::get_y_offset(ViewFieldGL *widget) {
  return widget->yOffst();
}
float s21::Controller::get_z_offset(ViewFieldGL *widget) {
  return widget->zOffst();
}
float s21::Controller::get_x_rot(ViewFieldGL *widget) { return widget->xRot(); }
float s21::Controller::get_y_rot(ViewFieldGL *widget) { return widget->yRot(); }
float s21::Controller::get_z_rot(ViewFieldGL *widget) { return widget->zRot(); }
float s21::Controller::get_vertex_size(ViewFieldGL *widget) {
  return widget->vertexSize();
}
float s21::Controller::get_edge_width(ViewFieldGL *widget) {
  return widget->edgeWidth();
}
eProjType s21::Controller::get_project_type(ViewFieldGL *widget) {
  return widget->projType();
}
eVertexType s21::Controller::get_vertex_type(ViewFieldGL *widget) {
  return widget->vertexType();
}
eEdgeType s21::Controller::get_edge_type(ViewFieldGL *widget) {
  return widget->edgeType();
}
QColor s21::Controller::get_bg_colour(ViewFieldGL *widget) {
  return widget->bgColor();
}
QColor s21::Controller::get_vx_colour(ViewFieldGL *widget) {
  return widget->vertexColor();
}
QColor s21::Controller::get_eg_colour(ViewFieldGL *widget) {
  return widget->edgeColor();
}
// set
void s21::Controller::set_scale(ViewFieldGL *widget, int pos) {
  widget->scaleChange(pos);
}
void s21::Controller::set_x_offset(ViewFieldGL *widget, double pos) {
  widget->xOffsetChange(pos);
}
void s21::Controller::set_y_offset(ViewFieldGL *widget, double pos) {
  widget->yOffsetChange(pos);
}
void s21::Controller::set_z_offset(ViewFieldGL *widget, double pos) {
  widget->zOffsetChange(pos);
}
void s21::Controller::set_x_rot(ViewFieldGL *widget, double pos) {
  widget->xRotChange(pos);
}
void s21::Controller::set_y_rot(ViewFieldGL *widget, double pos) {
  widget->yRotChange(pos);
}
void s21::Controller::set_z_rot(ViewFieldGL *widget, double pos) {
  widget->zRotChange(pos);
}
void s21::Controller::set_project_type(ViewFieldGL *widget, QString type) {
  widget->projectTypeChange(type);
}
void s21::Controller::set_vertex_type(ViewFieldGL *widget, QString type) {
  widget->vertexTypeChange(type);
}
void s21::Controller::set_edge_type(ViewFieldGL *widget, QString type) {
  widget->edgeTypeChange(type);
}
void s21::Controller::set_vertex_size(ViewFieldGL *widget, int pos) {
  widget->vertexSizeChange(pos);
}
void s21::Controller::set_edge_width(ViewFieldGL *widget, int pos) {
  widget->edgeWidthChange(pos);
}
void s21::Controller::set_bg_color(ViewFieldGL *widget) {
  widget->backColorChange();
}
void s21::Controller::set_vx_color(ViewFieldGL *widget) {
  widget->vertexColorChange();
}
void s21::Controller::set_eg_color(ViewFieldGL *widget) {
  widget->edgeColorChange();
}
// other
void s21::Controller::clean_data(ViewFieldGL *widget) { widget->cleanData(); }
void s21::Controller::parse_file(char *filename, ViewFieldGL *widget) {
  // parcer(filename, widget->data());
  widget->alt_parcer(filename);

  // new_parcer::parseObjFile(filename);
}
void s21::Controller::set_default(ViewFieldGL *widget) { widget->setDefault(); }
void s21::Controller::update(ViewFieldGL *widget) { widget->update(); }
double s21::Controller::normal_value(ViewFieldGL *widget) const {
  return widget->normalization();
}
void s21::Controller::normilize(ViewFieldGL *widget) { widget->toNormilize(); }
void s21::Controller::to_json(ViewFieldGL *widget) { widget->toJson(); }
void s21::Controller::from_json(const QJsonObject &json, ViewFieldGL *widget) {
  widget->fromJson(json);
}
//} // namespace s21
