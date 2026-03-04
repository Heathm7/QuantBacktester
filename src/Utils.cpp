#include "Utils.hpp"
#include <stdexcept>

double Utils::simpleMovingAverage(const std::vector<double>& values, size_t period, size_t index) {
	if (index + 1 < period) {
		throw std::invalid_argument("Not enough data for moving average");
	}

	double sum = 0.0;
	for (size_t i = index + 1 - period; i < index; i++) {
		sum += values[i];
	}

	return sum / period;
}

std::vector<double> Utils::extractCloses(const std::vector<PriceData>& data) {
	std::vector<double> closes;
	closes.reserve(data.size());

	for (const auto& row : data) {
		closes.push_back(row.close);
	}

	return closes;
}