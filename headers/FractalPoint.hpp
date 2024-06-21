#ifndef FRACTALS_FRACTALPOINT_HPP
#define FRACTALS_FRACTALPOINT_HPP


#include <QColor>
#include <QPoint>


class FractalPoint {
public:
  FractalPoint(qreal newX, qreal newY, QColor newColor);

  ~FractalPoint() = default;

  [[nodiscard]] QColor getColor() { return color_; }
  [[nodiscard]] qreal getX() { return x_; }
  [[nodiscard]] qreal getY() { return y_; }

  void setColor(QColor newColor);
  void setX(qreal newX);
  void setY(qreal newY);


private:
  qreal x_, y_;
  QColor color_;
};


#endif //FRACTALS_FRACTALPOINT_HPP
