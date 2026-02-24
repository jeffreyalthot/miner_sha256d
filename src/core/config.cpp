#include "miner/config.hpp"
#include <fstream>
#include <regex>
#include <sstream>

namespace miner::core {
namespace {
std::string slurp(const std::string& path) {
  std::ifstream in(path);
  std::ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
}
std::string findString(const std::string& src, const std::string& key, const std::string& def) {
  std::regex r("\"" + key + "\"\\s*:\\s*\"([^\"]+)\"");
  std::smatch m;
  return std::regex_search(src, m, r) ? m[1].str() : def;
}
std::size_t findUInt(const std::string& src, const std::string& key, std::size_t def) {
  std::regex r("\"" + key + "\"\\s*:\\s*([0-9]+)");
  std::smatch m;
  return std::regex_search(src, m, r) ? std::stoull(m[1].str()) : def;
}
bool findBool(const std::string& src, const std::string& key, bool def) {
  std::regex r("\"" + key + "\"\\s*:\\s*(true|false)");
  std::smatch m;
  if (!std::regex_search(src, m, r)) return def;
  return m[1].str() == "true";
}
void applyOverride(AppConfig& cfg, const std::string& kv) {
  auto pos = kv.find('=');
  if (pos == std::string::npos) return;
  auto key = kv.substr(0, pos);
  auto val = kv.substr(pos + 1);
  if (key == "perf.cpu_threads") cfg.perf.cpuThreads = std::stoul(val);
  else if (key == "perf.batch_size") cfg.perf.batchSize = std::stoul(val);
  else if (key == "active_pool") cfg.activePoolIndex = std::stoul(val);
  else if (key == "pool.name") cfg.pools[0].name = val;
  else if (key == "pool.host") cfg.pools[0].host = val;
  else if (key == "pool.port") cfg.pools[0].port = static_cast<std::uint16_t>(std::stoul(val));
  else if (key == "pool.user") cfg.pools[0].user = val;
  else if (key == "pool.password") cfg.pools[0].password = val;
  else if (key == "pool.tls") cfg.pools[0].tls = (val == "1" || val == "true" || val == "on");
  else if (key == "sv2.mode") cfg.sv2.mode = val;
  else if (key == "sv2.device_id") cfg.sv2.deviceId = val;
  else if (key == "log.level") cfg.logLevel = val;
}
} // namespace

AppConfig loadConfig(const std::string& path, const std::vector<std::string>& overrides) {
  auto raw = slurp(path);
  AppConfig cfg;
  cfg.logFile = findString(raw, "log_file", cfg.logFile);
  cfg.logLevel = findString(raw, "log_level", cfg.logLevel);
  cfg.activePoolIndex = findUInt(raw, "active_pool", cfg.activePoolIndex);

  cfg.perf.cpuThreads = findUInt(raw, "cpu_threads", cfg.perf.cpuThreads);
  cfg.perf.batchSize = findUInt(raw, "batch_size", cfg.perf.batchSize);
  cfg.perf.queueDepth = findUInt(raw, "queue_depth", cfg.perf.queueDepth);

  cfg.sv2.mode = findString(raw, "mode", cfg.sv2.mode);
  cfg.sv2.deviceId = findString(raw, "device_id", cfg.sv2.deviceId);
  cfg.sv2.workSelection = findBool(raw, "work_selection", cfg.sv2.workSelection);
  cfg.sv2.versionRolling = findBool(raw, "version_rolling", cfg.sv2.versionRolling);

  PoolProfile p;
  p.name = findString(raw, "name", "local");
  p.host = findString(raw, "host", "127.0.0.1");
  p.port = static_cast<std::uint16_t>(findUInt(raw, "port", 3336));
  p.tls = findBool(raw, "tls", false);
  p.user = findString(raw, "user", "worker1");
  p.password = findString(raw, "password", "x");
  cfg.pools.push_back(p);

  for (const auto& ov : overrides) applyOverride(cfg, ov);
  return cfg;
}

} // namespace miner::core
