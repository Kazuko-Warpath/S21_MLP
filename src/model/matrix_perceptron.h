#ifndef SRC_MODEL_MATRIX_PERCEPTRON_H_
#define SRC_MODEL_MATRIX_PERCEPTRON_H_

#include <locale.h>

#include <cmath>
#include <vector>

#include "common.h"

namespace s21 {

class MatrixPerceptron : public Common {
 public:
  //___________Create_And_Init_GraphPerceptron___________//
  MatrixPerceptron();
  MatrixPerceptron(int count_hidden_layers);
  void InitMatrixPerceptron();
  void GenerateWeightNeuron();

  //_____________________Parse_File______________________//
  void LoadValuesTrain(std::string const &filename_train);
  void LoadValuesTest(std::string const &filename_test);

  //________________________Train________________________//
  void EpochTrain(int epoch, std::vector<double> *report_graph);
  //  void CrossValidation(const char* filename_train, int k_validation);
  void TrainForthBack(int d) override;

  //_________________ForwardPropagation__________________//
  void ForwardPropagation();

  //_________________BackwardPropagation_________________//
  void BackwardPropagation();
  void WeightsCalculation(int l, int n, double delta_weight);

  //__________________Save_Load_Weights__________________//
  void SaveWeights(std::string save_new_weights);
  void LoadWeights(std::string const &filename_weights);

  //_______________________Predict_______________________//
  std::vector<int> Predict(std::string name_image);
  int FindMaximum();
  std::vector<int> FindMaximumPredict();

  //________________________Test_________________________//
  void Test(int test_sample);
  void TestForthBack(int d) override;
  double CalculatePercent(int k_group) override;
  void CountingSuccessfulLetters();
  void Testing(char *filename_test, int test_sample);

  //__________________ResizePerceptron___________________//
  void Clear();
  void ResizePerceptron(int count_hidden_layers);

 private:
  int count_hidden_layers_{};
  size_t count_layers_{};
  size_t number_out_layer_{};

  std::vector<std::vector<double>> value_;
  std::vector<double> expected_value_;
  std::vector<std::vector<double>> error_;
  std::vector<std::vector<std::vector<double>>> weight_;
  std::vector<std::vector<double>> delta_weight_;
};

}  // namespace s21
#endif  // SRC_MODEL_MATRIX_PERCEPTRON_H_
