#include <catch2/catch.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <pqrs/spdlog.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <sstream>

TEST_CASE("unique_filter") {
  std::error_code error_code;
  std::filesystem::create_directories("target", error_code);
  REQUIRE(!error_code);

  unlink("target/unique_filter_test.log");

  auto logger = spdlog::basic_logger_mt("unique_filter_test", "target/unique_filter_test.log");

  pqrs::spdlog::unique_filter unique_filter(logger, 16);

  // Reduce

  unique_filter.info("test1");
  unique_filter.info("test1");
  unique_filter.info("test1");
  unique_filter.warn("test1");
  unique_filter.warn("test1");
  unique_filter.warn("test1");
  unique_filter.error("test1");
  unique_filter.error("test1");
  unique_filter.error("test1");
  unique_filter.info("test1");
  unique_filter.warn("test1");
  unique_filter.error("test1");

  // Reset

  unique_filter.info("test2");
  unique_filter.reset();
  unique_filter.info("test2");
  unique_filter.reset();
  unique_filter.info("test2");

  // Old values will be removed after new items are pushed.

  unique_filter.info("test3");
  for (int i = 0; i < 100; ++i) {
    std::stringstream ss;
    ss << "dummy " << i;
    unique_filter.info(ss.str());
  }
  unique_filter.info("test3");

  logger->flush();

  // ========================================
  // Test

  std::deque<std::string> lines;
  {
    std::ifstream stream("target/unique_filter_test.log");
    REQUIRE(stream);
    std::string line;
    while (std::getline(stream, line)) {
      lines.push_back(line);
      std::cout << line << std::endl;
    }
  }

  // info test1 == 1

  {
    auto count = std::count_if(std::begin(lines),
                               std::end(lines),
                               [](auto&& l) {
                                 return l.find("test1") != std::string::npos &&
                                        l.find("info") != std::string::npos;
                               });
    REQUIRE(count == 1);
  }

  // warn test1 == 1

  {
    auto count = std::count_if(std::begin(lines),
                               std::end(lines),
                               [](auto&& l) {
                                 return l.find("test1") != std::string::npos &&
                                        l.find("warn") != std::string::npos;
                               });
    REQUIRE(count == 1);
  }

  // error test1 == 1

  {
    auto count = std::count_if(std::begin(lines),
                               std::end(lines),
                               [](auto&& l) {
                                 return l.find("test1") != std::string::npos &&
                                        l.find("error") != std::string::npos;
                               });
    REQUIRE(count == 1);
  }

  // test2 == 3

  {
    auto count = std::count_if(std::begin(lines),
                               std::end(lines),
                               [](auto&& l) {
                                 return l.find("test2") != std::string::npos;
                               });
    REQUIRE(count == 3);
  }

  // test3 == 2

  {
    auto count = std::count_if(std::begin(lines),
                               std::end(lines),
                               [](auto&& l) {
                                 return l.find("test3") != std::string::npos;
                               });
    REQUIRE(count == 2);
  }
}
