#!/usr/bin/env python3
"""Convert scanner stdout CSV lines into markdown report."""

from __future__ import annotations

import argparse
import csv
import pathlib


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", help="scanner output file")
    parser.add_argument("--output", default="reports/bullish_report.md")
    args = parser.parse_args()

    rows = []
    with open(args.input, newline="", encoding="utf-8") as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith("Bullish signals found"):
                continue
            rows.append(next(csv.reader([line])))

    output = pathlib.Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)

    with output.open("w", encoding="utf-8") as f:
        f.write("# Bullish Scanner Report\n\n")
        f.write("| Symbol | Date | Pattern | Close | SMA20 | Volume | AvgVolume20 |\n")
        f.write("|---|---|---|---:|---:|---:|---:|\n")
        for row in rows:
            f.write(f"| {' | '.join(row)} |\n")

    print(f"Report generated: {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
