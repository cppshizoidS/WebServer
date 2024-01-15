cmake_minimum_required(VERSION 3.20)

project(web_server)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(IWYU_PROGRAM NAMES include-what-you-use) # https://github.com/include-what-you-use/include-what-you-use
if(IWYU_PROGRAM)
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PROGRAM})
endif()

file(GLOB SOURCE_FILE src/*.cpp)
add_executable(web_server ${SOURCE_FILE})

target_include_directories(web_server PRIVATE include)
target_compile_options(web_server PRIVATE -Wall -Wextra -fcoroutines)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  target_link_libraries(web_server PRIVATE asan ubsan uring)
  target_compile_options(web_server PRIVATE -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)
else()
  target_link_libraries(web_server PRIVATE uring)
endif()
