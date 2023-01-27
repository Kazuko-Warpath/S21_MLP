#include "graph_perceptron.h"

namespace s21 {

//_____________________Create_And_Init_GraphPerceptron_____________________//
GraphPerceptron::GraphPerceptron() : GraphPerceptron(2) {}

GraphPerceptron::GraphPerceptron(int count_hidden_layers)
    : count_hidden_layers_(count_hidden_layers) {
  count_layers_ = count_hidden_layers_ + 2;
  number_out_layer_ = count_layers_ - 1;
  layers_.resize(count_layers_);
  InitGraphPerceptron();
}

void GraphPerceptron::InitGraphPerceptron() {
  layers_[0].SetCountNeuron(neuron_in);

  layers_[1].SetCountNeuron(neuron_hidden);
  layers_[1].SetCountWeightNeuron(neuron_in);

  for (size_t i = 2; i < number_out_layer_; i++) {
    layers_[i].SetCountNeuron(neuron_hidden);
    layers_[i].SetCountWeightNeuron(neuron_hidden);
  }
  layers_[number_out_layer_].SetCountNeuron(neuron_out);
  layers_[number_out_layer_].SetCountWeightNeuron(neuron_hidden);
}

void GraphPerceptron::GenerateWeightNeuron() {
  for (size_t i = 1; i < count_layers_; i++) {
    layers_[i].SetWeightsNeuron();
  }
}

//_______________________________Parse_File________________________________//
void GraphPerceptron::LoadValuesTrain(std::string const &filename_train) {
  LoadFile(filename_train);
  vector_vectorovi4 = GetVector();
}

void GraphPerceptron::LoadValuesTest(std::string const &filename_test) {
  LoadFile(filename_test);
  vector_for_test = GetVector();
}

//__________________________________Train__________________________________//

void GraphPerceptron::EpochTrain(int epoch, std::vector<double> *report_graph) {
  percent_progress_ = 0;
  size_letter_for_epoch = epoch * (int)vector_vectorovi4.size();
  for (int k = 0; k < epoch; k++) {
    for (size_t d = 0;
         d < vector_vectorovi4.size() && this->stop_train_or_test_ == false;
         d++) {
      TrainForthBack(d);
      percent_progress_ = CalculateProgressPercent(d, k);
    }
    report_graph->push_back(100 - (CalculatePercentTrain() * 100));
  }
}

void GraphPerceptron::TrainForthBack(int d) {
  letter_to_learn_ = vector_vectorovi4[d][0] - 1;
  for (size_t i = 1; i < vector_vectorovi4[d].size(); i++) {
    layers_[0].SetNeuronValue(i, vector_vectorovi4[d][i] / 255);
  }
  ForwardPropagation();
  CountingSuccessfulLetters();
  BackwardPropagation();
}

//___________________________ForwardPropagation____________________________//
void GraphPerceptron::ForwardPropagation() {
  for (size_t l = 1; l < count_layers_ && this->stop_train_or_test_ == false;
       l++) {
    for (size_t n = 0; n < layers_[l].GetCountNeuron(); n++) {
      double sum = 0;
      for (size_t w = 0; w < layers_[l - 1].GetCountNeuron(); w++) {
        sum +=
            layers_[l - 1].GetNeuronValue(w) * layers_[l].GetWeightNeuron(n, w);
      }
      layers_[l].SetNeuronValue(n + 1, SigmoidFunction(sum));
    }
  }
}

//___________________________BackwardPropagation___________________________//
void GraphPerceptron::BackwardPropagation() {
  for (size_t l = number_out_layer_;
       l != 0 && this->stop_train_or_test_ == false; l--) {
    for (size_t n = 0; n < layers_[l].GetCountNeuron(); n++) {
      double error = 0;
      if (l == number_out_layer_) {
        if (n == letter_to_learn_) {
          error = layers_[number_out_layer_].GetNeuronValue(n) - 1;
        } else {
          error = layers_[number_out_layer_].GetNeuronValue(n);
        }

      } else {
        for (size_t n_r = 0; n_r < layers_[l + 1].GetCountNeuron(); n_r++) {
          error += layers_[l + 1].GetNeuronDelta(n_r) *
                   layers_[l + 1].GetWeightNeuron(n_r, n);
        }
      }
      layers_[l].SetNeuronError(n, error);

      double delta_weight =
          error * SigmoidDerivative(layers_[l].GetNeuronValue(n));

      layers_[l].SetNeuronDeltaWeight(n, delta_weight);
      WeightsCalculation(l, n, delta_weight);
    }
  }
}

void GraphPerceptron::WeightsCalculation(int l, int n, double delta_weight) {
  for (size_t w = 0; w < layers_[l - 1].GetCountNeuron(); w++) {
    double new_weight =
        layers_[l].GetWeightNeuron(n, w) -
        layers_[l - 1].GetNeuronValue(w) * delta_weight * learning_rate;
    layers_[l].SetNeuronWeights(n, w, new_weight);
  }
}

//____________________________Save_Load_Weights____________________________//

void GraphPerceptron::SaveWeights(std::string save_new_weights) {
  std::fstream outweights;

  outweights.open(save_new_weights, std::ios::out);

  if (!outweights) {
    std::cerr << "file couldn't be opened ";
    exit(1);
  }

  for (size_t l = 1; l < count_layers_; l++) {
    for (size_t n = 0; n < layers_[l].GetCountNeuron(); n++) {
      for (size_t n_w = 0; n_w < layers_[l - 1].GetCountNeuron(); n_w++) {
        outweights << layers_[l].GetWeightNeuron(n, n_w) << ",";
      }
    }
  }
  outweights.close();
}

void GraphPerceptron::LoadWeights(std::string const &filename_weights) {
  std::ifstream inweights(filename_weights, std::ios::in);
  if (!inweights.bad()) {
    std::string line;

    while (inweights >> line) {
      size_t i = 0;
      char *ptrEnd;

      if (isdigit(line[i])) {
        for (size_t l = 1; l < count_layers_; l++) {
          for (size_t n = 0; n < layers_[l].GetCountNeuron(); n++) {
            for (size_t n_w = 0; n_w < layers_[l - 1].GetCountNeuron(); n_w++) {
              //  static locale_t c_loc = newlocale(LC_ALL_MASK, "C", nullptr);
              //               layers_[l].SetNeuronWeights(n, n_w,
              //                                           strtod_l(&line[i],
              //                                           &ptrEnd, c_loc));
              layers_[l].SetNeuronWeights(n, n_w, strtod(&line[i], &ptrEnd));
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
//_________________________________Predict_________________________________//

std::vector<int> GraphPerceptron::Predict(std::string name_image) {
  LoadValuesTest(name_image);
  Test(full_sample);
  return FindMaximumPredict();
}

int GraphPerceptron::FindMaximum() {
  double maximum = 0.0;
  int index_max = 0;

  for (size_t n = 0; n < layers_[number_out_layer_].GetCountNeuron(); n++) {
    if (layers_[number_out_layer_].GetNeuronValue(n) > maximum) {
      maximum = layers_[number_out_layer_].GetNeuronValue(n);
      index_max = n;
    }
  }

  return index_max;
}

std::vector<int> GraphPerceptron::FindMaximumPredict() {
  std::vector<int> index_max(4);
  std::map<double, int> sort;
  for (size_t n = 0; n < layers_[number_out_layer_].GetCountNeuron(); n++) {
    sort.insert({layers_[number_out_layer_].GetNeuronValue(n), n});
  }
  std::map<double, int>::reverse_iterator answer = sort.rbegin();
  for (int i = 0; i < 4 && answer != sort.rend(); i++) {
    index_max[i] = answer->second + 1;
    answer++;
  }
  return index_max;
}

//_______________________________Test__________________________________//
void GraphPerceptron::Test(int test_sample) {
  percent_progress_ = 0;
  successful_find_letter_ = 0;
  size_letter_for_epoch =
      (int)vector_for_test.size() * test_sample / full_sample;
  for (size_t d = 0;
       d < (size_t)size_letter_for_epoch && this->stop_train_or_test_ == false;
       d++) {
    TestForthBack(d);
    percent_progress_ = CalculateProgressPercent(d, 0);
  }
  CalculatePercent(1);
}

void GraphPerceptron::TestForthBack(int d) {
  letter_to_learn_ = vector_for_test[d][0] - 1;
  for (size_t n = 1;
       n < vector_for_test[d].size() && this->stop_train_or_test_ == false;
       n++) {
    layers_[0].SetNeuronValue(n, vector_for_test[d][n] / 255);
  }
  ForwardPropagation();
  CountingSuccessfulLetters();
}

double GraphPerceptron::CalculatePercent(int k_group) {
  return successful_find_letter_ / (double)vector_for_test.size() / k_group;
}

void GraphPerceptron::CountingSuccessfulLetters() {
  if ((size_t)layers_[number_out_layer_].FindMaximum() == letter_to_learn_) {
    successful_find_letter_++;
    true_positive += 1;
    true_negative += 25;
  } else {
    false_negative += 1;
    false_positive += 1;
    true_negative += 24;
  }
}

void GraphPerceptron::Testing(char *filename_test) {
  LoadValuesTest(filename_test);
  Test(full_sample);
}

//__________________ResizePerceptron___________________//
void GraphPerceptron::Clear() {
  for (size_t i = 0; i < count_layers_; i++) {
    layers_[i].ClearFunction();
  }

  layers_.clear();
  for (size_t d = 0; d < vector_vectorovi4.size(); d++) {
    vector_vectorovi4[d].clear();
  }
  vector_vectorovi4.clear();
  for (size_t d = 0; d < vector_for_test.size(); d++) {
    vector_for_test[d].clear();
  }
  vector_for_test.clear();

  successful_find_letter_ = 0;
  letter_to_learn_ = 0;
  count_hidden_layers_ = 0;
  count_layers_ = 0;
  number_out_layer_ = 0;
}

void GraphPerceptron::ResizePerceptron(int count_hidden_layers) {
  Clear();
  count_hidden_layers_ = count_hidden_layers;
  count_layers_ = count_hidden_layers + 2;
  number_out_layer_ = count_layers_ - 1;
  layers_.resize(count_layers_);
  InitGraphPerceptron();
}


};  // namespace s21
