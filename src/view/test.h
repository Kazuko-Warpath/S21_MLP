#ifndef TEST_H
#define TEST_H

#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QWidget>
#include <mutex>
#include <thread>

#include "../controller/controller.h"
#include "metrics.h"

namespace Ui {
class Test;
}

class Test : public QDialog {
  Q_OBJECT

 public:
  explicit Test(s21::Controller *controller, QDialog *parent = nullptr);
  ~Test();

 protected:
  void closeEvent(QCloseEvent *event);

 private slots:
  void on_test_clicked();
  void on_cancel_clicked();
  void on_load_file_clicked();
  void on_abort_clicked();
  void on_ok_clicked();
  void setStyleAbortTrue();
  void setStyleAbortFalse();
  void printFileName(QString FileName);
  void ProgressTest();
  void ProgressPercent();

  void on_graph_report_clicked();

 private:
  Ui::Test *ui;
  s21::Controller *controller_;
  QTimer *timer;
  int percent_progress_{};
  bool is_file_choose;
  bool abort_test;
  int test_sample;
  std::vector<double> metrics{4};
};

#endif  // TEST_H
