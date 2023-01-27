#ifndef SRC_NEURON_H_
#define SRC_NEURON_H_

#include <cmath>
#include <iostream>
#include <vector>

namespace s21 {
class Neuron {
 public:
  Neuron();
  ~Neuron() {}
  void GenerateWeight();
  void PrintNeuron();

  void SetCountWeight(int count_weight);
  void SetValue(double value);
  void SetError(double error);
  void SetDeltaWeight(double delta_weight);
  void SetWeightNeuron(int weight_index, double weight);

  double GetValue();
  double GetError();
  double GetNeuronDelta();
  double GetWeightVector(int index_weight);

  void ClearFunction();

 private:
  double value_{};
  int count_weight_;
  std::vector<double> array_weight_;
  double delta_weight_;
  double error_;
};

}  // namespace s21
#endif  // SRC_NEURON_H_
