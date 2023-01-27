#ifndef REPORT_H
#define REPORT_H

#include <QWidget>
#include <QtPrintSupport/QPrinter>

#include "qcustomplot.h"

namespace Ui {
class Report;
}

class Report : public QWidget {
  Q_OBJECT

 public:
  explicit Report(std::vector<double> report_graph, QWidget *parent = nullptr);
  ~Report();

 private:
  Ui::Report *ui;
};

#endif  // REPORT_H
