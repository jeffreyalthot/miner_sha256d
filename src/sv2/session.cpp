#include "miner/sv2.hpp"

namespace miner::sv2 {
void SessionMachine::onConnected() { state_ = SessionState::Connected; }
void SessionMachine::onSetupSent() { state_ = SessionState::SetupSent; }
void SessionMachine::onSetupOk() { state_ = SessionState::Authenticated; }
void SessionMachine::onChannelOpen() { state_ = SessionState::Mining; }
SessionState SessionMachine::state() const { return state_; }
} // namespace miner::sv2
