#include <catch2/catch.hpp>

#include <pqrs/spdlog.hpp>

class monitor_test final {
public:
  monitor_test(void) {
    time_source_ = std::make_shared<pqrs::dispatcher::hardware_time_source>();
    dispatcher_ = std::make_shared<pqrs::dispatcher::dispatcher>(time_source_);

    pqrs::filesystem::create_directory_with_intermediate_directories("target", 0755);
    unlink("target/foo.log");
    unlink("target/foo.1.log");
    unlink("target/bar.log");
    unlink("target/bar.1.log");

    std::vector<::spdlog::filename_t> target_file_paths{
        "target/foo.log",
        "target/bar.log",
    };

    monitor_ = std::make_shared<pqrs::spdlog::monitor>(dispatcher_,
                                                       target_file_paths,
                                                       0);
    monitor_->log_file_updated.connect([this](auto&& lines) {
      std::lock_guard<std::mutex> lock(lines_mutex_);

      lines_ = lines;
    });

    monitor_->async_start(std::chrono::milliseconds(100));
  }

  ~monitor_test(void) {
    monitor_ = nullptr;

    dispatcher_->terminate();
    dispatcher_ = nullptr;
  }

  std::shared_ptr<std::deque<std::string>> get_lines(void) {
    std::lock_guard<std::mutex> lock(lines_mutex_);

    return lines_;
  }

  void wait(void) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

private:
  std::shared_ptr<pqrs::dispatcher::hardware_time_source> time_source_;
  std::shared_ptr<pqrs::dispatcher::dispatcher> dispatcher_;
  std::shared_ptr<pqrs::spdlog::monitor> monitor_;
  std::shared_ptr<std::deque<std::string>> lines_;
  mutable std::mutex lines_mutex_;
};

TEST_CASE("monitor") {
  monitor_test monitor_test;

  system("echo '[2018-02-01 22:46:33.670] [info] [foo] message 1' >> target/foo.log");
  monitor_test.wait();

  {
    auto lines = monitor_test.get_lines();
    REQUIRE(lines);
    REQUIRE(lines->size() == 1);
    REQUIRE((*lines)[0] == "[2018-02-01 22:46:33.670] [info] [foo] message 1");
  }

  system("echo '[2018-02-01 22:46:33.671] [info] [bar] message 1' >> target/bar.log");
  monitor_test.wait();

  {
    auto lines = monitor_test.get_lines();
    REQUIRE(lines);
    REQUIRE(lines->size() == 2);
    REQUIRE((*lines)[0] == "[2018-02-01 22:46:33.670] [info] [foo] message 1");
    REQUIRE((*lines)[1] == "[2018-02-01 22:46:33.671] [info] [bar] message 1");
  }

  system("echo '[2018-02-01 22:46:33.678] [info] [foo] message 5' >> target/foo.log");
  monitor_test.wait();

  {
    auto lines = monitor_test.get_lines();
    REQUIRE(lines);
    REQUIRE(lines->size() == 3);
    REQUIRE((*lines)[0] == "[2018-02-01 22:46:33.670] [info] [foo] message 1");
    REQUIRE((*lines)[1] == "[2018-02-01 22:46:33.671] [info] [bar] message 1");
    REQUIRE((*lines)[2] == "[2018-02-01 22:46:33.678] [info] [foo] message 5");
  }

  // rotate

  system("mv target/foo.log target/foo.1.log");
  system("echo '[2018-02-01 22:46:33.707] [info] [foo] message 8' >> target/foo.log");
  monitor_test.wait();

  {
    auto lines = monitor_test.get_lines();
    REQUIRE(lines);
    REQUIRE(lines->size() == 4);
    REQUIRE((*lines)[0] == "[2018-02-01 22:46:33.670] [info] [foo] message 1");
    REQUIRE((*lines)[1] == "[2018-02-01 22:46:33.671] [info] [bar] message 1");
    REQUIRE((*lines)[2] == "[2018-02-01 22:46:33.678] [info] [foo] message 5");
    REQUIRE((*lines)[3] == "[2018-02-01 22:46:33.707] [info] [foo] message 8");
  }
}
