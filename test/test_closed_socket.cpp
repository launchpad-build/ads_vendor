#include "Sockets.h"

#include <csignal>
#include <cstdio>
#include <stdexcept>
#include <unistd.h>

int main()
{
  // Exercise the process-default signal behavior, including under test runners.
  std::signal(SIGPIPE, SIG_DFL);
  const int listener = socket(AF_INET, SOCK_STREAM, 0);
  if (listener < 0) return 1;
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (bind(listener, reinterpret_cast<sockaddr *>(&address), sizeof(address)) ||
      listen(listener, 1)) return 1;
  socklen_t length = sizeof(address);
  if (getsockname(listener, reinterpret_cast<sockaddr *>(&address), &length)) return 1;

  try {
    const auto addresses = bhf::ads::GetListOfAddresses(
      "127.0.0.1", std::to_string(ntohs(address.sin_port)));
    TcpSocket client(addresses.get());
    const int peer = accept(listener, nullptr, nullptr);
    if (peer < 0) throw std::runtime_error("accept failed");
    close(peer);
    close(listener);
    client.Shutdown();
    const char byte = 'x';
    const Frame frame(1, &byte);
    return client.write(frame) == 0 ? 0 : 1;
  } catch (const std::exception &error) {
    std::fprintf(stderr, "%s\n", error.what());
    close(listener);
    return 1;
  }
}
