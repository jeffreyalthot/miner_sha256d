#include "miner/sv2.hpp"

namespace miner::sv2 {
miner::Bytes encodeSetupConnection(const SetupConnection& msg) {
  std::string flat = msg.vendor + "|" + msg.hardwareVersion + "|" + msg.firmware + "|" + msg.deviceId;
  return miner::Bytes(flat.begin(), flat.end());
}
} // namespace miner::sv2
