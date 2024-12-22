#ifndef TRADINGSIMULATOR_H
#define TRADINGSIMULATOR_H

#include <vector>
#include <string>
#include <map>
#include "MarketData.h"
#include <fstream>

struct TradeOperation{
    std::string ticker;
    std::string date;
    std::string type;
    double price;
    double balance;
    int shares;
};

class TradingSimulator {
private:
    double balance;
    std::map<std::string, int> portfolio;
    std::vector<MarketData> data;
    std::vector<TradeOperation> operations;

public:
    TradingSimulator(double initialBalance, const std::vector<MarketData>& marketData);

    void buy(const std::string& ticker, double amount, const MarketData& marketData);
    void sell(const std::string& ticker, double amount, const MarketData& marketData);
    void executeAveragePeriod(const std::vector<MarketData>& data, size_t movingAveragePeriod);
    void executeRSI(const std::vector<MarketData>& data, size_t rsiPeriod);
    void saveReport(const std::string& filename);
    void displayStatistics(double initialBalance) const;
};

#endif