#include "Strategy.hpp"
#include "Utils.hpp"
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>

Strategy::Strategy(const std::map<std::string, std::vector<PriceData>>& universe)
    : universe(universe){}

std::map<std::string, std::vector<double>> Strategy::generateCrossAlphas()
{
    size_t nDates = 0;
    for (auto& [ticker, prices] : universe)
        if (prices.size() > nDates)
            nDates = prices.size();

    const size_t momLookback = 5;
    const size_t mrLookback = 3;
    const size_t volLookback = 10;

    auto momMap = momentumSignals();
    auto mrMap = meanReversionSignals();
    auto volMap = volatilitySignals();

    std::map<std::string, std::vector<double>> alphas;

    for (size_t i = 0; i < nDates; i++) {
        std::vector<double> momVec, mrVec;

        for (auto& [ticker, prices] : universe) {
            if (i < prices.size()) {
                momVec.push_back(momMap[ticker][i]);
                mrVec.push_back(mrMap[ticker][i]);
            }
        }

        auto momNorm = zscoreNormalize(momVec);
        auto mrNorm = zscoreNormalize(mrVec);

        size_t idx = 0;

        for (auto& [ticker, prices] : universe) {
            if (i >= prices.size())
                continue;

            double alpha = 0.5 * momNorm[idx] + 0.3 * mrNorm[idx];
            double vol = volMap[ticker][i];
            if (vol < 1e-4)
                vol = 1e-4;
            alpha /= vol;
            alphas[ticker].push_back(alpha);
            idx++;
        }
    }

    return alphas;
}

std::map<std::string, std::vector<double>> Strategy::momentumSignals() {
    std::map<std::string, std::vector<double>> result;
    size_t lookback = 5;

    for (auto& [ticker, prices] : universe) {
        auto closes = extractCloses(prices);
        std::vector<double> sig(prices.size(), 0.0);

        for (size_t i = lookback; i < prices.size(); i++)
            sig[i] = computeMomentum(closes, i, lookback);

        result[ticker] = sig;
    }
    return result;
}

std::map<std::string, std::vector<double>> Strategy::meanReversionSignals() {
    std::map<std::string, std::vector<double>> result;
    size_t lookback = 3;

    for (auto& [ticker, prices] : universe) {
        auto closes = extractCloses(prices);
        std::vector<double> sig(prices.size(), 0.0);

        for (size_t i = lookback; i < prices.size(); i++)
            sig[i] = computeMeanReversion(closes, i, lookback);

        result[ticker] = sig;
    }
    return result;
}

std::map<std::string, std::vector<double>> Strategy::volatilitySignals() {
    std::map<std::string, std::vector<double>> result;
    size_t lookback = 10;

    for (auto& [ticker, prices] : universe) {
        auto closes = extractCloses(prices);
        std::vector<double> sig(prices.size(), 1.0);

        for (size_t i = lookback; i < prices.size(), i++)
            sig[i] = computeVolatility(closes, i, lookback);

        result[ticker] = sig;
    }
    return result;
}

double Strategy::computeMomentum(const std::vector<double>& closes, size_t index, size_t lookback) {
    if (index < lookback)
        return 0.0;

    double past = closes[index - lookback];
    if (past == 0.0)
        return 0.0;

    return (closes[index] - past) / past;
}

double Strategy::computeMeanReversion(const std::vector<double>& closes, size_t index, size_t lookback) {
    if (index + 1 < lookback)
        return 0.0;
    
    double sum = 0.0;

    for (size_t i = index - lookback + 1; i <= index; i++)
        sum += closes[i];

    double ma = sum / lookback;
    if (ma == 0.0)
        return 0.0;

    return (ma - closes[index]) / ma;
}

double Strategy::computeVolatility(const std::vector<double>& closes, size_t index, size_t lookback) {
    if (index < lookback)
        return 1.0;

    std::vector<double> returns;

    for (size_t i = index - lookback + 1; i <= index; i++) {
        if (closes[i - 1] == 0.0)
            continue;
        
        double r = (closes[i] - closes[i - 1]) / closes[i - 1];
        returns.push_back(r);
    }

    if (returns.empty())
        return 1.0;

    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double var = 0.0;

    for (double r : returns)
        var += (r - mean) * (r - mean);

    var /= returns.size();

    return std::max(0.001, std::sqrt(var));
}

std::vector<double> Strategy::extractCloses(const std::vector<PriceData>& prices) {
    std::vector<double> closes;
    closes.reserve(prices.size());

    for (auto& p : prices)
        closes.push_back(p.close);

    return closes;
}

std::vector<double> Strategy::zscoreNormalize(const std::vector<double>& values) {
    double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    double var = 0.0;

    for (double v : values)
        var += (v - mean) * (v - mean);

    var /= values.size();
    std::vector<double> result;
    result.reserve(values.size());
    
    for (double v : values)
        result.push_back((v - mean) / std::sqrt(var + 1e-6));

    return result;
}