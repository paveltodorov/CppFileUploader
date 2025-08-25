cmake_minimum_required(VERSION 3.14)
project(file_uploader)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

set(CPR_USE_SYSTEM_CURL OFF)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.5
)
FetchContent_MakeAvailable(cpr)

set(SOURCES
    src/main.cpp
    src/ApiClient.cpp
    src/Token.cpp
)

add_executable(file_uploader ${SOURCES})

target_link_libraries(file_uploader PRIVATE nlohmann_json::nlohmann_json cpr::cpr)
