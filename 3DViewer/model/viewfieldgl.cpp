#include "viewfieldgl.h"

#include <QColorDialog>
#include <QPainter>

static float deg2rad(float deg) { return deg * M_PI / 180.0; }

ViewFieldGL::ViewFieldGL(QWidget *parent) : QOpenGLWidget(parent) {}

void ViewFieldGL::setDefault() {
  m_scale = 25;
  m_zoom = 1;

  m_xOffst = 0;
  m_yOffst = 0;
  m_zOffst = 0;
  new_xOffst = 0;
  new_yOffst = 0;
  new_zOffst = 0;

  m_xRot = 0;
  m_yRot = 0;
  m_zRot = 0;
  new_xRot = 0;
  new_yRot = 0;
  new_zRot = 0;

  x_middle = 0;
  y_middle = 0;
  z_middle = 0;
}

void ViewFieldGL::initializeGL() {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ < max_v_value) {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); // z coord buffer
  }
}

void ViewFieldGL::paintGL() {
  QPainter pntr(this);
  pntr.beginNativePainting();
  // background setting
  glClearColor(m_bgColor.redF(), m_bgColor.greenF(), m_bgColor.blueF(), 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ < max_v_value) {
    /*
  move_x(&m_data, m_xOffst);
  move_y(&m_data, m_yOffst);
  move_z(&m_data, m_zOffst);
  find_middle(&m_data, &x_middle, &y_middle, &z_middle);
  minus_middle(&m_data, x_middle, y_middle, z_middle);

  rotation_by_ox(&m_data, deg2rad(m_xRot));
  rotation_by_oy(&m_data, deg2rad(m_yRot));
  rotation_by_oz(&m_data, deg2rad(m_zRot));
  zoom_in_out(&m_data, m_zoom);
  plus_middle(&m_data, x_middle, y_middle, z_middle);
*/
    n_data.move_x(m_xOffst);
    n_data.move_y(m_yOffst);
    n_data.move_z(m_zOffst);
    n_data.find_middle(&x_middle, &y_middle, &z_middle);
    n_data.minus_middle(x_middle, y_middle, z_middle);
    n_data.rotation_by_ox(deg2rad(m_xRot));
    n_data.rotation_by_oy(deg2rad(m_yRot));
    n_data.rotation_by_oz(deg2rad(m_zRot));
    n_data.zoom_in_out(m_zoom);
    n_data.plus_middle(x_middle, y_middle, z_middle);

    m_zoom = 0;
    m_xOffst = 0;
    m_yOffst = 0;
    m_zOffst = 0;
    m_xRot = 0;
    m_yRot = 0;
    m_zRot = 0;
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (m_projType == CENTRAL) {
    gluPerspective(90.0, sizeW / sizeH, 0.0001, 1000 * m_normalization);
    glTranslated(0.0, 0.0, -2 * m_normalization);
    glRotatef(30, 1, 0, 0);
  } else {
    glOrtho(-sizeW / sizeH * 2 * m_normalization,
            sizeW / sizeH * 2 * m_normalization, -2 * m_normalization,
            2 * m_normalization, -100 * m_normalization,
            1000 * m_normalization);
    glTranslated(0, -m_normalization / 2, 0);
    glRotatef(10, 1, 1, 1);
  }

  glEnableClientState(GL_VERTEX_ARRAY);

  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    glTranslatef(m_xOffst, m_yOffst, m_zOffst);
    glRotatef(m_xRot, 1, 0, 0);
    glRotatef(m_yRot, 0, 1, 0);
    glRotatef(m_zRot, 0, 0, 1);
    glScalef(m_zoom, m_zoom, m_zoom);
  }
  // glVertexPointer(3, GL_DOUBLE, 0, m_data.vertexes_arr);

  glVertexPointer(3, GL_FLOAT, 0, n_data.vertices_.data());

  paintEdges();
  if (this->m_vertexType != NONE) {
    paintVertexes();
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  pntr.endNativePainting();
  printDataParameters(pntr);
}

void ViewFieldGL::resizeGL(int w, int h) {
  sizeW = w;
  sizeH = h;
  glViewport(0, 0, ((w) / 2), ((h) / 2));
  update();
}

void ViewFieldGL::toJson() const {

  // QJsonObject json;
  QFile file(".config.json");
  if (file.open(QIODevice::WriteOnly | QFile::Text)) {
    QJsonObject projObject;
    projObject["vertexSize"] = m_vertexSize;
    projObject["edgeWidth"] = m_edgeWidth;
    projObject["edgeType"] = (int)m_edgeType;
    projObject["vertexType"] = (int)m_vertexType;
    projObject["projType"] = (int)m_projType;
    projObject["bgColor"] =
        (m_bgColor.red() << 16) | (m_bgColor.green() << 8) | m_bgColor.blue();
    projObject["edgeColor"] = (m_edgeColor.red() << 16) |
                              (m_edgeColor.green() << 8) | m_edgeColor.blue();
    projObject["vertexColor"] = (m_vertexColor.red() << 16) |
                                (m_vertexColor.green() << 8) |
                                m_vertexColor.blue();

    file.write(QJsonDocument(projObject).toJson());
    file.close();
  }

  // return projObject;
}

void ViewFieldGL::fromJson(const QJsonObject &json) {

  m_vertexSize = json["vertexSize"].toDouble();
  m_edgeWidth = json["edgeWidth"].toDouble();
  m_edgeType = (eEdgeType)json["edgeType"].toInt();
  m_vertexType = (eVertexType)json["vertexType"].toInt();
  m_projType = (eProjType)json["projType"].toInt();

  int bgClrTotal = json["bgColor"].toInt();
  int edgeClrTotal = json["edgeColor"].toInt();
  int vrtxClrTotal = json["vertexColor"].toInt();

  m_bgColor = QColor((bgClrTotal & 0xff0000) >> 16, (bgClrTotal & 0xff00) >> 8,
                     (bgClrTotal & 0xff));
  m_edgeColor = QColor((edgeClrTotal & 0xff0000) >> 16,
                       (edgeClrTotal & 0xff00) >> 8, (edgeClrTotal & 0xff));
  m_vertexColor = QColor((vrtxClrTotal & 0xff0000) >> 16,
                         (vrtxClrTotal & 0xff00) >> 8, (vrtxClrTotal & 0xff));
}

ViewFieldGL::~ViewFieldGL() { cleanData(); }

// ========= public slots =========
void ViewFieldGL::scaleChange(int pos) {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    if (m_scale > pos) {
      m_zoom = m_zoom * powf(0.93, m_scale - pos);
    } else if (m_scale < pos) {
      m_zoom = m_zoom * powf(1.07, (float)pos - m_scale);
    }
  } else {
    if (m_scale > pos) {
      m_zoom = powf(0.95, m_scale - pos);
    } else if (m_scale < pos) {
      m_zoom = powf(1.05, (double)pos - m_scale);
    }
  }
  m_scale = pos;
  update();
}

