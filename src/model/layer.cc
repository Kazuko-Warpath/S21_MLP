#include "layer.h"
s21::Layer::Layer() {}

int s21::Layer::FindMaximum() {
  double maximum = 0.0;
  int index_max = 0;
  for (size_t i = 0; i < layer_.size(); i++) {
    if (GetNeuronValue(i) > maximum) {
      maximum = GetNeuronValue(i);
      index_max = i;
    }
  }
  return index_max;
}

void s21::Layer::SetCountNeuron(int count_neuron) {
  count_neuron_ = count_neuron;
  layer_.resize(count_neuron);
}

void s21::Layer::SetCountWeightNeuron(int count_weight_neuron) {
  count_weight_neuron_ = count_weight_neuron;
  for (size_t i = 0; i < count_neuron_; i++) {
    layer_[i].SetCountWeight(count_weight_neuron_);
  }
}

void s21::Layer::SetWeightsNeuron() {
  for (size_t k = 0; k < count_neuron_; k++) {
    layer_[k].GenerateWeight();
  }
}

void s21::Layer::SetNeuronValue(int number_neuron, double value) {
  layer_[number_neuron - 1].SetValue(value);
}

void s21::Layer::SetNeuronError(int index, double error) {
  layer_[index].SetError(error);
}

void s21::Layer::SetNeuronWeights(int index, int index_weight, double weight) {
  layer_[index].SetWeightNeuron(index_weight, weight);
}

void s21::Layer::SetErrorCalculation(int index, double error) {
  layer_[index].SetError(error);
}

void s21::Layer::SetNeuronDeltaWeight(int index, double delta_weight) {
  layer_[index].SetDeltaWeight(delta_weight);
}

size_t s21::Layer::GetCountNeuron() { return count_neuron_; }

double s21::Layer::GetNeuronValue(int index) {
  return layer_[index].GetValue();
}

double s21::Layer::GetNeuronError(int index) {
  return layer_[index].GetError();
}

double s21::Layer::GetNeuronDelta(int index) {
  return layer_[index].GetNeuronDelta();
}

double s21::Layer::GetWeightNeuron(int index, int weight_index) {
  return layer_[index].GetWeightVector(weight_index);
}

void s21::Layer::ClearFunction() {
  for (size_t i = 0; i < count_neuron_; i++) {
    layer_[i].ClearFunction();
  }
  count_neuron_ = 0;
  count_weight_neuron_ = 0;
}


