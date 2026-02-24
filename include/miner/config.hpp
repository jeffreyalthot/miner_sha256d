#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace miner::core {

struct PoolProfile {
  std::string name;
  std::string host;
  std::uint16_t port{3336};
  bool tls{false};
  std::string user;
  std::string password;
};

struct PerfConfig {
  std::size_t cpuThreads{1};
  std::size_t batchSize{1024};
  std::size_t queueDepth{4096};
  int readTimeoutMs{5000};
  int writeTimeoutMs{5000};
};

struct SV2Config {
  std::string mode{"cleartext"};
  std::string deviceId{"cpu-demo"};
  bool workSelection{false};
  bool versionRolling{false};
};

struct AppConfig {
  std::vector<PoolProfile> pools;
  std::size_t activePoolIndex{0};
  PerfConfig perf;
  SV2Config sv2;
  std::string logFile{"miner.log"};
  std::string logLevel{"info"};
};

AppConfig loadConfig(const std::string& path, const std::vector<std::string>& overrides);

} // namespace miner::core
