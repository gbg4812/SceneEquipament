cmake_minimum_required(VERSION 3.22)
project(SceneEquipament)

include(CTest)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(UNIX)
    set( CMAKE_CXX_FLAGS_DEBUG_INIT "-g -O0 -Wall" )
elseif( WIN32 )
    ##
endif()


file(GLOB SRC src/*.h)

add_library(${CMAKE_PROJECT_NAME} INTERFACE
    ${SRC}
)


target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE src/)

FetchContent_Declare(
    glm
    GIT_REPOSITORY	https://github.com/g-truc/glm.git
    GIT_TAG 82f86a8057c169ae0d6c939dfa148970e97d6693
)

FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE ${glm_SOURCE_DIR})
endif()


target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE glm)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
)


# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    resource_tests
    tests/resource_tests.cpp
)

target_link_libraries(
    resource_tests
    PRIVATE
    ${CMAKE_PROJECT_NAME}
    GTest::gtest_main
)

add_executable(
    mesh_tests
    tests/mesh_tests.cpp
)

target_link_libraries(
    mesh_tests
    PRIVATE
    ${CMAKE_PROJECT_NAME}
    GTest::gtest_main
)

add_executable(
    material_tests
    tests/material_tests.cpp
)

target_link_libraries(
    material_tests
    PRIVATE
    ${CMAKE_PROJECT_NAME}
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(resource_tests)
gtest_discover_tests(mesh_tests)
gtest_discover_tests(material_tests)
