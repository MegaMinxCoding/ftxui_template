cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# --- Fetch Catch2 --------------------------------------------------------------
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.0-preview4)

FetchContent_MakeAvailable(Catch2)

# --- Fetch FTXUI --------------------------------------------------------------
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui

  # Important: Specify a GIT_TAG XXXXX here.
)

FetchContent_GetProperties(ftxui)

if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------
project(arkanoid-ftxui
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(arkanoid-ftxui src/final_programm/renderer.cpp)
target_include_directories(arkanoid-ftxui PRIVATE src)

target_link_libraries(arkanoid-ftxui
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)

project(tester
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(trial_n_error src/trial_n_error/event_trys.cpp)
target_include_directories(trial_n_error PRIVATE src)

target_link_libraries(trial_n_error
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)
