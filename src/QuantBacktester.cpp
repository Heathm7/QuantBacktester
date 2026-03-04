#include "DataLoader.hpp"
#include "Strategy.hpp"
#include <iostream>

int main() {
    DataLoader loader("data/apple_5yr_one.csv");
    auto data = loader.getData();

    std::cout << "First row: "
        << data[0].date << " Open: " << data[0].open
        << " Close: " << data[0].close << "\n";

    std::cout << "Total rows loaded: " << data.size() << "\n";

    Strategy strat(data);
    auto alphas = strat.generateAlphaScores();

    std::cout << "First 20 alpha values:\n";

    for (size_t i = 0; i < 20 && i < alphas.size(); ++i)
    {
        std::cout << data[i].date
            << "  Close: " << data[i].close
            << "  Alpha: " << alphas[i]
            << "\n";
    }

    double sum = 0.0;
    int count = 0;

    for (double a : alphas)
    {
        if (!std::isnan(a) && a != 0.0) {
            sum += a;
            count++;
        }
    }

    if (count > 0)
        std::cout << "\nAverage non-zero alpha: "
        << sum / count << "\n";

    return 0;
}