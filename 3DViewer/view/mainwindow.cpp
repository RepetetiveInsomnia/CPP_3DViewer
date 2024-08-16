#include "mainwindow.h"

#include <QDateTime>

#include "ui_mainwindow.h"

extern "C" {
#include "3rdparty/gif.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("3DViewer v2.0");
  readConfig();

  QObject::connect(ui->newFileButton, &QPushButton::clicked, this,
                   &MainWindow::newFileButton);
  QObject::connect(ui->gifButton, &QPushButton::clicked, this,
                   &MainWindow::writeGifButton);
  QObject::connect(ui->screenshotButton, &QPushButton::clicked, this,
                   &MainWindow::screenshotButton);

  // GLWIdget slots to controller ui signals
  QObject::connect(ui->scaleSlider, &QSlider::valueChanged, this, [this]() {
    controller_->set_scale(ui->openGLWidget, ui->scaleSlider->value());
  });

  QObject::connect(
      ui->xOffstSpBox, &QDoubleSpinBox::valueChanged, this, [this]() {
        controller_->set_x_offset(ui->openGLWidget, ui->xOffstSpBox->value());
      });
  QObject::connect(
      ui->yOffstSpBox, &QDoubleSpinBox::valueChanged, this, [this]() {
        controller_->set_y_offset(ui->openGLWidget, ui->yOffstSpBox->value());
      });
  QObject::connect(
      ui->zOffstSpBox, &QDoubleSpinBox::valueChanged, this, [this]() {
        controller_->set_z_offset(ui->openGLWidget, ui->zOffstSpBox->value());
      });

  QObject::connect(
      ui->xRotSpBox, &QDoubleSpinBox::valueChanged, this, [this]() {
        controller_->set_x_rot(ui->openGLWidget, ui->xRotSpBox->value());
      });
  QObject::connect(
      ui->yRotSpBox, &QDoubleSpinBox::valueChanged, this, [this]() {
        controller_->set_y_rot(ui->openGLWidget, ui->yRotSpBox->value());
      });
  QObject::connect(
      ui->zRotSpBox, &QDoubleSpinBox::valueChanged, this, [this]() {
        controller_->set_z_rot(ui->openGLWidget, ui->zRotSpBox->value());
      });
  QObject::connect(
      ui->projectionTypeCmbBox, &QComboBox::currentTextChanged, this, [this]() {
        controller_->set_project_type(ui->openGLWidget,
                                      ui->projectionTypeCmbBox->currentText());
      });
  QObject::connect(ui->vrtxShapeCmbBox, &QComboBox::currentTextChanged, this,
                   [this]() {
                     controller_->set_vertex_type(
                         ui->openGLWidget, ui->vrtxShapeCmbBox->currentText());
                   });
  QObject::connect(ui->edgesTypeCmbBox, &QComboBox::currentTextChanged, this,
                   [this]() {
                     controller_->set_edge_type(
                         ui->openGLWidget, ui->edgesTypeCmbBox->currentText());
                   });
  QObject::connect(ui->vrtxSizeSlider, &QSlider::valueChanged, this, [this]() {
    controller_->set_vertex_size(ui->openGLWidget, ui->vrtxSizeSlider->value());
  });
  QObject::connect(ui->edgesWidthSlider, &QSlider::valueChanged, this,
                   [this]() {
                     controller_->set_edge_width(ui->openGLWidget,
                                                 ui->edgesWidthSlider->value());
                   });

  QObject::connect(ui->bgClrButton, &QToolButton::clicked, this,
                   &MainWindow::backColorChange);
  QObject::connect(ui->vrtxColorButton, &QToolButton::clicked, this,
                   &MainWindow::vertexColorChange);
  QObject::connect(ui->edgesColorButton, &QToolButton::clicked, this,
                   &MainWindow::edgeColorChange);
}
void MainWindow::parseFile() {
  controller_->clean_data(ui->openGLWidget);
  if (*m_filename == '\0' || !m_filename) {
    ui->filePathLabel->setText("File is not choosen");
  } else {
    std::locale prev_locale = std::locale::global(std::locale::classic());
    controller_->parse_file(m_filename, ui->openGLWidget);

    std::locale::global(prev_locale);
    setDefault();
    controller_->normilize(ui->openGLWidget);
    double step = controller_->normal_value(ui->openGLWidget) / (float)20;
    ui->xOffstSpBox->setSingleStep(step);
    ui->yOffstSpBox->setSingleStep(step);
    ui->zOffstSpBox->setSingleStep(step);
  }
}

void MainWindow::writeConfig() { controller_->to_json(ui->openGLWidget); }

void MainWindow::readConfig() {
  QFile file(".config.json");
  if (file.open(QIODevice::ReadOnly | QFile::Text)) {
    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    controller_->from_json(loadDoc.object(), ui->openGLWidget);
    file.close();
    controller_->update(ui->openGLWidget);
  }
  setUI();
}

