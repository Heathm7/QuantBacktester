#pragma once
#include <vector>
#include <map>
#include <string>
#include "DataLoader.hpp"

class Strategy {
public:
	Strategy(const std::map<std::string, std::vector<PriceData>>* universe);

	//Generate the alpha for each time index
	std::map<std::string, std::vector<double>> generateCrossAlphas();

private:
	std::map<std::string, std::vector<PriceData>> universe;
	std::map<std::string, std::vector<double>> momentumSignals();
	std::map<std::string, std::vector<double>> meanReversionSignals();
	std::map<std::string, std::vector<double>> volatilitySignals();

	double computeMomentum(const std::vector<double>& closes, size_t index, size_t lookback);
	double computeMeanReversion(const std::vector<double>& closes, size_t index, size_t lookback);
	double computeVolatility(const std::vector<double>& closes, size_t index, size_t lookback);

	std::vector<double> extractCloses(const std::vector<PriceData>& prices);
	std::vector<double> zscoreNormalize(const std::vector<double>& values);
};