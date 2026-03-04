#pragma once
#include <vector>
#include "DataLoader.hpp"

class Utils {
public: 
	static double simpleMovingAverage(
		const std::vector<double>& values,
		size_t period,
		size_t index
	);

	static std::vector<double> extractCloses(
		const std::vector<PriceData>& data
	);
};