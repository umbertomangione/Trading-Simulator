#ifndef MARKETDATA_H
#define MARKETDATA_H

#include <string>
#include <vector>

struct MarketData {
    std::string ticker;
    std::string date;
    double open;
    double close;
    double high;
    double low;
    int volume;

    static double calculateMovingAverage(const std::vector<MarketData>& data, size_t currentIndex, size_t period);
    static double calculateRSI(const std::vector<MarketData>& data, size_t currentIndex, size_t period);
};

#endif