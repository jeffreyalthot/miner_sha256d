#pragma once
#include <fstream>
#include <mutex>
#include <string>

namespace miner::core {

enum class LogLevel { Debug, Info, Warn, Error };

class Logger {
public:
  Logger(const std::string& filePath, LogLevel level, bool jsonFormat = true, std::size_t rotateBytes = 1 << 20);
  void log(LogLevel level, const std::string& message, const std::string& component = "app");

private:
  void rotateIfNeeded();
  std::string format(LogLevel level, const std::string& message, const std::string& component) const;

  std::ofstream out_;
  std::string filePath_;
  LogLevel level_;
  bool jsonFormat_;
  std::size_t rotateBytes_;
  std::mutex mu_;
};

LogLevel parseLogLevel(const std::string& level);
} // namespace miner::core
