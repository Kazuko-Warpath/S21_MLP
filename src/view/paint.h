#ifndef PAINT_H
#define PAINT_H

#include <QDebug>
#include <QDir>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QWidget>
#include <fstream>
#include <iostream>
#include <vector>
// #include "view/mainwindow.h"

class Paint : public QWidget {
  Q_OBJECT

public:
  explicit Paint(QWidget *parent = nullptr);
  void ClearImage();
  void ImageToVector();
  void SaveVector();
  void Prediction();

  QImage &LoadImage() { return image_; }
  ~Paint();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void draw(const QPoint &endPos);

signals:
  void signalSaveImage();

private:
  QImage image_;
  Qt::MouseButton mBtn_;
  QPoint pick_point_;
  std::vector<double> pixel_vector_{};
  //  MainWindow* main_window_ {};
};

#endif // PAINT_H
