#include "Strategy.hpp"
#include "Utils.hpp"
#include <cmath>
#include <stdexcept>

Strategy::Strategy(const std::vector<PriceData>& data)
    : data(data)
{
    closes = Utils::extractCloses(data);
}

std::vector<double> Strategy::generateAlphaScores()
{
    std::vector<double> alphas(data.size(), 0.0);

    const size_t momLookback = 5;
    const size_t mrLookback = 3;
    const size_t volLookback = 10;

    for (size_t i = 0; i < data.size(); ++i)
    {
        if (i < volLookback)
            continue; 

        double mom = momentumSignal(i, momLookback);
        double mr = meanReversionSignal(i, mrLookback);
        double vol = volatility(i, volLookback);

        if (vol < 1e-3)
            vol = 1e-3;

        // Linear alpha combination
        double alpha = 0.5 * mom + 0.3 * mr;
        //volatility scalling explodes with 1 stock
        //alpha /= (vol + 1e-6);
        alphas[i] = alpha;
    }

    return alphas;
}

double Strategy::momentumSignal(size_t index, size_t lookback)
{
    if (index < lookback)
        return 0.0;

    double pastPrice = closes[index - lookback];
    if (pastPrice == 0.0)
        return 0.0;

    return (closes[index] - pastPrice) / pastPrice;
}

double Strategy::meanReversionSignal(size_t index, size_t lookback)
{
    if (index + 1 < lookback)
        return 0.0;

    double ma = Utils::simpleMovingAverage(closes, lookback, index);
    if (ma == 0.0)
        return 0.0;

    return (ma - closes[index]) / ma;
}

double Strategy::volatility(size_t index, size_t lookback)
{
    if (index < lookback)
        return 1.0;  

    double mean = 0.0;
    std::vector<double> returns;
    returns.reserve(lookback);

    for (size_t i = index - lookback + 1; i <= index; ++i)
    {
        if (closes[i - 1] == 0.0)
            continue;

        double r = (closes[i] - closes[i - 1]) / closes[i - 1];
        returns.push_back(r);
        mean += r;
    }

    if (returns.empty())
        return 1.0;

    mean /= returns.size();

    double variance = 0.0;
    for (double r : returns)
        variance += (r - mean) * (r - mean);

    variance /= returns.size();

    double vol = std::sqrt(variance);

    if (vol < 0.001)
        vol = 0.001;

    return vol;
}