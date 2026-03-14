# NSE Scanner

C++/Python hybrid scanner for NSE bhavcopy based bullish opportunities.

## Implemented modules

- **NSE bhavcopy download**
  - C++: `BhavcopyDownloader` (`--download YYYY-MM-DD`)
  - Python: `python/bhavcopy_download.py`
- **Candlestick pattern detection**
  - Hammer
  - Bullish Engulfing
  - Morning Star
- **Bullish scanner**
  - Filters for bullish candlestick pattern
  - Requires close > SMA20
  - Requires volume >= 1.5x 20-day average volume

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/nse_scanner_app --download 2024-12-20
./build/nse_scanner_app --scan data/cm20DEC2024bhav.csv
```

Generate markdown report from scanner output:

```bash
./build/nse_scanner_app --scan data/cm20DEC2024bhav.csv > reports/latest_scan.txt
python3 python/report_bullish.py reports/latest_scan.txt
```
