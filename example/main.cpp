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

  dispatcher->terminate();
  dispatcher = nullptr;

  return 0;
}