void ViewFieldGL::xOffsetChange(double val) {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    m_xOffst = val;
  } else {
    m_xOffst = val - new_xOffst;

    if (new_xOffst != val) {
      new_xOffst = val;
    }
  }
  update();
}

void ViewFieldGL::yOffsetChange(double val) {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    m_yOffst = val;
  } else {
    m_yOffst = val - new_yOffst;

    if (new_yOffst != val) {
      new_yOffst = val;
    }
  }
  update();
}

void ViewFieldGL::zOffsetChange(double val) {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    m_zOffst = val;
  } else {
    m_zOffst = val - new_zOffst;

    if (new_zOffst != val) {
      new_zOffst = val;
    }
  }
  update();
}

void ViewFieldGL::xRotChange(double val) {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    m_xRot = val;
  } else {
    m_xRot = val - new_xRot;

    if (new_xRot != val) {
      new_xRot = val;
    }
  }
  update();
}

void ViewFieldGL::yRotChange(double val) {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    m_yRot = val;
  } else {
    m_yRot = val - new_yRot;

    if (new_yRot != val) {
      new_yRot = val;
    }
  }
  update();
}

void ViewFieldGL::zRotChange(double val) {
  if (/*m_data.count_of_vertexes*/ n_data.vert_count_ >= max_v_value) {
    m_zRot = val;
  } else {
    m_zRot = val - new_zRot;

    if (new_zRot != val) {
      new_zRot = val;
    }
  }
  update();
}

void ViewFieldGL::projectTypeChange(QString type) {
  if (type == QString("Parallel")) {
    m_projType = PARALLEL;
  } else {
    m_projType = CENTRAL;
  }
  update();
}

void ViewFieldGL::vertexTypeChange(QString type) {
  if (type == QString("None")) {
    m_vertexType = NONE;
  } else if (type == QString("Circle")) {
    m_vertexType = CIRCLE;
  } else {
    m_vertexType = SQUARE;
  }
  update();
}

void ViewFieldGL::edgeTypeChange(QString type) {
  if (type == QString("Solid")) {
    m_edgeType = SOLID;
  } else {
    m_edgeType = DOTTED;
  }
  update();
}

void ViewFieldGL::vertexSizeChange(int size) {
  m_vertexSize = size ? (double)size / 15 : 0.01;
  ;
  update();
}

void ViewFieldGL::edgeWidthChange(int size) {
  m_edgeWidth = size ? (double)size / 40 : 0.01;
  update();
}

