#include "report.h"

#include "ui_report.h"

Report::Report(std::vector<double> report_graph, QWidget *parent)
    : QWidget(parent), ui(new Ui::Report) {
  ui->setupUi(this);

  ui->widget->xAxis->setRange(0, report_graph.size() + 1);
  ui->widget->xAxis->ticker()->setTickCount(report_graph.size() + 1);
  ui->widget->yAxis->setRange(0, 100);
  ui->widget->xAxis->setLabel("epoch");
  ui->widget->yAxis->setLabel("error");
  QCPAxis *keyAxis = ui->widget->xAxis;
  QCPAxis *valueAxis = ui->widget->yAxis;
  QCPBars *fossil = new QCPBars(keyAxis, valueAxis);
  QVector<double> ticks = {};
  for (int i = 1; i <= (int)report_graph.size(); i++) {
    ticks.push_back(i);
  }
  //  valueAxis->setPadding(35);
  QVector<double> fossilData =
      QVector<double>(report_graph.begin(), report_graph.end());
  fossil->setData(ticks, fossilData);
}

Report::~Report() { delete ui; }
