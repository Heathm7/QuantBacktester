#include "DataLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
namespace fs = std::filesystem;

DataLoader::DataLoader(const std::string& folderPath) {
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".csv") {
            std::string filename = entry.path().string();
            std::string ticker = entry.path().stem().string();
            loadCSV(filename, ticker);
        }
    }
}

void DataLoader::loadCSV(const std::string& filename, const std::string& ticker) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    std::getline(file, line);
    std::vector<PriceData> data;

    while (std::getline(file, line)) {
        if (line.empty()) 
            continue;

        std::stringstream ss(line);
        std::string token;
        PriceData pd;

        std::getline(ss, pd.date, ',');
        std::getline(ss, token, ','); 
        pd.open = std::stod(token);
        std::getline(ss, token, ','); 
        pd.high = std::stod(token);
        std::getline(ss, token, ','); 
        pd.low = std::stod(token);
        std::getline(ss, token, ','); 
        pd.close = std::stod(token);
        std::getline(ss, token, ','); 
        pd.volume = std::stoll(token);
        data.push_back(pd);
    }
    
    allData[ticker] = data;
    std::cout << "Loaded " << data.size() << " rows from " << filename << '\n';
}

const std::map<std::string, std::vector<PriceData>>& DataLoader::getAllData() const {
    return allData;
}