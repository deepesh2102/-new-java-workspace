#!/usr/bin/env python3
"""Download and unzip NSE bhavcopy for a given ISO date."""

from __future__ import annotations

import argparse
import datetime as dt
import pathlib
import zipfile

import requests


def build_zip_name(day: dt.date) -> str:
    return f"cm{day.strftime('%d%b%Y').upper()}bhav.csv.zip"


def build_url(day: dt.date) -> str:
    month = day.strftime("%b").upper()
    zip_name = build_zip_name(day)
    return f"https://archives.nseindia.com/content/historical/EQUITIES/{day.year}/{month}/{zip_name}"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("date", help="ISO date (YYYY-MM-DD)")
    parser.add_argument("--out", default="data", help="output directory")
    args = parser.parse_args()

    day = dt.date.fromisoformat(args.date)
    out_dir = pathlib.Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    url = build_url(day)
    zip_path = out_dir / build_zip_name(day)

    response = requests.get(url, timeout=30)
    response.raise_for_status()
    zip_path.write_bytes(response.content)

    with zipfile.ZipFile(zip_path, "r") as zf:
        zf.extractall(out_dir)

    print(f"Downloaded: {zip_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
