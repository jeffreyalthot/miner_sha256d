#include "miner/config.hpp"
#include "miner/logging.hpp"
#include "miner/metrics.hpp"
#include "miner/mining.hpp"
#include "miner/sv2.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

int main(int argc, char** argv) {
  std::string configPath = "config.json";
  std::vector<std::string> overrides;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help") {
      std::cout << "Usage: ./miner [--config=file.json] [--set=k=v] [options pool]\n"
                << "Options pool rapides:\n"
                << "  --pool-name=<name>\n"
                << "  --pool-host=<host>\n"
                << "  --pool-port=<port>\n"
                << "  --pool-user=<wallet.worker>\n"
                << "  --pool-pass=<password>\n"
                << "  --pool-tls=<true|false>\n";
      return 0;
    }
    if (arg.rfind("--config=", 0) == 0) configPath = arg.substr(9);
    else if (arg.rfind("--set=", 0) == 0) overrides.push_back(arg.substr(6));
    else if (arg.rfind("--pool-name=", 0) == 0) overrides.push_back("pool.name=" + arg.substr(12));
    else if (arg.rfind("--pool-host=", 0) == 0) overrides.push_back("pool.host=" + arg.substr(12));
    else if (arg.rfind("--pool-port=", 0) == 0) overrides.push_back("pool.port=" + arg.substr(12));
    else if (arg.rfind("--pool-user=", 0) == 0) overrides.push_back("pool.user=" + arg.substr(12));
    else if (arg.rfind("--pool-pass=", 0) == 0) overrides.push_back("pool.password=" + arg.substr(12));
    else if (arg.rfind("--pool-tls=", 0) == 0) overrides.push_back("pool.tls=" + arg.substr(11));
  }

  auto cfg = miner::core::loadConfig(configPath, overrides);
  miner::core::Logger logger(cfg.logFile, miner::core::parseLogLevel(cfg.logLevel));
  miner::core::MetricsRegistry metrics;

  if (cfg.activePoolIndex >= cfg.pools.size()) cfg.activePoolIndex = 0;
  const auto& pool = cfg.pools[cfg.activePoolIndex];
  miner::sv2::connectAndSetup(pool, cfg.sv2, logger);

  miner::work::Job job;
  job.channelId = 1;
  job.jobId = 1;
  job.header.version = 0x20000000;
  job.header.ntime = static_cast<std::uint32_t>(std::time(nullptr));
  job.header.nbits = 0x207fffff; // easy regtest target

  miner::mining::Scheduler scheduler(cfg.perf, metrics, logger);
  scheduler.start(job);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  scheduler.stop();

  auto snap = metrics.snapshot();
  std::cout << "hashes/s approx: " << snap.instantHashrate << "\n";
  return 0;
}
