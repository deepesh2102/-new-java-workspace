#include "bullish_scanner.hpp"

#include <algorithm>

std::vector<BullishSignal> BullishScanner::scan(const std::unordered_map<std::string, std::vector<Candle>>& by_symbol) const {
    std::vector<BullishSignal> signals;

    for (const auto& [symbol, candles] : by_symbol) {
        if (candles.size() < 20) {
            continue;
        }

        const CandlestickPattern pattern = CandlestickDetector::detect_latest(candles);
        if (pattern == CandlestickPattern::None) {
            continue;
        }

        const Candle& latest = candles.back();
        const double sma20 = simple_moving_average(candles, 20);
        const double avg_volume20 = average_volume(candles, 20);

        const bool price_confirmation = latest.close > sma20;
        const bool volume_confirmation = avg_volume20 > 0.0 && latest.volume >= 1.5 * avg_volume20;

        if (!price_confirmation || !volume_confirmation) {
            continue;
        }

        BullishSignal signal;
        signal.symbol = symbol;
        signal.date = latest.date;
        signal.pattern = pattern;
        signal.close = latest.close;
        signal.sma20 = sma20;
        signal.volume = latest.volume;
        signal.avg_volume20 = avg_volume20;
        signals.push_back(signal);
    }

    std::sort(signals.begin(), signals.end(), [](const BullishSignal& a, const BullishSignal& b) {
        return a.symbol < b.symbol;
    });

    return signals;
}

double BullishScanner::simple_moving_average(const std::vector<Candle>& candles, std::size_t lookback) {
    const std::size_t n = candles.size();
    const std::size_t start = n - std::min(n, lookback);

    double sum = 0.0;
    for (std::size_t i = start; i < n; ++i) {
        sum += candles[i].close;
    }

    return sum / static_cast<double>(n - start);
}

double BullishScanner::average_volume(const std::vector<Candle>& candles, std::size_t lookback) {
    const std::size_t n = candles.size();
    const std::size_t start = n - std::min(n, lookback);

    double sum = 0.0;
    for (std::size_t i = start; i < n; ++i) {
        sum += candles[i].volume;
    }

    return sum / static_cast<double>(n - start);
}
