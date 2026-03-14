#include "bhavcopy_downloader.hpp"
#include "bullish_scanner.hpp"
#include "candlestick_detector.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace {

std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> cells;
    std::stringstream ss(line);
    std::string part;

    while (std::getline(ss, part, ',')) {
        cells.push_back(part);
    }

    return cells;
}

std::unordered_map<std::string, std::vector<Candle>> load_bhavcopy(const std::string& csv_path) {
    std::unordered_map<std::string, std::vector<Candle>> by_symbol;

    std::ifstream file(csv_path);
    if (!file) {
        throw std::runtime_error("Unable to open CSV: " + csv_path);
    }

    std::string line;
    if (!std::getline(file, line)) {
        return by_symbol;
    }

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        const auto cells = split_csv_line(line);
        if (cells.size() < 7) {
            continue;
        }

        try {
            Candle candle;
            candle.symbol = cells[0];
            candle.open = std::stod(cells[2]);
            candle.high = std::stod(cells[3]);
            candle.low = std::stod(cells[4]);
            candle.close = std::stod(cells[5]);
            candle.volume = std::stod(cells[6]);
            candle.date = "NA";

            by_symbol[candle.symbol].push_back(candle);
        } catch (...) {
            continue;
        }
    }

    return by_symbol;
}

void print_usage(const char* app) {
    std::cout << "Usage:\n"
              << "  " << app << " --download YYYY-MM-DD\n"
              << "  " << app << " --scan <bhavcopy_csv_path>\n";
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    const std::string command = argv[1];
    const std::string arg = argv[2];

    if (command == "--download") {
        BhavcopyDownloader downloader("data");
        return downloader.download_for_date(arg) ? 0 : 2;
    }

    if (command == "--scan") {
        const auto by_symbol = load_bhavcopy(arg);

        BullishScanner scanner;
        const auto signals = scanner.scan(by_symbol);

        std::cout << "Bullish signals found: " << signals.size() << '\n';
        for (const auto& s : signals) {
            std::cout << s.symbol << ','
                      << s.date << ','
                      << CandlestickDetector::to_string(s.pattern) << ','
                      << s.close << ','
                      << s.sma20 << ','
                      << s.volume << ','
                      << s.avg_volume20 << '\n';
        }

        return 0;
    }

    print_usage(argv[0]);
    return 1;
}
