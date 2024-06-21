#ifndef GEOMETRYENGINE_HPP
#define GEOMETRYENGINE_HPP


#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>


class GeometryEngine {
public:
  GeometryEngine();

  virtual ~GeometryEngine();

  void drawGeometry(QOpenGLShaderProgram *program);


private:
  void initGeometry();

  QOpenGLBuffer arrayBuffer_;
  QOpenGLBuffer indexBuffer_;
};



#endif //GEOMETRYENGINE_HPP
