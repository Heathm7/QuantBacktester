#pragma once
#include <vector>
#include "DataLoader.hpp"
#include "Strategy.hpp"

struct BacktestResult {
	std::vector<double> dailyReturns;
	std::vector<double> cumulativeReturns;
	double totalReturn;
	double sharpRatio;
};

class Backtester {
public:
	Backtester(const std::vector<PriceData>& data, const std::vector<double>& alphas);
	BacktestResult run(double capital = 100000.0);
private:
	std::vector<PriceData> data;
	std::vector<double> alphas;
	std::vector<double> computePositions();
	std::vector<double> computeDailyReturns(const std::vector<double>& positions);
	double computeSharpe(const std::vector<double>& dailyReturns);
};