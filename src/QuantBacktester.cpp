#include "DataLoader.hpp"
#include <iostream>

int main() {
    DataLoader loader("data/apple_5yr_one.csv");
    auto data = loader.getData();

    std::cout << "First row: "
        << data[0].date << " Open: " << data[0].open
        << " Close: " << data[0].close << "\n";

    std::cout << "Total rows loaded: " << data.size() << "\n";

    return 0;
}