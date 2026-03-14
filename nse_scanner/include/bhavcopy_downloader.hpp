#pragma once

#include <string>

class BhavcopyDownloader {
public:
    explicit BhavcopyDownloader(std::string download_root = "data");

    // date format: YYYY-MM-DD
    bool download_for_date(const std::string& iso_date) const;

private:
    std::string build_nse_url(const std::string& iso_date) const;
    std::string build_zip_name(const std::string& iso_date) const;
    std::string build_csv_name(const std::string& iso_date) const;

    std::string download_root_;
};
