#include "common.h"

namespace s21 {

void Common::LoadFile(std::string const &filename) {
  ClearVector();
  std::ifstream ifs(filename, std::ios::in);

  if (!ifs.bad()) {
    std::string line;
    std::string number;
    std::vector<double> parse_letter_;
    while (ifs >> line && this->stop_train_or_test_ == false) {
      parse_letter_.reserve(size_string);
      size_t i = 0;

      while (i < line.size()) {
        if (isdigit(line[i])) {
          char *ptrEnd;
          parse_letter_.push_back(strtod(&line[i], &ptrEnd));
          do {
            i++;
          } while (line[i] != ',' && i < line.size());
        }
        i++;
      }
      parse_file_.push_back(parse_letter_);
      parse_letter_.clear();
    }
  }
}

void Common::ClearVector() {
  for (size_t j = 0; j < parse_file_.size(); j++) {
    parse_file_[j].clear();
  }
  parse_file_.clear();
}

std::vector<double> Common::CalculateMetrics() {
  metrics.clear();
  accuracy =
      (double)(true_positive + true_negative) /
      (double)(true_positive + true_negative + false_positive + false_negative);
  precision = (double)true_positive / (double)(true_positive + false_positive);
  recall = (double)true_positive / (double)(true_positive + false_negative);
  f_measure = 2 * ((precision * recall) / (precision + recall));

  metrics.push_back(accuracy);
  metrics.push_back(precision);
  metrics.push_back(recall);
  metrics.push_back(f_measure);
  return metrics;
}

int Common::CalculateProgressPercent(int d, int k) {
  return (((d + 1) + k * (int)vector_vectorovi4.size())) * 100 /
         size_letter_for_epoch;
}

void Common::CrossValidation(std::string filename_train, int k_validation) {
  percent_progress_ = 0;
  k = k_validation;
  LoadFile(filename_train);
  vector_vectorovi4 = GetVector();
  vector_for_test = GetVector();
  size_letter_for_epoch = k_validation * (int)vector_vectorovi4.size();
  int one_part = vector_vectorovi4.size() / k;

  for (int k_count = 1; k_count <= k; k_count++) {
    successful_find_letter_ = 0;
    int progress_count = 0;
    for (int k_inner = 1; k_inner <= k; k_inner++) {
      if (k_inner == k_count) k_inner++;
      for (int d = one_part * (k_inner - 1);
           k_inner <= k && d < (one_part * k_inner) &&
           this->stop_train_or_test_ == false;
           d++, progress_count++) {
        TrainForthBack(d);
        percent_progress_ =
            CalculateProgressPercent(progress_count, k_count - 1);
      }
    }
    progress_count = one_part * (k - 1);
    for (int d = one_part * (k_count - 1); d < (one_part * k_count);
         d++, progress_count++) {
      TestForthBack(d);
      percent_progress_ = CalculateProgressPercent(progress_count, k_count - 1);
    }
    CalculatePercent(k);
    if (k_count < k) GenerateWeightNeuron();
  }
}

}  // namespace s21
