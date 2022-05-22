#include <boost/ut.hpp>
#include <pqrs/spdlog.hpp>

void run_factory_test() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "make_stdout_logger_mt"_test = [] {
    expect(pqrs::spdlog::factory::make_stdout_logger_mt("make_stdout_logger_mt_example").get() != nullptr);
  };

  "make_async_rotating_logger_mt"_test = [] {
    expect(pqrs::spdlog::factory::make_async_rotating_logger_mt(
               "make_async_rotating_logger_mt_example_1",
               "target/make_async_rotating_logger_mt.log",
               pqrs::spdlog::filesystem::log_directory_perms_0700)
               .get() != nullptr);

    std::error_code error_code;
    std::filesystem::create_directories("target/d000", error_code);
    expect(!error_code);
    std::filesystem::permissions("target/d000", std::filesystem::perms::none);

    expect(!pqrs::spdlog::factory::make_async_rotating_logger_mt(
        "make_async_rotating_logger_mt_example_2",
        "target/d000/log/make_async_rotating_logger_mt.log",
        std::filesystem::perms::none));

    std::filesystem::permissions("target/d000", std::filesystem::perms::owner_all);
    std::filesystem::remove_all("target/d000");
  };
}
