#ifndef FRACTALS_COMPLEX3D_HPP
#define FRACTALS_COMPLEX3D_HPP


#include <QPoint>


class Complex3D {
public:
  Complex3D();
  Complex3D(qreal newX, qreal newY, qreal newZ);

  ~Complex3D() = default;

  [[nodiscard]] qreal theta() const;
  [[nodiscard]] qreal phi() const;
  [[nodiscard]] qreal abs() const;

  friend void operator^(Complex3D &z, const int &n);
  friend Complex3D operator+(const Complex3D& a, const Complex3D& b);


private:
  qreal x_ = 0;
  qreal y_ = 0;
  qreal z_ = 0;
};


#endif //FRACTALS_COMPLEX3D_HPP
