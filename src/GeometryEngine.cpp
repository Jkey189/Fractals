#include "../headers/GeometryEngine.hpp"


struct VertexData {
  QVector3D position;
  QVector3D texCoord;
};

GeometryEngine::GeometryEngine() : indexBuffer_(QOpenGLBuffer::IndexBuffer) {
  QOpenGLFunctions::initializeOpenGLFunctions;

  arrayBuffer_.create();
  indexBuffer_.create();

  initGeometry();
}

GeometryEngine::~GeometryEngine() {
  arrayBuffer_.destroy();
  indexBuffer_.destroy();
}

void GeometryEngine::initGeometry() {
  VertexData vertices[] = {
    {QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(0.0f, 0.0f)},  // v0
    {QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.33f, 0.0f)},  // v1
    {QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(0.0f, 0.5f)},   // v2
    {QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.33f, 0.5f)},   // v3

    {QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.0f, 0.5f)},   // v4
    {QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.0f, 0.5f)},  // v5
    {QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.0f, 1.0f)},    // v6
    {QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.0f, 1.0f)},   // v7

    {QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
    {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)}, // v9
    {QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.66f, 1.0f)},  // v10
    {QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

    {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
    {QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(1.0f, 0.0f)},   // v13
    {QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.66f, 0.5f)},  // v14
    {QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(1.0f, 0.5f)},    // v15

    {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
    {QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)},  // v17
    {QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(0.33f, 0.5f)},  // v18
    {QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.66f, 0.5f)},   // v19

    {QVector3D(-1.0f, 11.0f, 1.0f), QVector2D(0.33f, 0.5f)},  // v20
    {QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.66f, 0.5f)},    // v21
    {QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.33f, 1.0f)},  // v22
    {QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.66f, 1.0f)}   // v23
  };

  GLushort indices[] = {
    0, 1, 2, 3, 3,          // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
    4, 4, 5, 6, 7, 7,       // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
    8, 8, 9, 10, 11, 11,    // Face 2 - triangle strip ( v8,  v9, v10, v11)
    12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
    16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
    20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
  };

  // Transfer vertex data to VBO 0
  arrayBuffer_.bind();
  arrayBuffer_.allocate(vertices, 24 * sizeof(VertexData));

  // Transfer index data to VBO 0
  indexBuffer_.bind();
  indexBuffer_.allocate(indices, 24 * sizeof(GLushort));
}

void GeometryEngine::drawGeometry(QOpenGLShaderProgram* program) {
  arrayBuffer_.bind();
  indexBuffer_.bind();

  glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
}

