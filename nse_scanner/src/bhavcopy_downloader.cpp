#include "bhavcopy_downloader.hpp"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

std::string month_to_abbrev(int month) {
    static constexpr std::array<const char*, 12> months {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
        "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
    };

    if (month < 1 || month > 12) {
        return "UNK";
    }
    return months.at(static_cast<std::size_t>(month - 1));
}

bool parse_iso_date(const std::string& iso_date, int& year, int& month, int& day) {
    if (iso_date.size() != 10 || iso_date.at(4) != '-' || iso_date.at(7) != '-') {
        return false;
    }

    try {
        year = std::stoi(iso_date.substr(0, 4));
        month = std::stoi(iso_date.substr(5, 2));
        day = std::stoi(iso_date.substr(8, 2));
    } catch (...) {
        return false;
    }

    return year > 1990 && month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

} // namespace

BhavcopyDownloader::BhavcopyDownloader(std::string download_root)
    : download_root_(std::move(download_root)) {}

std::string BhavcopyDownloader::build_zip_name(const std::string& iso_date) const {
    int year = 0;
    int month = 0;
    int day = 0;
    parse_iso_date(iso_date, year, month, day);

    std::ostringstream file_name;
    file_name << "cm"
              << std::setw(2) << std::setfill('0') << day
              << month_to_abbrev(month)
              << year
              << "bhav.csv.zip";

    return file_name.str();
}

std::string BhavcopyDownloader::build_csv_name(const std::string& iso_date) const {
    int year = 0;
    int month = 0;
    int day = 0;
    parse_iso_date(iso_date, year, month, day);

    std::ostringstream file_name;
    file_name << "cm"
              << std::setw(2) << std::setfill('0') << day
              << month_to_abbrev(month)
              << year
              << "bhav.csv";

    return file_name.str();
}

std::string BhavcopyDownloader::build_nse_url(const std::string& iso_date) const {
    int year = 0;
    int month = 0;
    int day = 0;
    if (!parse_iso_date(iso_date, year, month, day)) {
        return {};
    }

    const std::string month_abbrev = month_to_abbrev(month);
    const std::string zip_name = build_zip_name(iso_date);

    std::ostringstream url;
    url << "https://archives.nseindia.com/content/historical/EQUITIES/"
        << year << "/" << month_abbrev << "/" << zip_name;
    return url.str();
}

bool BhavcopyDownloader::download_for_date(const std::string& iso_date) const {
    const std::string url = build_nse_url(iso_date);
    if (url.empty()) {
        std::cerr << "Invalid ISO date. Expected YYYY-MM-DD, got: " << iso_date << '\n';
        return false;
    }

    std::filesystem::create_directories(download_root_);
    const std::filesystem::path zip_path = std::filesystem::path(download_root_) / build_zip_name(iso_date);

    const std::string curl_command = "curl -fL --retry 2 -o \"" + zip_path.string() + "\" \"" + url + "\"";
    if (std::system(curl_command.c_str()) != 0) {
        std::cerr << "Failed to download bhavcopy from: " << url << '\n';
        return false;
    }

    const std::string unzip_command = "unzip -o \"" + zip_path.string() + "\" -d \"" + download_root_ + "\"";
    if (std::system(unzip_command.c_str()) != 0) {
        std::cerr << "Failed to unzip bhavcopy: " << zip_path << '\n';
        return false;
    }

    const auto csv_path = std::filesystem::path(download_root_) / build_csv_name(iso_date);
    std::cout << "Bhavcopy ready: " << csv_path << '\n';
    return std::filesystem::exists(csv_path);
}
