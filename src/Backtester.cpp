#include "Backtester.hpp"
#include <cmath>
#include <numeric>
#include <iostream>

Backtester::Backtester(const std::vector<PriceData>& data,
						const std::vector<double>& alphas) : data(data), alphas(alphas){}

BacktestResult Backtester::run(double capital) {
	BacktestResult result;
	std::vector<double> position = computePositions();
	result.dailyReturns = computeDailyReturns(position);
	result.cumulativeReturns.resize(result.dailyReturns.size());
	double cR = capital;
	
	for (size_t i = 0; i < result.dailyReturns.size(); i++) {
		cR *= (1.0 + result.dailyReturns[i]);
		result.cumulativeReturns[i] = cR;
	}

	result.totalReturn = result.cumulativeReturns.back() / capital - 1.0;
	result.sharpRatio = computeSharpe(result.dailyReturns);

	return result;
}

std::vector<double> Backtester::computePositions() {
	std::vector<double> positions(alphas.size(), 0.0);
	double maxAlpha = 0.0;
	
	for (double a : alphas)
		if (std::abs(a) > maxAlpha)
			maxAlpha = std::abs(a);
	
	if (maxAlpha < 1e-6)
		maxAlpha = 1.0;

	double alphaScale = 0.1;

	for (size_t i = 0; i < alphas.size(); i++)
		
		positions[i] = alphas[i] / maxAlpha * alphaScale;	

	return positions;
}

std::vector<double> Backtester::computeDailyReturns(const std::vector<double>& positions) {
	std::vector<double> dailyReturns(data.size(), 0.0);

	for (size_t i = 1; i < data.size(); i++) {
		double ret = (data[i].close - data[i - 1].close) / data[i - 1].close;
		dailyReturns[i] = positions[i - 1] * ret;
	}

	return dailyReturns;
}

double Backtester::computeSharpe(const std::vector<double>& dailyReturns) {
	if (dailyReturns.size() < 2)
		return 0.0;

	double mean = std::accumulate(dailyReturns.begin(), dailyReturns.end(), 0.0);
	double variance = 0.0;

	for (double r : dailyReturns)
		variance += (r - mean) * (r - mean);

	variance /= (dailyReturns.size() - 1);
	double stddev = std::sqrt(variance);

	if (stddev < 1e-6)
		return 0.0;

	return mean / stddev * std::sqrt(252.0);
}