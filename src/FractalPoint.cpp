#include "../headers/FractalPoint.hpp"


FractalPoint::FractalPoint(qreal newX, qreal newY, QColor newColor) {
  x_ = newX;
  y_ = newY;
  color_ = newColor;
}

void FractalPoint::setColor(QColor newColor) {
  color_ = newColor;
}

void FractalPoint::setX(qreal newX) {
  x_ = newX;
}

void FractalPoint::setY(qreal newY) {
  y_ = newY;
}
