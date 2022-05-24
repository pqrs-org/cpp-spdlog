#pragma once

// (C) Copyright Takayama Fumihiko 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#include <optional>
#include <spdlog/common.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <sstream>

namespace pqrs {
namespace spdlog {
inline const char* get_pattern(void) {
  return "[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] %v";
}

inline std::optional<uint64_t> find_date_number(const std::string& line) {
  // line == "[2016-09-22 20:18:37.649] [info] [<name>] <message>"
  // return 20160922201837649

  if (line.size() < strlen("[0000-00-00 00:00:00.000]")) {
    return std::nullopt;
  }

  if (line.empty()) {
    return std::nullopt;
  }

  std::stringstream ss;
  size_t pos = 0;

  if (line[pos++] != '[') {
    return std::nullopt;
  }

  // years

  ss << line[pos++];
  ss << line[pos++];
  ss << line[pos++];
  ss << line[pos++];

  if (line[pos++] != '-') {
    return std::nullopt;
  }

  // months

  ss << line[pos++];
  ss << line[pos++];

  if (line[pos++] != '-') {
    return std::nullopt;
  }

  // days

  ss << line[pos++];
  ss << line[pos++];

  if (line[pos++] != ' ') {
    return std::nullopt;
  }

  // hours

  ss << line[pos++];
  ss << line[pos++];

  if (line[pos++] != ':') {
    return std::nullopt;
  }

  // minutes

  ss << line[pos++];
  ss << line[pos++];

  if (line[pos++] != ':') {
    return std::nullopt;
  }

  // seconds

  ss << line[pos++];
  ss << line[pos++];

  if (line[pos++] != '.') {
    return std::nullopt;
  }

  // milliseconds

  ss << line[pos++];
  ss << line[pos++];
  ss << line[pos++];

  if (line[pos++] != ']') {
    return std::nullopt;
  }

  try {
    return std::stoll(ss.str());
  } catch (...) {
  }

  return std::nullopt;
}

inline std::optional<uint64_t> make_sort_key(const std::string& line) {
  return find_date_number(line);
}

inline std::optional<::spdlog::level::level_enum> find_level(const std::string& line) {
  auto front = strlen("[0000-00-00 00:00:00.000] [");
  if (line.size() <= front) {
    return std::nullopt;
  }

  for (int i = 0; i < ::spdlog::level::off; ++i) {
    auto level = ::spdlog::level::level_enum(i);
    auto level_name = ::spdlog::level::to_string_view(level);

    std::string level_name_string(level_name.data(), level_name.size());
    level_name_string += "]";

    if (line.compare(front, level_name_string.size(), level_name_string) == 0) {
      return level;
    }
  }

  return std::nullopt;
}

inline ::spdlog::filename_t make_rotated_file_path(::spdlog::filename_t file_path) {
  return ::spdlog::sinks::rotating_file_sink<std::mutex>::calc_filename(file_path, 1);
}
} // namespace spdlog
} // namespace pqrs
