cmake_minimum_required(VERSION 3.22)
project(SceneEquipament)

include(CTest)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC src/*.hpp)

add_library(${CMAKE_PROJECT_NAME} INTERFACE
    ${SRC}
)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    model_tests
    tests/model_tests.cpp
)
target_link_libraries(
    model_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(model_tests)
