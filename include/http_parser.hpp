#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <optional>
#include <span>
#include <string>

namespace web_server {
class http_request;

class http_parser {
public:
  auto parse_packet(std::span<char> packet) -> std::optional<http_request>;

private:
  std::string raw_http_request_;
};
} // namespace web_server

#endif
