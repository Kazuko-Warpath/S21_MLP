#include "train.h"

#include <QThread>

#include "ui_train.h"

Train::Train(s21::Controller *controller, QDialog *parent)
    : QDialog(parent),
      ui(new Ui::Train),
      controller_(controller),
      timer(new QTimer(this)),
      is_file_choose{false},
      abort_train{true} {
  ui->setupUi(this);
  ui->file_name->setText("No file choosen");
  ui->progressBar->setValue(0);
}

Train::~Train() {
  delete ui;
  delete timer;
}

void Train::closeEvent(QCloseEvent *event) {
  timer->stop();
  controller_->SetStopTrainOrTest(true);
  event->accept();
}

void Train::setStyleAbortTrue() {
  ui->train->setStyleSheet(
      "QPushButton { border: 1px solid gray; background-color: rgb(189, 193, "
      "195); color: black;border-radius: 10px;} QPushButton:pressed "
      "{background-color: rgb(228, 226, 229);}");
  ui->save->setStyleSheet(
      "QPushButton {border: 1px solid gray; background-color: rgb(148, 179, "
      "193); color: black; border-radius: 10px;} QPushButton:pressed "
      "{background-color: rgb(166, 204, 221);}");
}

void Train::setStyleAbortFalse() {
  ui->train->setStyleSheet(
      "QPushButton { border: 1px solid gray; background-color: rgb(189, 193, "
      "195); color: black;border-radius: 10px;}");
  ui->save->setStyleSheet(
      "QPushButton {border: 1px solid gray; background-color: rgb(148, 179, "
      "193); color: black; border-radius: 10px;}");
}

void Train::on_cancel_clicked() { close(); }

void Train::on_train_clicked() {
  if (is_file_choose == true && abort_train == true) {
    abort_train = false;
    setStyleAbortFalse();
    this->resize(800, 550);
    percent_progress_ = 0;
    ui->progressBar->setValue(percent_progress_);
    std::thread t([this]() { ProgressTrain(); });
    t.detach();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(ProgressPercent()));
  }
}

void Train::ProgressTrain() {
  controller_->SetStopTrainOrTest(false);
  report_graph.clear();
  controller_->Train(ui->epoch->value(), FileName.toStdString(), &report_graph);
  ui->error_procent->setText(
      QString::number(((int)report_graph[report_graph.size() - 1])) + "%");
}

void Train::ProgressPercent() {
  percent_progress_ = controller_->getProgressProcent();
  ui->progressBar->setValue(percent_progress_);
  if (percent_progress_ == 100) {
    timer->stop();
    abort_train = true;
  }
}

void Train::on_load_file_clicked() {
  FileName = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath(),
                                          "csv (*.csv)");
  if (FileName.size() != 0) {
    printFileName(FileName);
    is_file_choose = true;
  }
}

void Train::printFileName(QString FileName) {
  QStringList parts = FileName.split("/");
  QString lastBit = parts.at(parts.size() - 1);
  ui->file_name->setText(lastBit);
}

void Train::on_aborting_clicked() {
  abort_train = true;
  setStyleAbortTrue();
  controller_->SetStopTrainOrTest(true);
  timer->stop();
  percent_progress_ = 0;
  ui->progressBar->setValue(percent_progress_);
}

void Train::on_save_clicked() {
  if (abort_train == true) {
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

void Train::on_graph_report_clicked() {
  if (abort_train == true) {
    Report *rep = new Report(report_graph);
    rep->show();
  }
}
