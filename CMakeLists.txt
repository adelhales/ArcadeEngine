cmake_minimum_required(VERSION 3.28)
project(ArcadeEngine CXX)

set(SFML_BUILD_NETWORK OFF)
set(SPDLOG_USE_STD_FORMAT ON)
set(TGUI_BACKEND SFML_GRAPHICS)

include(FetchContent)
FetchContent_Declare(sfml URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_Declare(logs URL https://github.com/gabime/spdlog/archive/refs/tags/v1.16.0.tar.gz)
FetchContent_Declare(tgui URL https://github.com/texus/TGUI/archive/refs/tags/v1.11.0.tar.gz)
FetchContent_Declare(enum URL https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.7.tar.gz)
FetchContent_MakeAvailable(sfml json logs tgui enum)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS Engine/Source/*.cpp Games/Source/*.cpp)
add_executable(ArcadeEngine ${SOURCES})

target_include_directories(ArcadeEngine PRIVATE Engine/Include Games/Include)
target_compile_features(ArcadeEngine PRIVATE cxx_std_20)
target_compile_options(ArcadeEngine PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>, /W4 /WX, -Wall -Wextra -Werror>)
target_link_libraries(ArcadeEngine PRIVATE SFML::Graphics SFML::Audio nlohmann_json::nlohmann_json spdlog::spdlog TGUI::TGUI magic_enum::magic_enum)