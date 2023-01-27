#include "controller.h"

namespace s21 {
void Controller::SetPerceptronType(bool perceptron_type) {
  perceptron_type_ = perceptron_type;
}

void Controller::SetCountHiddenLayers(int hidden_layers) {
  hidden_layers_ = hidden_layers;
}

void Controller::SetWeights(std::string filename_weights) {
  filename_weights_ = filename_weights;
}

void Controller::SaveWeights(std::string save_new_weights) {
  if (perceptron_type_ == 0) {
    g_net->SaveWeights(save_new_weights);
  } else {
    m_net->SaveWeights(save_new_weights);
  }
}

void Controller::SetNameTrain(char *filename_train) {
  filename_train_ = filename_train;
}

void Controller::SetNameTest(std::string const &filename_test) {
  filename_test_ = filename_test;
}

std::vector<int> Controller::Predict(std::string name_image) {
  std::vector<int> answer;
  if (perceptron_type_ == 0) {
    SetStopTrainOrTest(false);
    g_net->LoadWeights(filename_weights_);
    answer = g_net->Predict(name_image);
  } else {
    SetStopTrainOrTest(false);
    m_net->LoadWeights(filename_weights_);
    answer = m_net->Predict(name_image);
  }
  return answer;
}

void Controller::Change(int hidden, int type) {
  perceptron_type_ = type;
  hidden_layers_ = hidden;
  if (type == 0) {
    g_net->ResizePerceptron(hidden);
  } else {
    m_net->ResizePerceptron(hidden);
  }
  perceptron_type_ = type;
}

void Controller::Train(int epoch, std::string filename_train,
                       std::vector<double> *report_graph) {
  if (perceptron_type_ == 0) {
    g_net->setProgressProcent(0);
    g_net->LoadValuesTrain(filename_train);
    g_net->GenerateWeightNeuron();
    g_net->EpochTrain(epoch, report_graph);

  } else {
    m_net->setProgressProcent(0);
    m_net->LoadValuesTrain(filename_train);
    m_net->GenerateWeightNeuron();
    m_net->EpochTrain(epoch, report_graph);
  }
}

void Controller::Testing(int test_sample) {
  if (perceptron_type_ == 0) {
    g_net->setProgressProcent(0);
    g_net->LoadValuesTest(filename_test_);
    g_net->Test(test_sample);
  } else {
    m_net->setProgressProcent(0);
    m_net->LoadValuesTest(filename_test_);
    m_net->Test(test_sample);
  }
}

void Controller::Validation(int k, std::string filename) {
  if (perceptron_type_ == 0) {
    g_net->setProgressProcent(0);
    g_net->GenerateWeightNeuron();
    g_net->CrossValidation(filename, k);
  } else {
    m_net->setProgressProcent(0);
    m_net->GenerateWeightNeuron();
    m_net->CrossValidation(filename, k);
  }
}

int Controller::getProgressProcent() {
  int procent = 0;
  if (perceptron_type_ == 0) {
    procent = g_net->getProgressProcent();
  } else {
    procent = m_net->getProgressProcent();
  }

  return procent;
}

std::vector<double> Controller::GetMetrics() {
  if (perceptron_type_ == 0) {
    metrics = g_net->CalculateMetrics();
  } else {
    metrics = m_net->CalculateMetrics();
  }
  return metrics;
}

void Controller::SetStopTrainOrTest(bool var) {
  if (perceptron_type_ == 0) {
    g_net->SetStopTrainOrTest(var);
  } else {
    m_net->SetStopTrainOrTest(var);
  }
}

}  // namespace s21
