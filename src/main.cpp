#include "http_server.hpp"

auto main() -> int {
  web_server::http_server http_server;
  http_server.listen("8080");
}
