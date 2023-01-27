#include "matrix_perceptron.h"

namespace s21 {
//___________Create_And_Init_GraphPerceptron___________//
MatrixPerceptron::MatrixPerceptron() : MatrixPerceptron(2) {}

MatrixPerceptron::MatrixPerceptron(int count_hidden_layers) {
  count_hidden_layers_ = count_hidden_layers;
  count_layers_ = count_hidden_layers + 2;
  number_out_layer_ = count_layers_ - 1;
  InitMatrixPerceptron();
  // GenerateWeightNeuron();
  //   LoadWeights("/Users/tcocoa/CPP7_MLP-0/src/model/weights/2_layers.weights");
}

void MatrixPerceptron::InitMatrixPerceptron() {
  expected_value_.resize(neuron_out);
  value_.resize(count_layers_);
  error_.resize(count_layers_);
  delta_weight_.resize(count_layers_);
  weight_.resize(count_layers_);
  value_[0].resize(neuron_in);
  error_[0].resize(neuron_in);
  weight_[0].resize(0);
  delta_weight_[0].resize(neuron_in);

  for (size_t l = 1; l < number_out_layer_; l++) {
    value_[l].resize(neuron_hidden);
    error_[l].resize(neuron_hidden);
    weight_[l].resize(neuron_hidden);
    delta_weight_[l].resize(neuron_hidden);
    for (int n = 0; n < neuron_hidden; n++) {
      if (l == 1) {
        weight_[l][n].resize(neuron_in);
      } else {
        weight_[l][n].resize(neuron_hidden);
      }
    }
  }
  value_[number_out_layer_].resize(neuron_out);
  error_[number_out_layer_].resize(neuron_out);
  delta_weight_[number_out_layer_].resize(neuron_out);
  weight_[number_out_layer_].resize(neuron_out);
  for (int n = 0; n < neuron_out; n++) {
    weight_[number_out_layer_][n].resize(neuron_hidden);
  }
  expected_value_.resize(neuron_out);
}

void MatrixPerceptron::GenerateWeightNeuron() {
  for (size_t l = 1; l < count_layers_; l++) {
    for (size_t n = 0; n < weight_[l].size(); n++) {
      for (size_t w = 0; w < weight_[l][n].size(); w++) {
        weight_[l][n][w] = 0.001 * (std::rand() % 2001 - 1000);
      }
    }
  }
}

//_____________________Parse_File______________________//
void MatrixPerceptron::LoadValuesTrain(std::string const &filename_train) {
  LoadFile(filename_train);
  vector_vectorovi4 = GetVector();
}

void MatrixPerceptron::LoadValuesTest(std::string const &filename_test) {
  LoadFile(filename_test);
  vector_for_test = GetVector();
}

//________________________Train________________________//
void MatrixPerceptron::EpochTrain(int epoch,
                                  std::vector<double> *report_graph) {
  size_letter_for_epoch = epoch * (int)vector_vectorovi4.size();
  for (int k = 0; k < epoch; k++) {
    successful_find_letter_ = 0;
    for (size_t d = 0;
         d < vector_vectorovi4.size() && this->stop_train_or_test_ == false;
         d++) {
      TrainForthBack(d);
      percent_progress_ = CalculateProgressPercent(d, k);
    }
    report_graph->push_back(100 - (CalculatePercentTrain() * 100));
  }
}

void MatrixPerceptron::TrainForthBack(int d) {
  expected_value_.clear();
  expected_value_.resize(neuron_out);
  expected_value_[vector_vectorovi4[d][0] - 1] = 1;
  letter_to_learn_ = vector_vectorovi4[d][0] - 1;
  for (size_t n = 1; n < vector_vectorovi4[d].size(); n++) {
    value_[0][n - 1] = vector_vectorovi4[d][n] / 255;
  }
  ForwardPropagation();
  CountingSuccessfulLetters();
  BackwardPropagation();
}

//_________________ForwardPropagation__________________//
void MatrixPerceptron::ForwardPropagation() {
  for (size_t l = 1; l < count_layers_ && this->stop_train_or_test_ == false;
       l++) {
    for (size_t n = 0; n < weight_[l].size(); n++) {
      double sum = 0;
      for (size_t w = 0; w < weight_[l][n].size(); w++) {
        sum += value_[l - 1][w] * weight_[l][n][w];
      }
      value_[l][n] = SigmoidFunction(sum);
    }
  }
}

//_________________BackwardPropagation_________________//
void MatrixPerceptron::BackwardPropagation() {
  for (size_t l = number_out_layer_;
       l != 0 && this->stop_train_or_test_ == false; l--) {
    for (size_t n = 0; n < error_[l].size(); n++) {
      double error = 0;
      if (l == number_out_layer_) {
        error = value_[number_out_layer_][n] - expected_value_[n];
      } else {
        for (size_t n_r = 0; n_r < delta_weight_[l + 1].size(); n_r++) {
          error += delta_weight_[l + 1][n_r] * weight_[l + 1][n_r][n];
        }
      }
      error_[l][n] = error;

      double delta_weight = error * SigmoidDerivative(value_[l][n]);

      delta_weight_[l][n] = delta_weight;
      WeightsCalculation(l, n, delta_weight);
    }
  }
}

void MatrixPerceptron::WeightsCalculation(int l, int n, double delta_weight) {
  for (size_t w = 0; w < weight_[l][n].size(); w++) {
    double new_weight =
        weight_[l][n][w] - value_[l - 1][w] * delta_weight * learning_rate;
    weight_[l][n][w] = new_weight;
  }
}

//__________________Save_Load_Weights__________________//
void MatrixPerceptron::SaveWeights(std::string save_new_weights) {
  std::fstream outweights;

  outweights.open(save_new_weights, std::ios::out);

  if (!outweights) {
    std::cerr << "file couldn't be opened ";
    exit(1);
  }

  for (size_t l = 1; l < count_layers_; l++) {
    for (size_t n = 0; n < weight_[l].size(); n++) {
      for (size_t n_w = 0; n_w < weight_[l][n].size(); n_w++) {
        outweights << weight_[l][n][n_w] << ",";
      }
    }
  }
  outweights.close();
}

void MatrixPerceptron::LoadWeights(std::string const &filename_weights) {
  std::ifstream inweights(filename_weights, std::ios::in);
  if (!inweights.bad()) {
    std::string line;

    while (inweights >> line) {
      size_t i = 0;
      char *ptrEnd;
      if (isdigit(line[i]) || line[i] == '-') {
        for (size_t l = 1; l < count_layers_; l++) {
          for (size_t n = 0; n < weight_[l].size(); n++) {
            for (size_t n_w = 0; n_w < weight_[l][n].size(); n_w++) {
              //               static locale_t c_loc = newlocale(LC_ALL_MASK,
              //               "C", nullptr); weight_[l][n][n_w] =
              //               strtod_l(&line[i], &ptrEnd, c_loc);

              // static locale_t c_loc = newlocale(LC_ALL_MASK, "C", nullptr);
              weight_[l][n][n_w] = strtod(&line[i], &ptrEnd);

              do {
                i++;
              } while (line[i] != ',' && i < line.size());
              i++;
            }
          }
        }
      }
    }
  }

  inweights.close();
}

//_______________________Predict_______________________//
std::vector<int> MatrixPerceptron::Predict(std::string name_image) {
  LoadValuesTest(name_image);
  Test(full_sample);
  return FindMaximumPredict();
}

int MatrixPerceptron::FindMaximum() {
  double maximum = 0.0;
  int index_max = 0;
  for (size_t n = 0; n < value_[number_out_layer_].size(); n++) {
    if (value_[number_out_layer_][n] > maximum) {
      maximum = value_[number_out_layer_][n];
      index_max = n;
    }
  }
  return index_max;
}

std::vector<int> MatrixPerceptron::FindMaximumPredict() {
  std::vector<int> index_max(4);
  std::map<double, int> sort;
  for (size_t n = 0; n < value_[number_out_layer_].size(); n++) {
    sort.insert({value_[number_out_layer_][n], n});
  }
  std::map<double, int>::reverse_iterator answer = sort.rbegin();
  for (int i = 0; i < 4 && answer != sort.rend(); i++) {
    index_max[i] = answer->second + 1;
    answer++;
  }
  return index_max;
}

//________________________Test_________________________//
void MatrixPerceptron::Test(int test_sample) {
  percent_progress_ = 0;
  successful_find_letter_ = 0;
  size_letter_for_epoch =
      (int)vector_for_test.size() * (float)test_sample / full_sample;
  for (size_t d = 0;
       d < (size_t)size_letter_for_epoch && this->stop_train_or_test_ == false;
       d++) {
    TestForthBack(d);
    percent_progress_ = CalculateProgressPercent(d, 0);
  }
  CalculatePercent(1);
}

void MatrixPerceptron::TestForthBack(int d) {
  expected_value_.clear();
  expected_value_.resize(26);
  letter_to_learn_ = vector_for_test[d][0] - 1;
  expected_value_[vector_for_test[d][0] - 1] = 1;

  for (size_t n = 1;
       n < vector_for_test[d].size() && this->stop_train_or_test_ == false;
       n++) {
    value_[0][n - 1] = vector_for_test[d][n] / 255;
  }

  ForwardPropagation();
  CountingSuccessfulLetters();
}

double MatrixPerceptron::CalculatePercent(int k_group) {
  return successful_find_letter_ / (double)vector_for_test.size() / k_group;
}

void MatrixPerceptron::CountingSuccessfulLetters() {
  if (FindMaximum() == (int)letter_to_learn_) {
    successful_find_letter_++;
    true_positive += 1;
    true_negative += 25;
  } else {
    false_negative += 1;
    false_positive += 1;
    true_negative += 24;
  }
}

void MatrixPerceptron::Testing(char *filename_test, int test_sample) {
  LoadValuesTest(filename_test);
  Test(test_sample);
}

//__________________ResizePerceptron___________________//

void MatrixPerceptron::Clear() {
  expected_value_.clear();
  for (size_t l = 0; l < count_layers_; l++) {
    for (size_t n = 0; n < weight_[l].size(); n++) {
      weight_[l][n].clear();
    }
    weight_[l].clear();
    value_[l].clear();
    error_[l].clear();
    delta_weight_[l].clear();
  }
  weight_.clear();
  value_.clear();
  error_.clear();
  delta_weight_.clear();
  successful_find_letter_ = 0;
  letter_to_learn_ = 0;
  count_hidden_layers_ = 0;
  count_layers_ = 0;
  number_out_layer_ = 0;

  for (size_t d = 0; d < vector_vectorovi4.size(); d++) {
    vector_vectorovi4[d].clear();
  }
  vector_vectorovi4.clear();

  for (size_t d = 0; d < vector_for_test.size(); d++) {
    vector_for_test[d].clear();
  }
  vector_for_test.clear();
}

void MatrixPerceptron::ResizePerceptron(int count_hidden_layers) {
  Clear();
  count_hidden_layers_ = count_hidden_layers;
  count_layers_ = count_hidden_layers + 2;
  number_out_layer_ = count_layers_ - 1;
  InitMatrixPerceptron();
}



}  // namespace s21
