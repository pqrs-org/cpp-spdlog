#include <catch2/catch.hpp>

#include <pqrs/spdlog.hpp>

TEST_CASE("make_stdout_logger_mt") {
  REQUIRE(pqrs::spdlog::factory::make_stdout_logger_mt("make_stdout_logger_mt_example"));
}

TEST_CASE("make_async_rotating_logger_mt") {
  REQUIRE(pqrs::spdlog::factory::make_async_rotating_logger_mt(
      "make_async_rotating_logger_mt_example_1",
      "target/make_async_rotating_logger_mt.log",
      std::filesystem::perms::owner_all));

  std::error_code error_code;
  std::filesystem::create_directories("target/d000", error_code);
  REQUIRE(!error_code);
  std::filesystem::permissions("target/d000", std::filesystem::perms::none);

  REQUIRE(!pqrs::spdlog::factory::make_async_rotating_logger_mt(
      "make_async_rotating_logger_mt_example_2",
      "target/d000/log/make_async_rotating_logger_mt.log",
      std::filesystem::perms::none));

  std::filesystem::permissions("target/d000", std::filesystem::perms::owner_all);
  std::filesystem::remove_all("target/d000");
}
