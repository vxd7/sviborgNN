#pragma once

#include <vector>

std::vector<std::vector<double>> MistakeWeights(double SpeedTrain, std::vector<double> Mistake, std::vector<double> BeforeOutput);
std::vector<double> MistakeBias(double SpeedTrain, std::vector<double> Mistake);
std::vector<double> SumMistake(std::vector<std::vector<double>> Weights, std::vector<double> Mistake);