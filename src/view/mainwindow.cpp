#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(s21::Controller *controller, QWidget *parent)
    : QMainWindow(parent),
      controller_(controller),
      ui(new Ui::MainWindow),
      tr_(nullptr)
{

  ui->setupUi(this);
  SetParametrs();
  connect(ui->Painter, &Paint::signalSaveImage, this, &MainWindow::slotPredict);

}

MainWindow::~MainWindow() {
  delete ui;
  delete test_window_;
}

void MainWindow::SetParametrs() {
  hidden_count_ = 2;
  controller_->SetCountHiddenLayers(hidden_count_);
  controller_->SetPerceptronType(1);
  SetWeight(2);
}

void MainWindow::SetWeight(int hidden_layers) {
  char name = hidden_layers + '0';
  QString weight = QDir::homePath() + "/weights/" + name + "_layers.weights";
  controller_->SetWeights(weight.toStdString());
  printInfoAboutWeights(weight);
}

void MainWindow::slotPredict() {
  answer_predict.resize(4);
  QString name_image = QDir::homePath() + "/weights/image.csv";
  answer_predict = controller_->Predict(name_image.toStdString());
  ui->main_letter->setText((QString)((char)(answer_predict[0] + 64)));
  ui->letter_1->setText((QString)((char)(answer_predict[1] + 64)));
  ui->letter_2->setText((QString)((char)(answer_predict[2] + 64)));
  ui->letter_3->setText((QString)((char)(answer_predict[3] + 64)));
}

void MainWindow::on_setup_clicked() {
  if (size == true) {
    this->resize(800, 400);
    size = false;
  } else {
    this->resize(800, 700);
    size = true;
  }
}

void MainWindow::on_validation_clicked() {
  va_ = new Validation(controller_);
  va_->resize(800, 300);
  va_->exec();
}

void MainWindow::on_train_clicked() {
  tr_ = new Train(controller_);
  tr_->resize(800, 300);
  tr_->exec();
}

void MainWindow::on_load_image_clicked() {
  QString FileName = QFileDialog::getOpenFileName(
      this, "Open a file", QDir::homePath(), "bmp (*.bmp)");
  QImage tmp_img(FileName, "BMP");
  ui->Painter->LoadImage() =
      tmp_img.scaled(QSize(400, 400)).convertToFormat(QImage::Format_RGB16);
  ui->Painter->signalSaveImage();
  ui->Painter->Prediction();
  ui->Painter->update();

      tmp_img  = QImage(QSize(400, 400), QImage::Format_RGB16);
 ui->Painter->LoadImage() =   tmp_img;

}

void MainWindow::on_load_test_clicked() {
  test_window_ = new Test(controller_);
  test_window_->resize(800, 300);
  test_window_->exec();

}

void MainWindow::on_hidden_layer_currentIndexChanged(int index) {
  hidden_count_ = index + 2;
  SetWeight(hidden_count_);
  controller_->Change(hidden_count_, perceptron_type_);
}

void MainWindow::on_graph_clicked() {
  perceptron_type_ = 0;
  controller_->Change(hidden_count_, perceptron_type_);
}

void MainWindow::on_matrix_clicked() {
  perceptron_type_ = 1;
  controller_->Change(hidden_count_, perceptron_type_);
}

void MainWindow::on_load_weight_clicked() {
  QString FileName = QFileDialog::getOpenFileName(
      this, "Open a file", QDir::homePath(), "Weights (*.weights)");
  if (FileName.size() != 0) {
    QByteArray f = FileName.toLocal8Bit();
    char *f_str = f.data();
    controller_->SetWeights(f_str);
    printInfoAboutWeights(FileName);
  }
}

void MainWindow::printInfoAboutWeights(QString FileName) {
  QStringList parts = FileName.split("/");
  QString lastBit = parts.at(parts.size() - 1);
  ui->name_weights->setText(lastBit);
}
