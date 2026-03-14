#pragma once

#include "candle.hpp"
#include "candlestick_detector.hpp"

#include <string>
#include <unordered_map>
#include <vector>

struct BullishSignal {
    std::string symbol;
    std::string date;
    CandlestickPattern pattern {CandlestickPattern::None};
    double close {0.0};
    double sma20 {0.0};
    double volume {0.0};
    double avg_volume20 {0.0};
};

class BullishScanner {
public:
    std::vector<BullishSignal> scan(const std::unordered_map<std::string, std::vector<Candle>>& by_symbol) const;

private:
    static double simple_moving_average(const std::vector<Candle>& candles, std::size_t lookback);
    static double average_volume(const std::vector<Candle>& candles, std::size_t lookback);
};
