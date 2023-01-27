#include "neuron.h"

s21::Neuron::Neuron() {}

void s21::Neuron::GenerateWeight() {
  for (size_t i = 0; i < array_weight_.size(); i++) {
    array_weight_[i] = 0.001 * (std::rand() % 2001 - 1000);
  }
}

void s21::Neuron::SetCountWeight(int count_weight) {
  count_weight_ = count_weight;
  array_weight_.resize(count_weight);
}

void s21::Neuron::SetValue(double value) { value_ = value; }
void s21::Neuron::SetError(double error) { error_ = error; }
void s21::Neuron::SetDeltaWeight(double delta_weight) {
  delta_weight_ = delta_weight;
}
void s21::Neuron::SetWeightNeuron(int weight_index, double weight) {
  array_weight_[weight_index] = weight;
}

double s21::Neuron::GetValue() { return value_; }
double s21::Neuron::GetError() { return error_; }
double s21::Neuron::GetNeuronDelta() { return delta_weight_; }
double s21::Neuron::GetWeightVector(int index_weight) {
  return array_weight_[index_weight];
}

void s21::Neuron::ClearFunction() {
  value_ = 0;
  count_weight_ = 0;
  array_weight_.clear();
  delta_weight_ = 0;
  error_ = 0;
}

