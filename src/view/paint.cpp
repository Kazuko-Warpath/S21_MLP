#include "paint.h"

Paint::Paint(QWidget *parent) : QWidget(parent) {
  image_ = QImage(QSize(400, 400), QImage::Format_RGB16);
  ClearImage();
}

void Paint::ClearImage() {
  image_.fill(qRgb(255, 255, 255));
  update();
}

void Paint::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, image_, dirtyRect);
}

void Paint::mousePressEvent(QMouseEvent *event) {
  mBtn_ = event->button();
  if (mBtn_ == Qt::LeftButton) {
    pick_point_ = event->pos();
  } else {
    ClearImage();
    update();
  }
}

void Paint::mouseMoveEvent(QMouseEvent *event) {
  if (mBtn_ == Qt::LeftButton) {
    draw(event->pos());
  }
}

void Paint::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    Prediction();
  }
}
void Paint::Prediction() {
  ImageToVector();
  SaveVector();
  emit signalSaveImage();
}
void Paint::draw(const QPoint &endPos) {
  QPainter painter(&image_);
  painter.setPen(
      QPen(Qt::black, 30, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawLine(pick_point_, endPos);
  update();
  pick_point_ = endPos;
}

void Paint::ImageToVector() {
  pixel_vector_.clear();
  for (int i = 0; i < 28; i++) {
    for (int j = 0; j < 28; j++) {
      QRgb color = image_.pixel(i * 400 / 28, j * 400 / 28);
      pixel_vector_.push_back(255 - (color & 0x000000ff));
    }
  }
}

void Paint::SaveVector() {
  std::fstream greta;
  QString name_image = QDir::homePath() + "/weights/image.csv";
  greta.open(name_image.toLocal8Bit(), std::ios::out);
  if (greta.is_open()) {
    greta << "0,";
    for (size_t i = 0; i < pixel_vector_.size(); i++) {
      greta << pixel_vector_[i] << ",";
    }
    greta.close();
  }
}

Paint::~Paint() {}
