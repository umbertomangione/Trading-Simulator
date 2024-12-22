#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "MarketData.h"

class DataLoader {
public:
    static std::unordered_map<std::string, std::vector<MarketData>> loadCSV(const std::string& filename);
};

#endif