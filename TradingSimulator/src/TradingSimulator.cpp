#include "TradingSimulator.h"
#include <iostream>
#include <stdexcept>

TradingSimulator::TradingSimulator(double initialBalance, const std::vector<MarketData>& marketData) : balance(initialBalance), data(marketData) {}

void TradingSimulator::buy(const std::string& ticker, double amount, const MarketData& marketData){
    if(amount > balance){
        throw std::runtime_error("Insufficient funds for purchase.");
    } else if(amount <= 0){
        throw std::invalid_argument("The amount must be greater than 0.");
    }

    int sharesToBuy = static_cast<int> (amount / marketData.close);
    double totalCost = sharesToBuy * marketData.close; 
    if (sharesToBuy <= 0){
        throw std::runtime_error("Amount too low to buy any shares.");
    }
    balance -= totalCost;
    portfolio[ticker] += sharesToBuy;

    std::cout << "Bought " << sharesToBuy << " shares of " << ticker <<" at €" << marketData.close << " per share.\n";
    operations.push_back({ticker,marketData.date, "Buy", marketData.close, balance, portfolio[ticker]});
}

void TradingSimulator::sell(const std::string& ticker, double amount, const MarketData& marketData){
    int sharesToSell = static_cast<int>(amount / marketData.close);

    if(sharesToSell > portfolio[ticker]){
        throw std::runtime_error("Insufficient shares for sell.");
    }  else if(amount <= 0){
        throw std::invalid_argument("The amount must be greater than 0.");
    }

    double totalGain = sharesToSell * marketData.close; 

    balance += totalGain;
    portfolio[ticker] -= sharesToSell;

    std::cout << "Sold " << sharesToSell << " shares of " << ticker << " at €" << marketData.close << " per share.\n";
    operations.push_back({ticker, marketData.date, "Sell", marketData.close, balance, portfolio[ticker]});
}

void TradingSimulator::executeAveragePeriod(const std::vector<MarketData>& data, size_t movingAveragePeriod){
    for(size_t i = movingAveragePeriod - 1; i < data.size(); i++){
        double movingAverage = MarketData::calculateMovingAverage(data, i, movingAveragePeriod);

        const auto& currentData = data[i];
        if(currentData.close < movingAverage && balance > currentData.close){
            std::cout << "\nAutomatic purchase of " << currentData.ticker << " on " << currentData.date << " (Price: " << currentData.close << ")\n";
            buy(currentData.ticker, balance, currentData);
        } else if (currentData.close > movingAverage && portfolio[currentData.ticker] > 0){
            std::cout << "\nAutomatic sale of " << currentData.ticker << " on " << currentData.date << " (Price: " << currentData.close << ")\n";
            sell(currentData.ticker, portfolio[currentData.ticker] * currentData.close, currentData);
        }
    }
}

void TradingSimulator::executeRSI(const std::vector<MarketData>& data, size_t rsiPeriod){
    for(size_t i = rsiPeriod - 1; i < data.size(); i++){
        double rsi = MarketData::calculateRSI(data, i, rsiPeriod);

        const auto& currentData = data[i];
        std::cout << "Date: " << currentData.date
                  << ", Close: " << currentData.close
                  << ", RSI: " << rsi << std::endl;

        if(rsi < 30 && balance > currentData.close){
            std::cout << "\nRSI Buy signal for " << currentData.ticker << " on " << currentData.date << " (RSI: " << rsi << ", Price: " << currentData.close << ")\n";
            buy(currentData.ticker, balance, currentData);
        } else if(rsi > 70 && portfolio[currentData.ticker] > 0){
            std::cout << "\nRSI Sell signal for " << currentData.ticker << " on " << currentData.date << " (RSI: " << rsi << ", Price: " << currentData.close << ")\n";
            sell(currentData.ticker, portfolio[currentData.ticker] * currentData.close, currentData);
        }
    }
}
    

void TradingSimulator::saveReport(const std::string& filename){
    std::ofstream file(filename);
    if(!file.is_open()){
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    file << "Ticker,Date,Type,Price,Balance,Shares\n";
    for(const auto& op : operations){
        file << op.ticker << "," << op.date << "," << op.type << "," << op.price << "," << op.balance << "," << op.shares << "\n";
    }
    file.close();
    std::cout << "\nReport saved in " << filename << "\n\n";
}

void TradingSimulator::displayStatistics(double initialBalance) const {
    int totalBuys = 0, totalSells = 0;
    
    for (const auto& op : operations){
        if(op.type == "Buy")
            totalBuys++;
        if(op.type == "Sell")
            totalSells++;
    }

    double profit = balance - initialBalance;
    double profitPercentage = (profit/initialBalance) * 100;
    
    std::cout << "\n----- Trading Statistics -----\n\n";
    std::cout << "Initial Balance: €" << initialBalance << "\n";
    std::cout << "Final Balance: €" << balance << "\n";
    std::cout << "Total Profit: €" << profit << " (" << profitPercentage << "%)\n";

    std::cout << "\nPortfolio:\n";
    for(const auto& p : portfolio){
        std::cout << "   " << p.first << ": " << p.second << " shares\n";
    }

    std::cout << "\nTotal operations: " << operations.size() << "\n";
    std::cout << "   - Purchases: " << totalBuys << "\n";
    std::cout << "   - Sales: " << totalSells << "\n\n";
    std::cout << "------- End Statistics -------\n";
}