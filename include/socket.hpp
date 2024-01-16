#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>

#include <coroutine>
#include <optional>
#include <span>
#include <tuple>

#include "file_descriptor.hpp"
#include "io_uring.hpp"
#include "task.hpp"

namespace web_server {

class server_socket : public file_descriptor {
public:
  server_socket();

  auto bind(const char *port) -> void;

  auto listen() const -> void;

  class multishot_accept_guard {
  public:
    multishot_accept_guard(
        int raw_file_descriptor, sockaddr_storage *client_address, socklen_t *client_address_size
    );
    ~multishot_accept_guard();

    [[nodiscard]] auto await_ready() const -> bool;
    auto await_suspend(std::coroutine_handle<> coroutine) -> void;
    auto await_resume() -> int;

  private:
    bool initial_await_ = true;
    const int raw_file_descriptor_;
    sockaddr_storage *client_address_;
    socklen_t *client_address_size_;
    sqe_data sqe_data_;
  };

  auto accept(sockaddr_storage *client_address = nullptr, socklen_t *client_address_size = nullptr)
      -> multishot_accept_guard &;

private:
  std::optional<multishot_accept_guard> multishot_accept_guard_;
};

class client_socket : public file_descriptor {
public:
  explicit client_socket(int raw_file_descriptor);

  class recv_awaiter {
  public:
    recv_awaiter(int raw_file_descriptor, size_t length);

    [[nodiscard]] auto await_ready() const -> bool;
    auto await_suspend(std::coroutine_handle<> coroutine) -> void;
    auto await_resume() -> std::tuple<unsigned int, ssize_t>;

  private:
    const int raw_file_descriptor_;
    const size_t length_;
    sqe_data sqe_data_;
  };

  auto recv(size_t length) -> recv_awaiter;

  class send_awaiter {
  public:
    send_awaiter(int raw_file_descriptor, const std::span<char> &buffer, size_t length);

    [[nodiscard]] auto await_ready() const -> bool;
    auto await_suspend(std::coroutine_handle<> coroutine) -> void;
    [[nodiscard]] auto await_resume() const -> ssize_t;

  private:
    const int raw_file_descriptor_;
    const size_t length_;
    const std::span<char> &buffer_;
    sqe_data sqe_data_;
  };

  auto send(const std::span<char> &buffer, size_t length) -> task<ssize_t>;
};

} // namespace web_server

#endif
