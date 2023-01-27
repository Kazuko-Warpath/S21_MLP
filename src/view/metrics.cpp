#include "metrics.h"

#include "ui_metrics.h"

Metrics::Metrics(std::vector<double> metrics, QWidget *parent)
    : QWidget(parent), ui(new Ui::Metrics) {
  ui->setupUi(this);
  if (metrics.size() == 4) {
    ui->accuracy_2->setText(QString::number(metrics[0], 'g', 2));
    ui->precision_2->setText(QString::number(metrics[1], 'g', 2));
    ui->recall_2->setText(QString::number(metrics[2], 'g', 2));
    ui->f_measure_2->setText(QString::number(metrics[3], 'g', 2));
  }
}

Metrics::~Metrics() { delete ui; }
