#pragma once
#include <string>
#include <vector>

//Create a struct to hold one row of data
struct PriceData {
	std::string date;
	double open,
		high,
		low,
		close,
		volume;
};

class DataLoader {
public: 
	//Create constructor
	DataLoader(const std::string& filename);
	std::vector<PriceData> getData() const;

private:
	std::vector<PriceData> data;
	void loadCSV(const std::string& filename);
};