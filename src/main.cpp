#include <QApplication>
#include <QFontDatabase>

#include "controller/controller.h"
#include "view/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QFontDatabase::addApplicationFont(":/helvetica_regular.otf");
  QFontDatabase::addApplicationFont(":/chalkboardse.otf");
  s21::MatrixPerceptron matrix_model;
  s21::GraphPerceptron graph_model;

  s21::Controller controller(&matrix_model, &graph_model);
  MainWindow w(&controller);

  w.resize(800, 400);
  w.show();

  return a.exec();
}
