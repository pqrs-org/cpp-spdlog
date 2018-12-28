#include <csignal>
#include <iostream>
#include <pqrs/dispatcher.hpp>
#include <pqrs/spdlog.hpp>

namespace {
auto global_wait = pqrs::make_thread_wait();
}

int main(void) {
  std::signal(SIGINT, [](int) {
    global_wait->notify();
  });

  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  std::vector<spdlog::filename_t> target_file_paths{
      "/var/log/karabiner/grabber.log",
      "/var/log/karabiner/observer.log",
  };
  auto monitor = std::make_shared<pqrs::spdlog::monitor>(dispatcher,
                                                         target_file_paths,
                                                         10);
  monitor->log_file_updated.connect([](auto&& lines) {
    if (lines) {
      for (const auto& l : *lines) {
        std::cout << l << std::endl;
      }

      std::cout << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;
    }
  });
  monitor->async_start(std::chrono::milliseconds(1000));

  // ============================================================

  global_wait->wait_notice();

  // ============================================================

  monitor = nullptr;

  dispatcher->terminate();
  dispatcher = nullptr;

  return 0;
}
