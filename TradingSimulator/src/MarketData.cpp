#include "MarketData.h"
#include <stdexcept>
#include <numeric>

double MarketData::calculateMovingAverage(const std::vector<MarketData>& data, size_t currentIndex, size_t period){
    if(currentIndex < period-1){
        throw std::invalid_argument("Not enough data to calculate the moving average.");
    }

    double sum = std::accumulate(data.begin() + (currentIndex - period + 1), data.begin() + (currentIndex + 1), 0.0, [](double acc, const MarketData& m){
        return acc + m.close;
    });

    return sum/period;
}

double MarketData::calculateRSI(const std::vector<MarketData>& data, size_t currentIndex, size_t period){
    if(currentIndex < period - 1){
        throw std::invalid_argument("Not enough data to calculate RSI.");
    }

    double gain = 0.0, loss = 0.0;
    for(size_t i = currentIndex - period + 1; i <= currentIndex; ++i){
        double change = data[i].close - data[i - 1].close;
        if(change > 0){
            gain += change;
        } else {
            loss -= change;
        }
    }

    double avgGain = gain / period;
    double avgLoss = loss / period;

    if(avgLoss == 0){
        return 100.0;
    }

    double rs = avgGain / avgLoss;
    return 100.0 - (100.0 / (1.0 + rs));
}