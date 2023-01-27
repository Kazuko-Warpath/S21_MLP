#include "test.h"

#include "ui_test.h"

Test::Test(s21::Controller *controller, QDialog *parent)
    : QDialog(parent),
      ui(new Ui::Test),
      controller_(controller),
      timer(new QTimer(this)),
      is_file_choose{false},
      abort_test{true} {
  ui->setupUi(this);
  ui->file_name->setText("No file choosen");
}

Test::~Test() {
  delete timer;
  delete ui;
}

void Test::closeEvent(QCloseEvent *event) {
  timer->stop();
  controller_->SetStopTrainOrTest(true);
  event->accept();
}

void Test::on_cancel_clicked() { close(); }
void Test::on_ok_clicked() {
  if (abort_test == true) close();
}

void Test::setStyleAbortTrue() {
  ui->test->setStyleSheet(
      "QPushButton { border: 1px solid gray; background-color: rgb(189, 193, "
      "195); color: black;border-radius: 10px;} QPushButton:pressed "
      "{background-color: rgb(228, 226, 229);}");
  ui->ok->setStyleSheet(
      "QPushButton {border: 1px solid gray; background-color: rgb(148, 179, "
      "193); color: black; border-radius: 10px;} QPushButton:pressed "
      "{background-color: rgb(166, 204, 221);}");
}

void Test::setStyleAbortFalse() {
  ui->test->setStyleSheet(
      "QPushButton { border: 1px solid gray; background-color: rgb(189, 193, "
      "195); color: black;border-radius: 10px;}");
  ui->ok->setStyleSheet(
      "QPushButton {border: 1px solid gray; background-color: rgb(148, 179, "
      "193); color: black; border-radius: 10px;}");
}

void Test::on_test_clicked() {
  test_sample = ui->test_sample->value();
  if (is_file_choose == true && abort_test == true && test_sample) {
    abort_test = false;
    setStyleAbortFalse();
    this->resize(800, 550);
    percent_progress_ = 0;
    ui->progressBar->setValue(percent_progress_);
    std::thread t([this]() { ProgressTest(); });
    t.detach();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(ProgressPercent()));
  } else if (is_file_choose == true && abort_test == true && !test_sample) {
    this->resize(800, 550);
    percent_progress_ = 100;
    ui->progressBar->setValue(percent_progress_);
  }
}

void Test::ProgressTest() {
  controller_->SetStopTrainOrTest(false);
  controller_->Testing(test_sample);
}

void Test::ProgressPercent() {
  percent_progress_ = controller_->getProgressProcent();
  ui->progressBar->setValue(percent_progress_);
  if (percent_progress_ == 100) {
    timer->stop();
    abort_test = true;
    setStyleAbortTrue();
  }
}

void Test::on_load_file_clicked() {
  QString FileName = QFileDialog::getOpenFileName(
      this, "Open a file", QDir::homePath(), "csv (*.csv)");
  if (FileName.size() != 0) {
    controller_->SetNameTest(FileName.toStdString());
    printFileName(FileName);
    is_file_choose = true;
  }
}

void Test::printFileName(QString FileName) {
  QStringList parts = FileName.split("/");
  QString lastBit = parts.at(parts.size() - 1);
  ui->file_name->setText(lastBit);
}

void Test::on_abort_clicked() {
  abort_test = true;
  setStyleAbortTrue();

  controller_->SetStopTrainOrTest(true);
  timer->stop();
  percent_progress_ = 0;
  ui->progressBar->setValue(percent_progress_);
}

void Test::on_graph_report_clicked() {
  if (abort_test == true) {
    metrics.clear();
    metrics = controller_->GetMetrics();
    Metrics *met = new Metrics(metrics);
    met->show();
  }
}
