#ifndef FRACTALS_FRACTALDATA_HPP
#define FRACTALS_FRACTALDATA_HPP


#include <QColor>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QVector3D>

#include <random>


enum FractalType {
  MANDELBROT,
  PSYCHOFRACTAL,
  ANOTHERFRACTAL,
  FLOWERFRACTAL,
  NEWFRACTAL,
  NEWNEWFRACTAL
};

enum ColorType {
  FRACTAL,
  AMBIENCE
};

class FractalData {
public:
  FractalData();

  [[maybe_unused]] FractalData(qreal x, qreal y, qreal z, quint8 n, FractalType type);

  static const QVector3D baseCamera;
  static constexpr const qreal defaultZoom = -3.0;
  static constexpr const qreal defaultSpeed = 0.25;

  qreal x{}, y{}, z{};
  quint8 n = 2;
  FractalType type{};
  QColor fractalColor = QColor(55, 255, 55);
  QColor ambienceColor = QColor(255, 55, 55);
  QVector3D camera = baseCamera;
  qreal zoomCoefficient = defaultZoom;
  qreal rotateSpeed = defaultSpeed;
  qreal absoluteSpeed = defaultSpeed;

  bool isRotating = false;

  FractalData(qreal x, qreal y, qreal z, quint8 n, FractalType type, const QColor &fractalColor,
              const QColor &ambienceColor, const QVector3D &camera, qreal zoomCoefficient, bool isRotating);

  void setZoomCoefficient(qreal zoomCoefficient = defaultZoom);

  void setAbsoluteSpeed(qreal absoluteSpeed = defaultSpeed);

  [[nodiscard]] QJsonObject serialize() const;

  void readForm(QJsonDocument &in);

  void genRandom();
};


#endif //FRACTALS_FRACTALDATA_HPP
