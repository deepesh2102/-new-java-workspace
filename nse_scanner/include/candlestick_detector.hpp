#pragma once

#include "candle.hpp"

#include <string>
#include <vector>

enum class CandlestickPattern {
    None,
    Hammer,
    BullishEngulfing,
    MorningStar
};

class CandlestickDetector {
public:
    static CandlestickPattern detect_latest(const std::vector<Candle>& series);
    static std::string to_string(CandlestickPattern pattern);

private:
    static bool is_hammer(const Candle& c);
    static bool is_bullish_engulfing(const Candle& previous, const Candle& current);
    static bool is_morning_star(const Candle& c1, const Candle& c2, const Candle& c3);
};
