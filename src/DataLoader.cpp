#include "DataLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

DataLoader::DataLoader(const std::string& filename) {
    loadCSV(filename);
}

void DataLoader::loadCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    // Skip header line
    if (!std::getline(file, line)) {
        std::cerr << "CSV file is empty: " << filename << "\n";
        return;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 6) continue;

        try {
            PriceData pd;
            pd.date = tokens[0];

            // Skip lines where Open is not a number
            pd.open = std::stod(tokens[1]);
            pd.high = std::stod(tokens[2]);
            pd.low = std::stod(tokens[3]);
            pd.close = std::stod(tokens[4]);
            pd.volume = std::stoll(tokens[5]);

            data.push_back(pd);
        }
        catch (const std::invalid_argument&) {
            // Skip junk lines
            continue;
        }
        catch (const std::out_of_range&) {
            continue;
        }
    }

    std::cout << "Loaded " << data.size() << " rows from " << filename << '\n';
}

std::vector<PriceData> DataLoader::getData() const {
    return data;
}