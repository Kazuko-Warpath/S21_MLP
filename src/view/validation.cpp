#include "validation.h"

#include <QDebug>

#include "ui_validation.h"

Validation::Validation(s21::Controller *controller, QDialog *parent)
    : QDialog(parent),
      ui(new Ui::Validation),
      controller_(controller),
      is_file_choose{false},
      abort_validation{true},
      timer(new QTimer(this)) {
  ui->setupUi(this);
  ui->file_name->setText("No File Choosen");
}

Validation::~Validation() {
  delete timer;
  delete ui;
}

void Validation::closeEvent(QCloseEvent *event) {
  timer->stop();
  controller_->SetStopTrainOrTest(true);
  event->accept();
}

void Validation::on_cancel_clicked() { close(); }

void Validation::on_save_clicked() {
  if (abort_validation == true) {
    char name = controller_->GetCountHiddenLayers() + '0';
    QString FileName = QFileDialog::getSaveFileName(
        this, "Open a file",
        QDir::homePath() + "/" + name + "_new_hidden_layers_", "(*.weights)");
    if (!FileName.isNull() && !FileName.isEmpty()) {
      qDebug() << FileName;
      controller_->SaveWeights(FileName.toStdString());
    }
    close();
  }
}

void Validation::on_train_clicked() {
  if (is_file_choose == true && abort_validation == true) {
    abort_validation = false;
    setStyleAbortFalse();
    this->resize(800, 500);
    percent_progress_ = 0;
    ui->progressBar->setValue(percent_progress_);
    std::thread t([this]() { ProgressValidation(); });
    t.detach();
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(ProgressValPercent()));
  }
}

void Validation::ProgressValidation() {
  k_group_ = 5 * (ui->k_->currentIndex() + 1);
  controller_->SetStopTrainOrTest(false);
  controller_->Validation(k_group_, FileName.toStdString());
}

void Validation::ProgressValPercent() {
  percent_progress_ = controller_->getProgressProcent();
  ui->progressBar->setValue(percent_progress_);
  if (percent_progress_ == 100) {
    timer->stop();
    abort_validation = true;
    setStyleAbortTrue();
  }
}

void Validation::on_load_file_clicked() {
  FileName = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath(),
                                          "csv (*.csv)");
  if (FileName.size() != 0) {
    printFileName(FileName);
    is_file_choose = true;
    setStyleAbortTrue();
  }
}

void Validation::printFileName(QString FileName) {
  QStringList parts = FileName.split("/");
  QString lastBit = parts.at(parts.size() - 1);
  ui->file_name->setText(lastBit);
}

void Validation::on_abort_clicked() {
  abort_validation = true;
  controller_->SetStopTrainOrTest(true);
  timer->stop();
  percent_progress_ = 0;
  ui->progressBar->setValue(percent_progress_);
}

void Validation::setStyleAbortTrue() {
  ui->train->setStyleSheet(
      "QPushButton { border: 1px solid gray; background-color: rgb(189, 193, "
      "195); color: black;border-radius: 10px;} QPushButton:pressed "
      "{background-color: rgb(228, 226, 229);}");
  ui->save->setStyleSheet(
      "QPushButton {border: 1px solid gray; background-color: rgb(148, 179, "
      "193); color: black; border-radius: 10px;} QPushButton:pressed "
      "{background-color: rgb(166, 204, 221);}");
}

void Validation::setStyleAbortFalse() {
  ui->train->setStyleSheet(
      "QPushButton { border: 1px solid gray; background-color: rgb(189, 193, "
      "195); color: black;border-radius: 10px;}");
  ui->save->setStyleSheet(
      "QPushButton {border: 1px solid gray; background-color: rgb(148, 179, "
      "193); color: black; border-radius: 10px;}");
}
