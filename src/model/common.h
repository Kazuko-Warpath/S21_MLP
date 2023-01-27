#ifndef SRC_MODEL_COMMON_H_
#define SRC_MODEL_COMMON_H_

#include <atomic>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <vector>

namespace s21 {
constexpr int size_string = 1024;
constexpr double learning_rate = 0.1;
constexpr double neuron_out = 26;
constexpr double neuron_hidden = 140;
constexpr double neuron_in = 784;
constexpr int full_sample = 5;

class Common {
 public:
  void LoadFile(std::string const &filename);
  std::vector<std::vector<double>> GetVector() { return parse_file_; }
  // void ViewImg();
  double SigmoidFunction(double value) { return 1.0 / (1.0 + exp(-value)); }
  double SigmoidDerivative(double value) { return value * (1 - value); }
  void ClearVector();
  std::vector<double> CalculateMetrics();
  int GetSize() { return vector_vectorovi4.size(); }
  int CalculateProgressPercent(int d, int k);
  void setProgressProcent(int value) { percent_progress_ = value; }
  int getProgressProcent() { return percent_progress_; };
  void CrossValidation(std::string filename_train, int k_validation);
  virtual void TrainForthBack(int d) = 0;
  virtual void TestForthBack(int d) = 0;
  virtual double CalculatePercent(int k_group) = 0;
  void GenerateWeightNeuron() {}

  double CalculatePercentTrain() {
    double k =
        ((double)successful_find_letter_ / (double)vector_vectorovi4.size());
    return k;
  }

  void SetExitTrain(bool var) { exit_train_ = var; }

  void SetStopTrainOrTest(bool var) { stop_train_or_test_ = var; }

 protected:
  bool exit_train_{false};

  int successful_find_letter_{};
  size_t letter_to_learn_{};
  std::atomic<int> percent_progress_{};
  int percenttest{};
  int size_letter_for_epoch{};

  std::vector<std::vector<double>> vector_vectorovi4;
  std::vector<std::vector<double>> vector_for_test;
  std::vector<std::vector<double>> parse_file_;

  int true_positive{};
  int true_negative{};
  int false_positive{};
  int false_negative{};

  double time{};

  int crossvalidation{};
  int k{};
  int anti_k{};

  int current_percent{};
  int vec_size{};
  std::mutex m_mutex;
  bool stop_train_or_test_{false};

  double accuracy{};
  double f_measure{};
  double precision{};
  double recall{};
  std::vector<double> metrics{};
};

}  // namespace s21
#endif  // SRC_MODEL_COMMON_H_
