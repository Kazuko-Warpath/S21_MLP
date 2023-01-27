#ifndef METRICS_H
#define METRICS_H

#include <QWidget>

namespace Ui {
class Metrics;
}

class Metrics : public QWidget {
  Q_OBJECT

 public:
  explicit Metrics(std::vector<double> metrics, QWidget *parent = nullptr);
  ~Metrics();

 private:
  Ui::Metrics *ui;
};

#endif  // METRICS_H
