#include "miner/config.hpp"
#include "miner/logging.hpp"
#include "miner/metrics.hpp"
#include "miner/mining.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char** argv) {
  std::string configPath = "config.json";
  std::vector<std::string> overrides;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg.rfind("--config=", 0) == 0) configPath = arg.substr(9);
    else if (arg.rfind("--set=", 0) == 0) overrides.push_back(arg.substr(6));
  }

  auto cfg = miner::core::loadConfig(configPath, overrides);
  miner::core::Logger logger(cfg.logFile, miner::core::parseLogLevel(cfg.logLevel));
  miner::core::MetricsRegistry metrics;

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
