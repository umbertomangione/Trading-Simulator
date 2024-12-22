#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::unordered_map<std::string, std::vector<MarketData>> DataLoader::loadCSV(const std::string& filename){
    std::unordered_map<std::string, std::vector<MarketData>> portfolio;
    std::ifstream file(filename);

    if(!file.is_open()){
        throw std::runtime_error("Unable to open the file: " + filename);
    }

    std::string line;
    std::getline(file, line);

    while(std::getline(file, line)){
        std::stringstream ss(line);
        MarketData data;
        //std::string temp;

        std::getline(ss, data.ticker, ',');
        std::getline(ss, data.date, ',');
        ss >> data.open;
        ss.ignore(1);
        ss >> data.high;
        ss.ignore(1);
        ss >> data.low;
        ss.ignore(1);
        ss >> data.close;
        ss.ignore(1);
        ss >> data.volume;
        
        /*std::getline(ss, temp, ',');
        data.open = std::stod(temp);
        std::getline(ss, temp, ',');
        data.high = std::stod(temp);
        std::getline(ss, temp, ',');
        data.low = std::stod(temp);
        std::getline(ss, temp, ',');
        data.close = std::stod(temp);
        std::getline(ss, temp, ',');
        data.volume = std::stoi(temp);*/

        portfolio[data.ticker].push_back(data);
    }

    file.close();
    return portfolio;
}