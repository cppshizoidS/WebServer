#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <cstddef>
#include <thread>

#include "socket.hpp"
#include "task.hpp"
#include "thread_pool.hpp"

namespace web_server {
class thread_worker {
public:
  explicit thread_worker(const char *port);

  auto accept_client() -> task<>;

  auto handle_client(client_socket client_socket) -> task<>;

  auto event_loop() -> task<>;

private:
  server_socket server_socket_;
};

class http_server {
public:
  explicit http_server(size_t thread_count = std::thread::hardware_concurrency());

  auto listen(const char *port) -> void;

private:
  thread_pool thread_pool_;
};
} // namespace web_server

#endif
