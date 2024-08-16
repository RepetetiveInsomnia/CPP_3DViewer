#ifndef SRC_3DVIEWER_MAINWINDOW_H_
#define SRC_3DVIEWER_MAINWINDOW_H_

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPainter>
#include <QTimer>
#include <QToolButton>
#include <QWidget>

#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

  void parseFile();
  void writeConfig();
  void readConfig();
  void setUI();
  void setDefault();

  ~MainWindow();

private:
  void setColorButton(QToolButton *btn, QColor color);
private slots:
  void newFileButton();
  void writeGifButton();
  void screenshotButton();
  void backColorChange();
  void vertexColorChange();
  void edgeColorChange();

private:
  char *m_filename = NULL;
  Ui::MainWindow *ui;
  s21::Controller *controller_;
};
#endif // SRC_3DVIEWER_MAINWINDOW_H_
