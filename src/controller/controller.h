#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_
#include <QDebug>
#include <map>

#include "../model/graph_perceptron.h"
#include "../model/matrix_perceptron.h"

namespace s21 {
class Controller {
 public:
  Controller(MatrixPerceptron *matrix_model, GraphPerceptron *graph_model)
      : m_net(matrix_model),
        g_net(graph_model)

  {}
  void SetPerceptronType(bool perceptron_type);
  void SetCountHiddenLayers(int hidden_layers);
  int GetCountHiddenLayers() { return hidden_layers_; }
  void SetWeights(std::string filename_weights);
  void SaveWeights(std::string save_new_weights);
  void SetNameTrain(char *filename_weights);
  void SetNameTest(std::string const &filename_weights);
  void SetStopTrainOrTest(bool var);
  void Change(int hidden, int type);
  void Validation(int k, std::string filename);
  void Train(int epoch, std::string filename_train,
             std::vector<double> *report_graph);
  void Testing(int test_sample);
  int getProgressProcent();
  std::vector<double> GetMetrics();
  std::vector<int> Predict(std::string name_image);

 private:
  MatrixPerceptron *m_net;
  GraphPerceptron *g_net;
  bool perceptron_type_;
  int hidden_layers_;
  std::string filename_weights_;
  char *filename_train_;
  std::string filename_test_;
  std::vector<double> metrics{4};
};
}  // namespace s21

#endif  // SRC_CONTROLLER_H_
