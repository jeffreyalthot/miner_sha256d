#include "miner/logging.hpp"
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace miner::core {

Logger::Logger(const std::string& filePath, LogLevel level, bool jsonFormat, std::size_t rotateBytes)
    : filePath_(filePath), level_(level), jsonFormat_(jsonFormat), rotateBytes_(rotateBytes) {
  out_.open(filePath_, std::ios::app);
}

void Logger::rotateIfNeeded() {
  if (!std::filesystem::exists(filePath_)) return;
  if (std::filesystem::file_size(filePath_) < rotateBytes_) return;
  out_.close();
  std::filesystem::rename(filePath_, filePath_ + ".1");
  out_.open(filePath_, std::ios::trunc);
}

std::string Logger::format(LogLevel level, const std::string& message, const std::string& component) const {
  const char* lvl = (level == LogLevel::Debug) ? "DEBUG" : (level == LogLevel::Info) ? "INFO" : (level == LogLevel::Warn) ? "WARN" : "ERROR";
  auto now = std::chrono::system_clock::now();
  auto tt = std::chrono::system_clock::to_time_t(now);
  std::ostringstream os;
  if (jsonFormat_) {
    os << "{\"ts\":\"" << std::put_time(std::gmtime(&tt), "%FT%TZ") << "\",\"lvl\":\"" << lvl
       << "\",\"component\":\"" << component << "\",\"msg\":\"" << message << "\"}";
  } else {
    os << "[" << lvl << "] " << component << ": " << message;
  }
  return os.str();
}

void Logger::log(LogLevel level, const std::string& message, const std::string& component) {
  if (static_cast<int>(level) < static_cast<int>(level_)) return;
  std::scoped_lock lock(mu_);
  rotateIfNeeded();
  auto line = format(level, message, component);
  out_ << line << '\n';
  out_.flush();
}

LogLevel parseLogLevel(const std::string& level) {
  if (level == "debug") return LogLevel::Debug;
  if (level == "warn") return LogLevel::Warn;
  if (level == "error") return LogLevel::Error;
  return LogLevel::Info;
}

} // namespace miner::core
