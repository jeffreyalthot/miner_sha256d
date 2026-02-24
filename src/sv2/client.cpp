#include "miner/sv2.hpp"

#include <cerrno>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace miner::sv2 {
namespace {
#ifdef _WIN32
using SocketHandle = SOCKET;
constexpr SocketHandle kInvalidSocket = INVALID_SOCKET;

bool initializeSockets() {
  static const auto started = []() {
    WSADATA wsaData{};
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
  }();
  return started;
}

void closeSocket(SocketHandle fd) {
  closesocket(fd);
}

std::string lastSocketError() {
  return "code=" + std::to_string(WSAGetLastError());
}
#else
using SocketHandle = int;
constexpr SocketHandle kInvalidSocket = -1;

bool initializeSockets() {
  return true;
}

void closeSocket(SocketHandle fd) {
  close(fd);
}

std::string lastSocketError() {
  return std::strerror(errno);
}
#endif

SocketHandle openTcpSocket(const core::PoolProfile& pool) {
  if (!initializeSockets()) return kInvalidSocket;

  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  addrinfo* result = nullptr;
  auto port = std::to_string(pool.port);
  if (getaddrinfo(pool.host.c_str(), port.c_str(), &hints, &result) != 0) return kInvalidSocket;

  SocketHandle fd = kInvalidSocket;
  for (addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fd == kInvalidSocket) continue;
    if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) break;
    closeSocket(fd);
    fd = kInvalidSocket;
  }

  freeaddrinfo(result);
  return fd;
}
} // namespace

bool connectAndSetup(const core::PoolProfile& pool, const core::SV2Config& sv2Cfg, core::Logger& logger) {
  if (pool.tls) {
    logger.log(core::LogLevel::Warn, "TLS demandé mais non implémenté dans cette base (utiliser tls=false)", "sv2");
    return false;
  }

  auto fd = openTcpSocket(pool);
  if (fd == kInvalidSocket) {
    logger.log(core::LogLevel::Warn,
               "Connexion pool impossible " + pool.host + ":" + std::to_string(pool.port) + " err=" + lastSocketError(),
               "sv2");
    return false;
  }

  SetupConnection setup;
  setup.deviceId = sv2Cfg.deviceId;
  auto setupPayload = encodeSetupConnection(setup);
  auto frame = encodeFrame(Frame{0x0001, setupPayload});

  auto written = send(fd, reinterpret_cast<const char*>(frame.data()), static_cast<int>(frame.size()), 0);
  if (written < 0) {
    logger.log(core::LogLevel::Warn, "Envoi SetupConnection échoué", "sv2");
    closeSocket(fd);
    return false;
  }

  logger.log(core::LogLevel::Info,
             "Pool connecté " + pool.name + " (" + pool.host + ":" + std::to_string(pool.port) + "), setup envoyé pour user=" + pool.user,
             "sv2");
  closeSocket(fd);
  return true;
}

} // namespace miner::sv2
