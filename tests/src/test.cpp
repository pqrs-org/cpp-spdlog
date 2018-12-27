#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/spdlog.hpp>

TEST_CASE("get_timestamp_number") {
  {
    auto actual = pqrs::spdlog::get_sort_key("[2016-10-15 00:09:47.283] [info] [grabber] version 0.90.50");
    REQUIRE(actual == 20161015000947283ULL);
  }
  {
    auto actual = pqrs::spdlog::get_sort_key("[]");
    REQUIRE(actual == std::nullopt);
  }
  {
    auto actual = pqrs::spdlog::get_sort_key("[yyyy-mm-dd hh:mm:ss.mmm]");
    REQUIRE(actual == std::nullopt);
  }
}

TEST_CASE("get_level") {
  {
    auto actual = pqrs::spdlog::get_level("[2016-10-15 00:09:47.283] [info] [grabber] version 0.90.50");
    REQUIRE(actual == spdlog::level::info);
  }
  {
    auto actual = pqrs::spdlog::get_level("[2016-10-15 00:09:47.283] [error] [grabber] version 0.90.50");
    REQUIRE(actual == spdlog::level::err);
  }
  {
    auto actual = pqrs::spdlog::get_level("[2016-10-15 00:09:47.283] [unknown] [grabber] version 0.90.50");
    REQUIRE(actual == std::nullopt);
  }
  {
    auto actual = pqrs::spdlog::get_level("[2016-10-15 00:09:47.283] ");
    REQUIRE(actual == std::nullopt);
  }
  {
    auto actual = pqrs::spdlog::get_level("[2016-10-15 00:09:47.283] [");
    REQUIRE(actual == std::nullopt);
  }
  {
    auto actual = pqrs::spdlog::get_level("[2016-10-15 00:09:47.283] [info");
    REQUIRE(actual == std::nullopt);
  }
  {
    auto actual = pqrs::spdlog::get_level("[2016-10-15 00:09:47.283] [info]");
    REQUIRE(actual == spdlog::level::info);
  }
}
