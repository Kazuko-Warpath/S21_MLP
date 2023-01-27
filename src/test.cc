#include <gtest/gtest.h>
#include "model/common.h"
#include "model/graph_perceptron.h"
#include "model/matrix_perceptron.h"
#include <vector>

namespace s21 {

    TEST(MLP_TEST, test1) {
        MatrixPerceptron m_net(3);
        m_net.LoadWeights("model/weights/3_layers.weights");
        m_net.LoadValuesTest("../datasets/test.csv");
        m_net.Test(5);
        ASSERT_TRUE(m_net.FindMaximum() == 10);
    }

    TEST(MLP__TEST, test2) {
        MatrixPerceptron m_net(3);
        m_net.LoadValuesTrain("../datasets/test.csv");
        m_net.GenerateWeightNeuron();
        std::vector<double> report_graph;
        m_net.EpochTrain(1, &report_graph);
        m_net.SaveWeights("model/fictive.weights");
        m_net.CalculateMetrics();
        m_net.CrossValidation("../datasets/test.csv", 1);
        m_net.FindMaximumPredict();
        m_net.ResizePerceptron(4);
    }

    TEST(MLP__TEST, test3) {
        GraphPerceptron g_net(3);
        g_net.LoadValuesTrain("../datasets/test.csv");
        g_net.GenerateWeightNeuron();
        std::vector<double> report_graph;
        g_net.EpochTrain(1, &report_graph);
        g_net.SaveWeights("model/fictive.weights");
        g_net.CalculateMetrics();
        g_net.CrossValidation("../datasets/test.csv", 1);
        g_net.FindMaximumPredict();
        g_net.ResizePerceptron(4);
    }

    TEST(MLP_TEST, test4) {
        GraphPerceptron g_net(3);
        g_net.LoadWeights("model/weights/3_layers.weights");
        g_net.LoadValuesTest("../datasets/test.csv");
        g_net.Test(5);
        ASSERT_TRUE(g_net.FindMaximum() == 10);
    }

    int main(int argc, char **argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
} 
