#ifndef TRAIN_H
#define TRAIN_H

#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QWidget>
#include <chrono>
#include <mutex>
#include <thread>

#include "../controller/controller.h"
#include "report.h"

namespace Ui {
class Train;
}

class Train : public QDialog {
  Q_OBJECT

 public:
  explicit Train(s21::Controller *controller, QDialog *parent = nullptr);
  ~Train();

 private slots:
  void on_train_clicked();
  void on_cancel_clicked();
  void on_load_file_clicked();
  void on_aborting_clicked();
  void on_save_clicked();
  void setStyleAbortTrue();
  void setStyleAbortFalse();
  void printFileName(QString FileName);
  void ProgressTrain();
  void ProgressPercent();

  void on_graph_report_clicked();

 signals:
  void trainExit();

 protected:
  void closeEvent(QCloseEvent *event);

 private:
  Ui::Train *ui;
  s21::Controller *controller_;
  int epoch_;
  QTimer *timer;
  char *filename_train_;
  std::thread *th_;
  std::atomic<int> percent_progress_{};
  bool exit_thread_{true};
  QString FileName;
  bool is_file_choose;
  bool abort_train;
  std::vector<double> report_graph{};

  double accuracy{};
  double f_measure{};
  double precision{};
  double recall{};
};

#endif  // TRAIN_H
