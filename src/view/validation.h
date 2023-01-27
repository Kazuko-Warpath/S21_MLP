#ifndef VALIDATION_H
#define VALIDATION_H

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

namespace Ui {
class Validation;
}

class Validation : public QDialog {
  Q_OBJECT

 public:
  explicit Validation(s21::Controller *controller, QDialog *parent = nullptr);
  ~Validation();

 protected:
  void closeEvent(QCloseEvent *event);

 private slots:
  void on_cancel_clicked();
  void on_train_clicked();
  void on_load_file_clicked();
  void on_abort_clicked();
  void on_save_clicked();
  void setStyleAbortTrue();
  void setStyleAbortFalse();
  void ProgressValidation();
  void ProgressValPercent();
  void printFileName(QString FileName);

 private:
  Ui::Validation *ui;
  s21::Controller *controller_;
  bool is_file_choose;
  bool abort_validation;
  QTimer *timer;
  int percent_progress_{};
  int k_group_{};
  QString FileName{};
};

#endif  // VALIDATION_H
