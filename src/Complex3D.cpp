#include "../headers/Complex3D.hpp"
#include <cmath>

Complex3D::Complex3D(qreal newX, qreal newY, qreal newZ) {
  x_ = newX;
  y_ = newY;
  z_ = newZ;
}

qreal Complex3D::abs() const {
  return std::sqrt{x_ * x_ + y_ * y_ + z_ * z_};
}

qreal Complex3D::theta() const {
  return atan2(sqrt(x_ * x_ + y_ * y_), z_);
}

qreal Complex3D::phi() const {
  return atan2(y_, x_);
}

void operator^(Complex3D &z, const int &n) {
  int helper = 1;
  const int m = n - n % 2;
  qreal rN = z.abs();

  while (helper < m) {
    rN *= rN;
    helper *= 2;
  }

  if (n % 2) { // 1
    rN *= z.abs();
  }

  qreal thetaN = z.theta() * n;
  qreal phiN = z.phi() * n;

  z.x_ = rN * sin(thetaN) * cos(phiN);
  z.y_ = rN * sin(thetaN) * sin(phiN);
  z.z_ = rN * cos(thetaN);
}

Complex3D operator+(const Complex3D &a, const Complex3D &b) {
  return {a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_};
}
