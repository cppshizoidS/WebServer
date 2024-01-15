#ifndef IO_URING_HPP
#define IO_URING_HPP

#include <liburing.h>
#include <sys/socket.h>

#include <span>
#include <vector>
struct io_uring_buf_ring;
struct io_uring_cqe;

namespace web_server {
struct sqe_data {
  void *coroutine = nullptr;
  int cqe_res = 0;
  unsigned int cqe_flags = 0;
};

class io_uring {
public:
  static auto get_instance() noexcept -> io_uring &;

  io_uring();

  ~io_uring();

  io_uring(io_uring &&other) = delete;

  auto operator=(io_uring &&other) -> io_uring & = delete;

  io_uring(const io_uring &other) = delete;

  auto operator=(const io_uring &other) -> io_uring & = delete;

  class cqe_iterator {
  public:
    explicit cqe_iterator(const ::io_uring *io_uring, const unsigned int head);

    cqe_iterator(const cqe_iterator &) = default;

    auto operator++() noexcept -> cqe_iterator &;

    auto operator!=(const cqe_iterator &right) const noexcept -> bool;

    auto operator*() const noexcept -> io_uring_cqe *;

  private:
    const ::io_uring *io_uring_;
    unsigned int head_;
  };

  auto begin() -> cqe_iterator;

  auto end() -> cqe_iterator;

  auto cqe_seen(io_uring_cqe *const cqe) -> void;

  auto submit_and_wait(int wait_nr) -> int;

  auto submit_multishot_accept_request(
      sqe_data *sqe_data, int raw_file_descriptor, sockaddr *client_addr, socklen_t *client_len
  ) -> void;

  auto submit_recv_request(sqe_data *sqe_data, int raw_file_descriptor, size_t length) -> void;

  auto submit_send_request(
      sqe_data *sqe_data, int raw_file_descriptor, const std::span<char> &buffer, size_t length
  ) -> void;

  auto submit_splice_request(
      sqe_data *sqe_data, int raw_file_descriptor_in, int raw_file_descriptor_out, size_t length
  ) -> void;

  auto submit_cancel_request(sqe_data *sqe_data) -> void;

  auto setup_buffer_ring(
      io_uring_buf_ring *buffer_ring, std::span<std::vector<char>> buffer_list,
      unsigned int buffer_ring_size
  ) -> void;

  auto add_buffer(
      io_uring_buf_ring *buffer_ring, std::span<char> buffer, unsigned int buffer_id,
      unsigned int buffer_ring_size
  ) -> void;

private:
  ::io_uring io_uring_;
};
} // namespace web_server

#endif
