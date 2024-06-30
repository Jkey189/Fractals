#ifndef FRACTALWIDGET_HPP
#define FRACTALWIDGET_HPP


#include "FractalData.hpp"
#include "GeometryEngine.hpp"

#include <QApplication>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QTimer>
#include <QVector2D>
#include <cmath>
#include <utility>


class FractalWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  using QOpenGLWidget::QOpenGLWidget;

  ~FractalWidget();

  void setFractalData(FractalData* data);


protected:
  void wheelEvent(QWheelEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void initShaders();

  void rotateFractal(const QVector2D &difference);
  void autoRotate();


private:
  bool mousePressed_ = false;
  QOpenGLShaderProgram program_;
  FractalData *fractalData_;
  GeometryEngine *geometries_ = nullptr;
  QElapsedTimer *elapsedTimer_ = nullptr;
  QTimer *timer_ = nullptr;

  QMatrix4x4 projection_;
  QVector2D mousePressPosition_;
  QVector3D pointAxisX_ = QVector3D(1.0, 0.0, 1.5);
  QVector3D pointAxisY_ = QVector3D(0.0, 1.0, 1.5);
  qreal autoRotatePos_ = 0.0;
};


#endif //FRACTALWIDGET_HPP
