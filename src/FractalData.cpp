#include "../headers/FractalData.hpp"

#include <QPair>
#include <QMap>
#include <QVector>
#include <QPair>
#include <functional>
#include <map>
#include <utility>
#include <vector>


const QVector3D FractalData::baseCamera = QVector3D(0, 0, 1.5);

namespace {
  qreal randomReal() {
    std::uniform_real_distribution dist(-0.3, 0.3);
    return dist(*QRandomGenerator::global());
  }

  QColor randomColor() {
    return QColor(QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255),
                  QRandomGenerator::global()->bounded(255));
  }

  static std::map<const std::string, const std::pair<std::function<qreal(std::vector<int> const &,
      std::vector<int> const &)>, const qreal>> metrics = {
      {"minkowski-normalized",
       {
        [](std::vector<int> const &u, std::vector<int> const &v) -> qreal {
          static const int p = 5;
          qreal out = 0;

          for (size_t i = 0; i < std::min(u.size(), v.size()); ++i) {
            out += std::pow(std::abs(u[i] - v[i]) / 255, p);
          }

          return std::pow(out, 1.0 / p) / std::pow(3, 1.0 / p);
        }, 0.25
       }
      }
  };

  bool isSimilar(QColor const &u, QColor const &v, std::pair<std::function<qreal(std::vector<int>, std::vector<int>)>,
       qreal> const &metrics) {
    static const auto toVector = [](QColor const &color) -> std::vector<int> {
      return {color.red(), color.green(), color.blue()};
    };

    return metrics.first(toVector(u), toVector(v)) < metrics.second;
  }

  bool isBlack(QColor const &color) {
    return color.value() < 150;
  }
}

[[maybe_unused]] FractalData::FractalData(qreal x, qreal y, qreal z, quint8 n, FractalType type) : x(x), y(y), z(z),
                  n(n), type(type) {}

QJsonObject FractalData::serialize() const {
  QJsonObject serialized;
  serialized.insert("x", x);
  serialized.insert("y", y);
  serialized.insert("z", z);
  serialized.insert("n", n);
  serialized.insert("type", type);
  serialized.insert("Fractal color_", fractalColor.name());
  serialized.insert("Ambience color_", ambienceColor.name());
  serialized.insert("zoomCoefficient", zoomCoefficient);

  QJsonArray cameraPosition;
  cameraPosition.insert(0, camera.x());
  cameraPosition.insert(1, camera.y());
  cameraPosition.insert(2, camera.z());

  serialized.insert("camera", cameraPosition);

  return serialized;
}

void FractalData::readForm(QJsonDocument &in) {
  genRandom();
  QJsonObject fractalData;

  try {
    fractalData = in.object().value("Fractal").toObject();
  } catch(...) {
    return;
  }

  if (fractalData.contains("camera")) {
    // smth
  } else {
    camera = baseCamera;
  }

  QMap<QString, qreal*> qMap = {{"x", &x}, {"y", &y}, {"z", &z}};
  for (auto it = qMap.begin(); it != qMap.end(); ++it) {
    if (fractalData.contains(it.key())) {
      *it.value() = fractalData.value(it.key()).toDouble();
    }
  }

  if (fractalData.contains("n")) {
    n = fractalData.value("n").toInt();
  }
  if (fractalData.contains("type")) {
    type = static_cast<FractalType>(fractalData.value("type").toInt());
  }
  if (fractalData.contains("Fractal color_")) {
    fractalColor = QColor(fractalData.value("Fractal color_").toString());
  }
  if (fractalData.contains("Ambience color_")) {
    ambienceColor = QColor(fractalData.value("Ambience color_").toString());
  }
  if (fractalData.contains("zoomCoefficient")) {
    zoomCoefficient = fractalData.value("zoomCoefficient").toDouble();
  }
}

void FractalData::genRandom() {
  x = randomReal();
  y = randomReal();
  z = randomReal();
  n = 2 * (QRandomGenerator::global()->bounded(4) + 2);
  type = static_cast<FractalType>(QRandomGenerator::global()->bounded(5));
  auto genColors = [this] {
    fractalColor = randomColor();
    ambienceColor = randomColor();
  };

  do {
    genColors();
  } while (isSimilar(fractalColor, ambienceColor, metrics["minkowski-normalized"]) ||
           isBlack(fractalColor) || isBlack(ambienceColor));
}

FractalData::FractalData() {
  genRandom();
}

FractalData::FractalData(qreal x, qreal y, qreal z, quint8 n, FractalType type, const QColor &fractalColor,
                         const QColor &ambienceColor, const QVector3D &camera, qreal zoomCoefficient, bool isRotating) :
                         x(x), y(y), z(z), n(n), fractalColor(fractalColor), ambienceColor(ambienceColor),
                         camera(camera), zoomCoefficient(zoomCoefficient), isRotating(isRotating) {}

void FractalData::setZoomCoefficient(qreal zoomCoefficient) {
  FractalData::zoomCoefficient = zoomCoefficient;
}

void FractalData::setAbsoluteSpeed(qreal absoluteSpeed) {
  FractalData::absoluteSpeed = absoluteSpeed;
}
