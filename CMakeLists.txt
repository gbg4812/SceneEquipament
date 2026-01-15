cmake_minimum_required(VERSION 3.22)
project(SceneEquipament)

include(CTest)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(UNIX)
    set( CMAKE_CXX_FLAGS_DEBUG_INIT "-g -O0 -Wall" )
elseif( WIN32 )
    ##
endif()


file(GLOB SRC src/*.cpp)

add_library(${CMAKE_PROJECT_NAME} INTERFACE
    ${SRC}
)


target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE src/)

FetchContent_Declare(
 glm
 GIT_REPOSITORY	https://github.com/g-truc/glm.git
 GIT_TAG 	a532f5b1cf27d6a3c099437e6959cf7e398a0a67
)

FetchContent_MakeAvailable(glm)


target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE glm::glm)

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
    ${CMAKE_PROJECT_NAME}
    GTest::gtest_main
)

add_executable(
    mesh_tests
    tests/mesh_tests.cpp
)

target_link_libraries(
    mesh_tests
    ${CMAKE_PROJECT_NAME}
    GTest::gtest_main
    glm::glm
)

include(GoogleTest)
gtest_discover_tests(model_tests)
gtest_discover_tests(mesh_tests)
