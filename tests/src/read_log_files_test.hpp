#include <algorithm>
#include <boost/ut.hpp>
#include <pqrs/spdlog.hpp>

namespace {
void test_read_log_files(pqrs::not_null_shared_ptr_t<std::deque<std::string>> actual,
                         const std::string& expected_file_path) {
  using namespace boost::ut;

  std::deque<std::string> expected;
  std::ifstream stream(expected_file_path);
  std::string line;
  while (std::getline(stream, line)) {
    if (!line.empty()) {
      expected.push_back(line);
    }
  }

  expect(std::ranges::equal(*actual, expected));
}
} // namespace

void run_read_log_files_test() {
  using namespace boost::ut::literals;

  "read_log_files"_test = [] {
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

    // broken line

    {
      std::vector<spdlog::filename_t> target_file_paths{
          "data/log/broken1.log",
          "data/log/broken2.log",
      };
      auto actual = pqrs::spdlog::read_log_files(target_file_paths, 0);

      test_read_log_files(actual, "data/expected/broken.log");
    }

    // corrupted line

    {
      std::vector<spdlog::filename_t> target_file_paths{
          "data/log/corrupted1.log",
      };
      auto actual = pqrs::spdlog::read_log_files(target_file_paths, 0);

      test_read_log_files(actual, "data/expected/corrupted.log");
    }
  };
}
