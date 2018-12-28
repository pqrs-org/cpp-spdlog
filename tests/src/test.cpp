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

namespace {
void test_read_log_files(std::shared_ptr<std::deque<std::string>> actual,
                         const std::string expected_file_path) {
  std::deque<std::string> expected;
  std::ifstream stream(expected_file_path);
  std::string line;
  while (std::getline(stream, line)) {
    if (!line.empty()) {
      expected.push_back(line);
    }
  }

  REQUIRE(actual);
  REQUIRE(actual->size() == expected.size());
  for (size_t i = 0; i < actual->size(); ++i) {
    REQUIRE((*actual)[i] == expected[i]);
  }
}
} // namespace

TEST_CASE("read_log_files") {
  // normal

  {
    std::vector<spdlog::filename_t> target_file_paths{
        "data/log/foo.log",
        "data/log/bar.log",
        "data/log/empty.log",
    };
    auto actual = pqrs::spdlog::read_log_files(target_file_paths, 0);

    test_read_log_files(actual, "data/expected/full.log");
  }

  // target_file_paths is empty

  {
    std::vector<spdlog::filename_t> target_file_paths{};
    auto actual = pqrs::spdlog::read_log_files(target_file_paths, 0);

    test_read_log_files(actual, "data/expected/empty.log");
  }

  // target_file_paths does not include a valid file path.

  {
    std::vector<spdlog::filename_t> target_file_paths{
        "data/log/not_found.log",
    };
    auto actual = pqrs::spdlog::read_log_files(target_file_paths, 0);

    test_read_log_files(actual, "data/expected/empty.log");
  }

  // target_file_paths contains only empty log.

  {
    std::vector<spdlog::filename_t> target_file_paths{
        "data/log/empty.log",
    };
    auto actual = pqrs::spdlog::read_log_files(target_file_paths, 0);

    test_read_log_files(actual, "data/expected/empty.log");
  }

  // max_line_count

  {
    std::vector<spdlog::filename_t> target_file_paths{
        "data/log/foo.log",
        "data/log/bar.log",
        "data/log/empty.log",
    };
    auto actual = pqrs::spdlog::read_log_files(target_file_paths, 5);

    test_read_log_files(actual, "data/expected/max_line_count.log");
  }
}
