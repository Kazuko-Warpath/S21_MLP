#ifndef SRC_LAYER_H_
#define SRC_LAYER_H_
#include <vector>

#include "neuron.h"
namespace s21 {

class Layer {
 public:
  Layer();
  int FindMaximum();
  // void PrintLayer();

  void SetCountNeuron(int count_neuron);
  void SetCountWeightNeuron(int count_weight_neuron);
  void SetWeightsNeuron();
  void SetNeuronValue(int number_neuron, double value);
  void SetNeuronError(int index, double error);
  void SetNeuronWeights(int index, int index_weight, double weight);
  void SetErrorCalculation(int index, double error);
  void SetNeuronDeltaWeight(int index, double delta_weight);

  size_t GetCountNeuron();
  double GetNeuronValue(int index);
  double GetNeuronError(int index);
  double GetNeuronDelta(int index);
  double GetNeuronExpectedValue(int index);
  double GetWeightNeuron(int index, int weight_index);

  void ClearFunction();

 private:
  size_t count_neuron_;
  size_t count_weight_neuron_;
  std::vector<Neuron> layer_;
};

}  // namespace s21
#endif  // SRC_LAYER_H_
