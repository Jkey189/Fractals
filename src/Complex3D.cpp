#include "../headers/Complex3D.hpp"
#include <cmath>

Complex3D::Complex3D(qreal newX, qreal newY, qreal newZ) {
  x_ = newX;
  y_ = newY;
  z_ = newZ;
}

Complex3D operator+(const Complex3D& a, const Complex3D& b) {
  return {a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_};
}

qreal Complex3D::abs() const {
  return std::sqrt{x_ * x_ + y_ * y_ + z_ * z_};
}

qreal Complex3D::theta() const {
  return atan2(sqrt(x_ * x_ + y_ * y_), z_);
}

