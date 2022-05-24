#include <boost/ut.hpp>
#include <pqrs/spdlog.hpp>

#include "factory_test.hpp"
#include "monitor_test.hpp"
#include "read_log_files_test.hpp"
#include "unique_filter_test.hpp"

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "make_sort_key"_test = [] {
    expect(pqrs::spdlog::find_date_number("[2016-10-15 00:09:47.283] [info] [<name>] <message>") == 20161015000947283ULL);
    expect(pqrs::spdlog::find_date_number("[]") == std::nullopt);
    expect(pqrs::spdlog::find_date_number("[yyyy-mm-dd hh:mm:ss.mmm]") == std::nullopt);
    expect(pqrs::spdlog::find_date_number("[2016-10-15 00:09:47.283]") == 20161015000947283ULL);
    expect(pqrs::spdlog::find_date_number("[2016-10-15 00:09:47.283") == std::nullopt);
    expect(pqrs::spdlog::find_date_number("[2016 10-15 00:09:47.283]") == std::nullopt);
    expect(pqrs::spdlog::find_date_number("[2016-10-15 00:09:47.283 ") == std::nullopt);
    expect(pqrs::spdlog::find_date_number("[2016-mm-15 00:09:47.283 ") == std::nullopt);
  };

  "find_level"_test = [] {
    expect(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [info] [<name>]") == spdlog::level::info);
    expect(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [error] [<name>]") == spdlog::level::err);
    expect(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [unknown] [<name>]") == std::nullopt);
    expect(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] ") == std::nullopt);
    expect(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [") == std::nullopt);
    expect(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [info") == std::nullopt);
    expect(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [info]") == spdlog::level::info);
  };

  "make_rotated_file_path"_test = [] {
    expect(pqrs::spdlog::make_rotated_file_path("log/spdlog.log") == "log/spdlog.1.log");
    expect(pqrs::spdlog::make_rotated_file_path("/var/log/spdlog.log") == "/var/log/spdlog.1.log");
  };

  run_factory_test();
  run_monitor_test();
  run_read_log_files_test();
  run_unique_filter_test();

  return 0;
}
