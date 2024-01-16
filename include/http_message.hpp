#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <string>
#include <tuple>
#include <vector>

namespace web_server {

class http_request {
public:
  std::string method;
  std::string url;
  std::string version;
  std::vector<std::tuple<std::string, std::string>> header_list;
};

class http_response {
public:
  std::string version;
  std::string status;
  std::string status_text;
  std::vector<std::tuple<std::string, std::string>> header_list;

  [[nodiscard]] auto serialize() const -> std::string;
};

} // namespace web_server
#endif
