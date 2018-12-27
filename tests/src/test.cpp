#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/spdlog.hpp>

TEST_CASE("make_sort_key") {
  REQUIRE(pqrs::spdlog::make_sort_key("[2016-10-15 00:09:47.283] [info] [<name>] <message>") == 20161015000947283ULL);
  REQUIRE(pqrs::spdlog::make_sort_key("[]") == std::nullopt);
  REQUIRE(pqrs::spdlog::make_sort_key("[yyyy-mm-dd hh:mm:ss.mmm]") == std::nullopt);
  REQUIRE(pqrs::spdlog::make_sort_key("[2016-10-15 00:09:47.283]") == 20161015000947283ULL);
  REQUIRE(pqrs::spdlog::make_sort_key("[2016-10-15 00:09:47.283") == std::nullopt);
  REQUIRE(pqrs::spdlog::make_sort_key("[2016 10-15 00:09:47.283]") == std::nullopt);
  REQUIRE(pqrs::spdlog::make_sort_key("[2016-10-15 00:09:47.283 ") == std::nullopt);
  REQUIRE(pqrs::spdlog::make_sort_key("[2016-mm-15 00:09:47.283 ") == std::nullopt);
}

TEST_CASE("find_level") {
  REQUIRE(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [info] [<name>]") == spdlog::level::info);
  REQUIRE(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [error] [<name>]") == spdlog::level::err);
  REQUIRE(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [unknown] [<name>]") == std::nullopt);
  REQUIRE(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] ") == std::nullopt);
  REQUIRE(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [") == std::nullopt);
  REQUIRE(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [info") == std::nullopt);
  REQUIRE(pqrs::spdlog::find_level("[2016-10-15 00:09:47.283] [info]") == spdlog::level::info);
}

TEST_CASE("make_rotated_file_path") {
  REQUIRE(pqrs::spdlog::make_rotated_file_path("log/spdlog.log") == "log/spdlog.1.log");
  REQUIRE(pqrs::spdlog::make_rotated_file_path("/var/log/spdlog.log") == "/var/log/spdlog.1.log");
}
