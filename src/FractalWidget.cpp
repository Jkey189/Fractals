#include "../headers/FractalWidget.hpp"


namespace {
  QVector3D transformColor(const QColor &color) {
    return QVector3D(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
  }
}

FractalWidget::~FractalWidget() {
  makeCurrent();

  delete geometries_;

  doneCurrent();
}

void FractalWidget::wheelEvent(QWheelEvent *event) {
  static const qreal degreesCoefficient = 1.0 / 360;
  static const qreal EPS = 0.001;

  QPoint numDegrees = event->angleDelta();

  static const qreal minZoom = -5.0;
  static const qreal maxZoom = 9.0;

  qreal delta = numDegrees.y() * degreesCoefficient;
  qreal newValue = fractalData_->zoomCoefficient + delta;
  qreal newSpeed = fractalData_->rotateSpeed;

  if (newValue > FractalData::defaultZoom) {
    newSpeed /= (1 + (newValue - FractalData::defaultZoom) * 0.1);
  }

  if (EPS < abs(delta) && minZoom <= newValue && newValue <= maxZoom) {
    fractalData_->zoomCoefficient = newValue;
    fractalData_->rotateSpeed = newSpeed;

    update();
  }
}

void FractalWidget::mousePressEvent(QMouseEvent *event) {
  mousePressPosition_ = QVector2D(event->pos()); // ->position();
}

void FractalWidget::mouseReleaseEvent(QMouseEvent *event) {
  mousePressed_ = false;
}

namespace {
  QVector3D rotate(QVector3D point, qreal alpha, QVector3D axis) {
    qreal t11 = cos(alpha) + (1 - cos(alpha)) * axis.x() * axis.x();
    qreal t12 = (1 - cos(alpha)) * axis.x() * axis.y() - sin(alpha) * axis.z();
    qreal t13 = (1 - cos(alpha)) * axis.x() * axis.z() + sin(alpha) * axis.y();
    qreal t21 = (1 - cos(alpha)) * axis.x() * axis.y() + sin(alpha) * axis.z();
    qreal t22 = cos(alpha) + (1 - cos(alpha)) * axis.y() * axis.y();
    qreal t23 = (1 - cos(alpha)) * axis.y() * axis.z() - sin(alpha) * axis.x();
    qreal t31 = (1 - cos(alpha)) * axis.x() * axis.z() - sin(alpha) * axis.y();
    qreal t32 = (1 - cos(alpha)) * axis.y() * axis.z() + sin(alpha) * axis.x();
    qreal t33 = cos(alpha) + (1 - cos(alpha)) * axis.z() * axis.z();

    return QVector3D(point.x() * t11 + point.y() * t21 + point.z() * t31,
                     point.x() * t12 + point.y() * t22 + point.z() * t32,
                     point.x() * t13 + point.y() * t23 + point.z() * t33);
  }
}

void FractalWidget::rotateFractal(const QVector2D &difference) {
  if (difference.x() == 0 && difference.y() == 0) {
    return;
  }

  QVector2D alpha = difference * (M_PI / 720.0);

  QVector3D vecAxisY = (pointAxisY_ - fractalData_->camera).normalized();

  pointAxisX_ = rotate(pointAxisX_, alpha.x(), vecAxisY);
  pointAxisY_ = rotate(pointAxisY_, alpha.x(), vecAxisY);
  fractalData_->camera = rotate(fractalData_->camera, alpha.x(), vecAxisY);

  QVector3D vecAxisX = (pointAxisX_ - fractalData_->camera).normalized();

  pointAxisX_ = rotate(pointAxisX_, alpha.y(), vecAxisX);
  pointAxisY_ = rotate(pointAxisY_, alpha.y(), vecAxisX);
  fractalData_->camera = rotate(fractalData_->camera, alpha.y(), vecAxisX);

  update();
}

void FractalWidget::mouseMoveEvent(QMouseEvent *event) {
  if (!mousePressed_) {
    return;
  }

  rotateFractal(QVector2D(event->pos()) - mousePressPosition_);
}

void FractalWidget::initializeGL() {
  this->setMouseTracking(true);

  initializeOpenGLFunctions();
  initShaders();

  glClearColor(0, 0, 0, 1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  geometries_ = new GeometryEngine;
  timer_ = new QTimer;
  elapsedTimer_ = new QElapsedTimer;

  connect(timer_, &QTimer::timeout, [&](){ autoRotate(); });

  elapsedTimer_->start();
  timer_->start;
}
