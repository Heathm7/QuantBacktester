#pragma once
#include <vector>
#include "DataLoader.hpp"

class Strategy {
public:
	Strategy(const std::vector<PriceData>& data);

	//Generate the alpha for each time index
	std::vector<double> generateAlphaScores();

private:
	std::vector<PriceData> data;
	std::vector<double> closes;

	double momentumSignal(size_t index, size_t lookback);
	double meanReversionSignal(size_t index, size_t lookback);
	double volatility(size_t index, size_t lookback);
};