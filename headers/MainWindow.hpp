#ifndef FRACTALS_MAINWINDOW_HPP
#define FRACTALS_MAINWINDOW_HPP


#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QProgressBar>
#include <QScrollBar>
#include <QSlider>
#include <QTemporaryDir>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
};
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(QOpenGlWidget)

class MainWindow : public QMainWindow {
  Q_OBJECT
  static const qint64 LIMIT = 180000;

public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow() override;


private:
  Ui::MainWindow *ui;

};


#endif //FRACTALS_MAINWINDOW_HPP
