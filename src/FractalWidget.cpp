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

void FractalWidget::initShaders() {
  if (!program_.addShaderFromSourceFile(QOpenGLShader::Vertex, "fileName")) {
    close();
  }
  if (!program_.addShaderFromSourceFile(QOpenGLShader::Fragment, "fileName")) {
    close();
  }
  if (!program_.link()) {
    close();
  }
  if (!program_.bind()) {
    close();
  }
}

void FractalWidget::resizeGL(int w, int h) {
  qreal aspect = static_cast<qreal>(w) / static_cast<qreal>(h ? h : 1);

  const qreal zNear = 3.0;
  const qreal zFar = 7.0;
  const qreal fov = 45.0;

  projection_.setToIdentity();
  projection_.perspective(fov, aspect, zNear, zFar);
}

void FractalWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 matrix;
  matrix.translate(0.0, 0.0, fractalData_->zoomCoefficient);
  matrix.lookAt(fractalData_->camera, -fractalData_->camera, pointAxisY_ - fractalData_->camera);

  program_.setUniformValue("mvp_matrix", projection_ * matrix);

  program_.setUniformValue("POWER", (GLint)fractalData_->n);
  program_.setUniformValue("Resolution", dynamic_cast<QApplication*>(QCoreApplication::instance())->devicePixelRatio() * QVector2D(this->width(), this->height));
  program_.setUniformValue("CriticalPointX", (GLfloat)fractalData_->x);
  program_.setUniformValue("CriticalPointY", (GLfloat)fractalData_->y);
  program_.setUniformValue("CriticalPointZ", (GLfloat)fractalData_->z);
  program_.setUniformValue("TYPE", (GLint)fractalData_->type);
  program_.setUniformValue("Ambience", transformColor(fractalData_->ambienceColor));
  program_.setUniformValue("FractalColor", transformColor(fractalData_->fractalColor)); // "ColorFractal"
  program_.setUniformValue("CameraPosition", fractalData_->camera);
  program_.setUniformValue("ZoomCoefficient", (GLfloat)fractalData_->zoomCoefficient);

  // draw
  geometries_->drawGeometry(&program_);
}

void FractalWidget::setFractalData(FractalData* data) {
  fractalData_ = data;
}

void FractalWidget::autoRotate() {
  if (fractalData_->isRotating) {
    auto nextPos = static_cast<qreal>(elapsedTimer_->elapsed());
    qreal dx = (nextPos - autoRotatePos_) * fractalData_->absoluteSpeed;
    autoRotatePos_ = nextPos;

    rotateFractal({static_cast<float>(dx), 0.0});
  }
}
