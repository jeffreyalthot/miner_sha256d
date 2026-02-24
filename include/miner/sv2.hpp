#pragma once
#include "miner/config.hpp"
#include "miner/logging.hpp"
#include "miner/common.hpp"
#include <cstdint>
#include <optional>
#include <string>

namespace miner::sv2 {

struct Frame {
  std::uint16_t type{0};
  miner::Bytes payload;
};

miner::Bytes encodeFrame(const Frame& frame);
std::optional<Frame> decodeFrame(miner::Bytes& buffer);

struct SetupConnection {
  std::string vendor{"miner_sha256d"};
  std::string hardwareVersion{"cpu"};
  std::string firmware{"0.1"};
  std::string deviceId{"cpu-demo"};
};

struct OpenChannel {
  std::string user;
  std::uint32_t nominalHashrate{0};
};

struct NewMiningJob {
  std::uint32_t channelId{0};
  std::uint32_t jobId{0};
  miner::Hash256 merkleRoot{};
  std::uint32_t version{0};
  std::uint32_t nbits{0};
  std::uint32_t ntime{0};
};

miner::Bytes encodeSetupConnection(const SetupConnection& msg);

enum class SessionState { Disconnected, Connected, SetupSent, Authenticated, ChannelOpen, Mining };

class SessionMachine {
public:
  void onConnected();
  void onSetupSent();
  void onSetupOk();
  void onChannelOpen();
  SessionState state() const;

private:
  SessionState state_{SessionState::Disconnected};
};

bool connectAndSetup(const core::PoolProfile& pool, const core::SV2Config& sv2Cfg, core::Logger& logger);

} // namespace miner::sv2
