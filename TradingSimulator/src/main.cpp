#include "DataLoader.h"
#include "TradingSimulator.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <map>

std::regex numberRegex("([1-6]*)");

int main(){
    try {
        double initialBalance = 10000.0;
        
        DataLoader loader;
        auto portfolio = loader.loadCSV("./data/historical_data.csv");

        // Scegli il ticker da simulare
        std::string ticker;
        std::cout << "Available tickers: ";
        for (const auto& [key, _] : portfolio) {
            std::cout << key << " ";
        }
        std::cout << "\nChoose a ticker to simulate: ";
        std::cin >> ticker;
        std::cin.ignore();

        if (portfolio.find(ticker) == portfolio.end()) {
            std::cerr << "Ticker not found!" << std::endl;
            return 1;
        }
        
        const auto& marketData = portfolio[ticker];
        std::map<std::string, MarketData> marketMap;
        for(const auto& data : marketData){
            marketMap[data.date] = data;
        }

        TradingSimulator simulator(initialBalance, marketData);

        std::string choice;
        std::string date;
        double amount;

        while(true){
            std::cout << "1. Buy\n2. Sell\n3. Show Wallet and Statistics\n4. Run automated strategy\n5. Save Report\n6. Exit\n";
            std::getline(std::cin, choice);

            if(std::regex_match(choice, numberRegex)){
                switch (stoi(choice)) {
                    case 1:
                        std::cout << "Enter the figure to buy : ";
                        std::cin >> amount;
                        std::cin.ignore();

                        std::cout << "Insert the date (YYYY-MM-DD): ";
                        std::getline(std::cin, date);

                        if(marketMap.find(date) != marketMap.end()) {
                            const auto& data = marketMap[date];
                            simulator.buy(ticker, amount, data);    
                        } else {
                            std::cout << "The date wasn't found.\n";
                        }
                        break;
                    case 2:
                        std::cout << "Enter the figure to sell : ";
                        std::cin >> amount;
                        std::cin.ignore();

                        std::cout << "Insert the date (YYYY-MM-DD): ";
                        std::getline(std::cin, date);

                        if(marketMap.find(date) != marketMap.end()) {
                            const auto& data = marketMap[date];
                            simulator.sell(ticker, amount, data);
                        }
                        break;
                    case 3:
                        simulator.displayStatistics(initialBalance);
                        break;
                    case 4:
                        int strategyChoice;
                        std::cout << "\nChoose strategy:\n";
                        std::cout << "1. Simple Moving Average (SMA)\n";
                        std::cout << "2. Relative Strength Index (RSI)\n";
                        std::cin >> strategyChoice;
                        std::cin.ignore();

                        if(strategyChoice == 1){
                            size_t period;
                            std::cout << "Enter the period for the moving average (e.g. 5 days): ";
                            std::cin >> period;
                            std::cin.ignore();

                            simulator.executeAveragePeriod(marketData, period);
                            simulator.displayStatistics(initialBalance);
                        } else if(strategyChoice == 2){
                            size_t period;
                            std::cout << "Enter the period for the RSI (e.g. 5 days): ";
                            std::cin >> period;
                            std::cin.ignore();

                            simulator.executeRSI(marketData, period);
                            simulator.displayStatistics(initialBalance);
                        } else {
                            std::cout << "Invalid choice.\n\n";
                        }
                        break;
                    case 5:
                        simulator.saveReport("data/trading_report.csv");
                        break;
                    case 6:
                        simulator.saveReport("data/trading_report.csv");
                        std::cout << "Exiting...\n";
                        return 0;
                }
            } else {
                std::cout << "Invalid input. Please enter a number between 1 and 6." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}