void MainWindow::setUI() {
  auto glW = ui->openGLWidget;
  ui->scaleSlider->setValue(controller_->get_scale(glW));
  ui->vrtxSizeSlider->setValue(controller_->get_vertex_size(glW));
  ui->edgesWidthSlider->setValue(controller_->get_edge_width(glW));
  if (controller_->get_edge_type(glW) == SOLID) {
    ui->edgesTypeCmbBox->setCurrentText("Solid");
  } else {
    ui->edgesTypeCmbBox->setCurrentText("Dotted");
  }

  if (controller_->get_vertex_type(glW) == NONE) {
    ui->vrtxShapeCmbBox->setCurrentText("None");
  } else if (controller_->get_vertex_type(glW) == CIRCLE) {
    ui->vrtxShapeCmbBox->setCurrentText("Circle");
  } else {
    ui->vrtxShapeCmbBox->setCurrentText("Square");
  }

  if (controller_->get_project_type(glW) == PARALLEL) {
    ui->projectionTypeCmbBox->setCurrentText("Parallel");
  } else {
    ui->projectionTypeCmbBox->setCurrentText("Central");
  }

  ui->xRotSpBox->setValue(controller_->get_x_rot(glW));
  ui->yRotSpBox->setValue(controller_->get_y_rot(glW));
  ui->zRotSpBox->setValue(controller_->get_z_rot(glW));

  ui->xOffstSpBox->setValue(controller_->get_x_offset(glW));
  ui->yOffstSpBox->setValue(controller_->get_y_offset(glW));
  ui->zOffstSpBox->setValue(controller_->get_z_offset(glW));

  setColorButton(ui->bgClrButton, controller_->get_bg_colour(glW));
  setColorButton(ui->vrtxColorButton, controller_->get_vx_colour(glW));
  setColorButton(ui->edgesColorButton, controller_->get_eg_colour(glW));
}

void MainWindow::setDefault() {
  controller_->set_default(ui->openGLWidget);
  setUI();
}

MainWindow::~MainWindow() {
  writeConfig();
  delete ui;
}

void MainWindow::setColorButton(QToolButton *btn, QColor color) {
  QPixmap px(20, 20);
  px.fill(color);
  btn->setIcon(px);
}

void MainWindow::newFileButton() {
  QFile inputFile(QFileDialog::getOpenFileName(
      this, tr("Open File"), "", tr("Object files (*.obj)"), 0,
      QFileDialog::DontUseNativeDialog));
  // QFileDialog::DontUseNativeDialog;
  QFileInfo info(inputFile);
  QString absoluteFilePath = info.absoluteFilePath();

  QByteArray byteArray = absoluteFilePath.toLocal8Bit();
  if (byteArray.size() && QFile::exists(absoluteFilePath)) {
    m_filename = byteArray.data();
    ui->filePathLabel->setText(info.fileName());
    parseFile();
    controller_->update(ui->openGLWidget);
  }
}

void MainWindow::writeGifButton() {
  QString temp = QCoreApplication::applicationDirPath();
  // TODO: write splitting by "/" from end to 3DViewer
  temp.chop(9);
  QString timeStr =
      QDateTime::currentDateTime().toString("yyyy-MM-dd_hh.mm.ss");
  QString initName = temp + "data-samples/" + "3DV_gif_" + timeStr + ".gif";
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save gif"), initName, tr("Gif files (*.gif)"), 0,
      QFileDialog::DontUseNativeDialog);

  if (!fileName.isEmpty()) {
    QImage img = ui->openGLWidget->grabFramebuffer();
    QPainter painter(&img);
    QImage img640_480;
    QTime dieTime;
    int width = 640;
    int height = 480;
    GifWriter gif;
    QByteArray ba = fileName.toLocal8Bit();
    const char *c_str = ba.data();
    GifBegin(&gif, c_str, width, height, 5);

    for (int i = 0; i < 85; ++i) {
      ui->openGLWidget->render(&painter);
      img640_480 = img.scaled(QSize(640, 480));
      GifWriteFrame(&gif, img640_480.bits(), width, height, 6);
      dieTime = QTime::currentTime().addMSecs(100);
      while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    GifEnd(&gif);
    QMessageBox msgBox;
    msgBox.setText("GIF saved successfully.");
    msgBox.exec();
  } else {
    QMessageBox::warning(this, "", "Failed to save gif.");
  }
}

void MainWindow::screenshotButton() {
  QString temp = QCoreApplication::applicationDirPath();
  temp.chop(9);
  QString timeStr =
      QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.jpg");
  QString initName = temp + "data-samples/" + "3DV_ScreenShot_" + timeStr;
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save screenshot"), initName, tr("Image Files (*.jpg *.bmp)"), 0,
      QFileDialog::DontUseNativeDialog);

  if (!fileName.isEmpty()) {
    QImage img = ui->openGLWidget->grabFramebuffer();
    if (img.save(fileName)) {
      QMessageBox msgBox;
      msgBox.setText("Screenshot saved successfully.");
      msgBox.exec();
    } else {
      QMessageBox::warning(this, "", "Failed to save screenshot.");
    }
  }
}

void MainWindow::backColorChange() {
  auto glW = ui->openGLWidget;
  controller_->set_bg_color(glW);
  setColorButton(ui->bgClrButton, controller_->get_bg_colour(glW));
}

void MainWindow::vertexColorChange() {
  auto glW = ui->openGLWidget;
  controller_->set_vx_color(glW);
  setColorButton(ui->vrtxColorButton, controller_->get_vx_colour(glW));
}

void MainWindow::edgeColorChange() {
  auto glW = ui->openGLWidget;
  controller_->set_eg_color(glW);
  setColorButton(ui->edgesColorButton, controller_->get_eg_colour(glW));
}
