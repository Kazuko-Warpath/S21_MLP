#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>

#include "../controller/controller.h"
#include "../model/graph_perceptron.h"
#include "../model/matrix_perceptron.h"
#include "test.h"
#include "train.h"
#include "validation.h"
#include "view/paint.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Controller *controller, QWidget *parent = nullptr);
  ~MainWindow();

 public slots:
  void slotPredict();
  void closeTrain() {
    delete tr_;
    tr_ = nullptr;
  }

 private slots:
  void on_setup_clicked();
  void on_validation_clicked();
  void on_train_clicked();
  void on_load_image_clicked();
  void on_load_test_clicked();
  void printInfoAboutWeights(QString FileName);
  void on_hidden_layer_currentIndexChanged(int index);
  void on_graph_clicked();
  void on_matrix_clicked();
  void on_load_weight_clicked();
  void SetParametrs();
  void SetWeight(int hidden_layers);

 private:
  bool size{};
  Paint pa;
  std::vector<int> answer_predict;
  int perceptron_type_{};
  int hidden_count_{};
  s21::Controller *controller_;
  Ui::MainWindow *ui;
  Train *tr_{nullptr};
  Test *test_window_{nullptr};
  Validation *va_{nullptr};

};
#endif  // MAINWINDOW_H
