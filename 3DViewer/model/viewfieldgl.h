#ifndef SRC_3DVIEWER_UI_VIEWFIELDGL_VIEWFIELDGL_H_
#define SRC_3DVIEWER_UI_VIEWFIELDGL_VIEWFIELDGL_H_

#include <QColor>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QString>
//#include <vector>
#include "affine_ts.h"

#if defined(__APPLE__)

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif
// using namespace s21;

// namespace s21 {

typedef enum { PARALLEL, CENTRAL } eProjType;
typedef enum { SOLID, DOTTED } eEdgeType;
typedef enum { NONE, CIRCLE, SQUARE } eVertexType;

class ViewFieldGL : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  friend class s21::Controller;
  explicit ViewFieldGL(QWidget *parent = nullptr);
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int h) override;

  // -------------------- SET / GET --------------------

  virtual ~ViewFieldGL();

  // =============== slots ===============

protected:
  // get/set
  eEdgeType edgeType() const;
  eVertexType vertexType() const;
  eProjType projType() const;
  const QColor &bgColor() const;
  const QColor &edgeColor() const;
  const QColor &vertexColor() const;

  float xRot() const;
  float yRot() const;
  float zRot() const;

  float xOffst() const;
  float yOffst() const;
  float zOffst() const;
  float scale() const;

  void setDefault();
  double normalization() const;
  void toJson() const;
  void fromJson(const QJsonObject &json);

  double vertexSize() const;
  double edgeWidth() const;

protected slots:
  void alt_parcer(char *filename);

  void xOffsetChange(double val);
  void yOffsetChange(double val);
  void zOffsetChange(double val);

  void xRotChange(double val);
  void yRotChange(double val);
  void zRotChange(double val);

  void projectTypeChange(QString type);
  void vertexTypeChange(QString type);
  void edgeTypeChange(QString type);

  void vertexSizeChange(int size);
  void edgeWidthChange(int size);

  void backColorChange();
  void vertexColorChange();
  void edgeColorChange();

  void toNormilize();

  void cleanData();

  void scaleChange(int pos);

private:
  s21::ParcedObj n_data;

  float m_scale = 25;
  float m_zoom = 1;

  float m_xOffst = 0;
  float m_yOffst = 0;
  float m_zOffst = 0;
  float new_xOffst = 0;
  float new_yOffst = 0;
  float new_zOffst = 0;

  float m_xRot = 0;
  float m_yRot = 0;
  float m_zRot = 0;
  float new_xRot = 0;
  float new_yRot = 0;
  float new_zRot = 0;

  float x_middle = 0;
  float y_middle = 0;
  float z_middle = 0;

  float m_normalization = 10;
  float sizeW, sizeH;

  float m_vertexSize = 0.1;
  float m_edgeWidth = 0.1;
  unsigned int max_v_value = 1000000;
  int *my_vector;
  unsigned int count_of_f = 0;
  eEdgeType m_edgeType = SOLID;
  eVertexType m_vertexType = CIRCLE;
  eProjType m_projType = CENTRAL;
  QColor m_bgColor = QColor(0xA0, 0xA0, 0xA0);
  QColor m_edgeColor = QColor(0xFF, 0xFF, 0xFF);
  QColor m_vertexColor = QColor(0xFF, 0xAA, 0xFF);

  void paintEdges();
  void paintVertexes();
  void printDataParameters(QPainter &pntr);
  char *filename() const;
  void setFilename(char *newFilename);
  void setNormalization(double newNormalization);
};
//} // namespace s21
#endif // SRC_3DVIEWER_UI_VIEWFIELDGL_VIEWFIELDGL_H_
