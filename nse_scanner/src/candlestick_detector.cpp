#include "candlestick_detector.hpp"

#include <algorithm>
#include <cmath>

CandlestickPattern CandlestickDetector::detect_latest(const std::vector<Candle>& series) {
    if (series.empty()) {
        return CandlestickPattern::None;
    }

    const Candle& c = series.back();
    if (is_hammer(c)) {
        return CandlestickPattern::Hammer;
    }

    if (series.size() >= 2 && is_bullish_engulfing(series[series.size() - 2], c)) {
        return CandlestickPattern::BullishEngulfing;
    }

    if (series.size() >= 3 && is_morning_star(series[series.size() - 3], series[series.size() - 2], c)) {
        return CandlestickPattern::MorningStar;
    }

    return CandlestickPattern::None;
}

std::string CandlestickDetector::to_string(CandlestickPattern pattern) {
    switch (pattern) {
    case CandlestickPattern::Hammer:
        return "Hammer";
    case CandlestickPattern::BullishEngulfing:
        return "BullishEngulfing";
    case CandlestickPattern::MorningStar:
        return "MorningStar";
    default:
        return "None";
    }
}

bool CandlestickDetector::is_hammer(const Candle& c) {
    const double body = std::max(0.0001, std::abs(c.close - c.open));
    const double full_range = std::max(0.0001, c.high - c.low);
    const double upper_shadow = c.high - std::max(c.open, c.close);
    const double lower_shadow = std::min(c.open, c.close) - c.low;

    return lower_shadow >= 2.0 * body && upper_shadow <= body && (body / full_range) <= 0.4;
}

bool CandlestickDetector::is_bullish_engulfing(const Candle& previous, const Candle& current) {
    const bool previous_bearish = previous.close < previous.open;
    const bool current_bullish = current.close > current.open;
    const bool engulf_body = current.open <= previous.close && current.close >= previous.open;

    return previous_bearish && current_bullish && engulf_body;
}

bool CandlestickDetector::is_morning_star(const Candle& c1, const Candle& c2, const Candle& c3) {
    const bool first_bearish = c1.close < c1.open;
    const double c1_body = std::max(0.0001, std::abs(c1.close - c1.open));

    const double c2_body = std::abs(c2.close - c2.open);
    const bool second_small_body = c2_body <= (0.4 * c1_body);

    const bool third_bullish = c3.close > c3.open;
    const double midpoint_first = (c1.open + c1.close) / 2.0;
    const bool recovery = c3.close >= midpoint_first;

    return first_bearish && second_small_body && third_bullish && recovery;
}