void ViewFieldGL::backColorChange() {
  QColor newColor =
      QColorDialog::getColor(Qt::white, this, tr("Select Background Color"));

  if (newColor.isValid()) {
    m_bgColor = newColor;
  }
  update();
}

void ViewFieldGL::vertexColorChange() {
  QColor newColor =
      QColorDialog::getColor(Qt::white, this, tr("Select Background Color"));

  if (newColor.isValid()) {
    m_vertexColor = newColor;
  }
  update();
}

void ViewFieldGL::edgeColorChange() {
  QColor newColor =
      QColorDialog::getColor(Qt::white, this, tr("Select Background Color"));

  if (newColor.isValid()) {
    m_edgeColor = newColor;
  }
  update();
}

void ViewFieldGL::toNormilize() {
  m_normalization = 0.0000001;
  /*
    for (int i = 0; i < (int)m_data.count_of_vertexes * 3; i++) {
      double value = abs(m_data.vertexes_arr[i]);
      m_normalization = (value > m_normalization) ? value : m_normalization;
    }
  */
  for (int i = 0; i < (int)n_data.vert_count_ * 3; i++) {
    float value = abs(n_data.vertices_[i]);
    m_normalization = (value > m_normalization) ? value : m_normalization;
  }
}

void ViewFieldGL::paintEdges() {
  if (m_edgeType == DOTTED) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xF00F);
  }
  glLineWidth(m_edgeWidth);
  glColor3f(m_edgeColor.redF(), m_edgeColor.greenF(), m_edgeColor.blueF());
  /*
    glDrawElements(GL_LINES, m_data.oneLine_size, GL_UNSIGNED_INT,
                   m_data.oneLine_array);
  */

  glDrawElements(GL_LINES, n_data.faces_.size(), GL_UNSIGNED_INT,
                 n_data.faces_.data());

  if (m_edgeType == DOTTED) {
    glDisable(GL_LINE_STIPPLE);
  }
}

void ViewFieldGL::paintVertexes() {
  if (this->m_vertexType == CIRCLE) {
    glEnable(GL_POINT_SMOOTH);
  }

  glPointSize(m_vertexSize);
  glColor3f(m_vertexColor.redF(), m_vertexColor.greenF(),
            m_vertexColor.blueF());
  glDrawArrays(GL_POINTS, 0, n_data.vert_count_);
  if (this->m_vertexType == CIRCLE) {
    glDisable(GL_POINT_SMOOTH);
  }
}

void ViewFieldGL::printDataParameters(QPainter &pntr) {
  double h = height();
  double boxW = 250;
  double boxH = 80;
  double textOffset = 15;

  QPoint botL = QPoint(0, h);
  QPoint topR = QPoint(boxW, h - boxH);
  QRectF textBox = QRect(botL, topR);
  QBrush brush = QBrush(QColor(100, 100, 100, 100));
  pntr.fillRect(textBox, QColor(100, 100, 100, 100));

  QString vrtxMsg =
      QString("Number of vertexes:     ") + QString::number(n_data.vert_count_);
  QString facetsMsg = QString("Number of facets:         ") +
                      QString::number(n_data.face_count_);
  pntr.setPen(Qt::white);
  pntr.drawText(QPoint(textOffset, h - (boxH - 2 * textOffset)), vrtxMsg);
  pntr.drawText(QPoint(textOffset, h - (boxH - 4 * textOffset + 5)), facetsMsg);
}

double ViewFieldGL::vertexSize() const { return m_vertexSize; }

double ViewFieldGL::edgeWidth() const { return m_edgeWidth; }

double ViewFieldGL::normalization() const { return m_normalization; }

void ViewFieldGL::setNormalization(double newNormalization) {
  m_normalization = newNormalization;
}

void ViewFieldGL::alt_parcer(char *filename) {

  const std::string path(filename);
  n_data.parseObjFile(path);
}

void ViewFieldGL::cleanData() { n_data.clearData(); }

float ViewFieldGL::xRot() const { return m_xRot; }

float ViewFieldGL::yRot() const { return m_yRot; }

float ViewFieldGL::zRot() const { return m_zRot; }

float ViewFieldGL::xOffst() const { return m_xOffst; }

float ViewFieldGL::yOffst() const { return m_yOffst; }

float ViewFieldGL::zOffst() const { return m_zOffst; }

float ViewFieldGL::scale() const { return m_scale; }

const QColor &ViewFieldGL::bgColor() const { return m_bgColor; }

const QColor &ViewFieldGL::edgeColor() const { return m_edgeColor; }

const QColor &ViewFieldGL::vertexColor() const { return m_vertexColor; }

eEdgeType ViewFieldGL::edgeType() const { return m_edgeType; }

eVertexType ViewFieldGL::vertexType() const { return m_vertexType; }

eProjType ViewFieldGL::projType() const { return m_projType; }
