#include <iostream>
#include <iomanip>
#include "DataLoader.hpp"
#include "Utils.hpp"
#include "Strategy.hpp"
#include "Backtester.hpp"

int main()
{
    //Load CSV
    DataLoader loader("data/apple_5yr_one.csv");
    auto data = loader.getData();

    if (data.empty())
    {
        std::cerr << "No data loaded. Exiting.\n";
        return 1;
    }

    std::cout << "Loaded " << data.size() << " rows from CSV\n";
    std::cout << "First row: " << data[0].date
        << " Open: " << data[0].open
        << " Close: " << data[0].close << "\n";

    //Generate Alpha 
    Strategy strat(data);
    auto alphas = strat.generateAlphaScores();

    std::cout << "\nFirst 20 alpha values:\n";
    for (size_t i = 0; i < 20 && i < data.size(); ++i)
    {
        std::cout << std::fixed << std::setprecision(6)
            << data[i].date
            << "  Close: " << data[i].close
            << "  Alpha: " << alphas[i] << "\n";
    }

    //Compute average non-zero alpha
    double sumAlpha = 0.0;
    size_t countAlpha = 0;
    for (double a : alphas)
    {
        if (a != 0.0)
        {
            sumAlpha += a;
            countAlpha++;
        }
    }
    double avgAlpha = countAlpha > 0 ? sumAlpha / countAlpha : 0.0;
    std::cout << "Average non-zero alpha: " << avgAlpha << "\n";

    //Backtest
    Backtester bt(data, alphas);
    BacktestResult result = bt.run();

    std::cout << "\nBacktest results:\n";
    std::cout << "Total return: " << std::fixed << std::setprecision(2)
        << result.totalReturn * 100 << "%\n";
    std::cout << "Sharpe ratio: " << std::fixed << std::setprecision(2)
        << result.sharpRatio << "\n";

    //first 20 cumulative returns
    std::cout << "\nFirst 20 cumulative returns:\n";
    for (size_t i = 0; i < 20 && i < result.cumulativeReturns.size(); ++i)
    {
        std::cout << data[i].date << "  "
            << result.cumulativeReturns[i] << "\n";
    }

    return 0;
}