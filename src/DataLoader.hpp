#pragma once
#include <string>
#include <vector>
#include <map>

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
	DataLoader(const std::string& folderPath);
	const std::map<std::string, std::vector<PriceData>>& getAllData() const;

private:
	std::map<std::string, std::vector<PriceData>> allData;
	void loadCSV(const std::string& filename, const std::string& ticker);
};