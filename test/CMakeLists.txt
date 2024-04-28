cmake_minimum_required(VERSION 3.14)

include(CTest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network)

add_executable(
  ${target}_test
  ${source_files}
  ${header_files}
  event_test.cpp
  agenda_test.cpp
  rrules_test.cpp
  utils_test.cpp
)
target_link_libraries(
  ${target}_test
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  GTest::gtest_main
)

#target_include_directories(test PUBLIC ../include)
target_include_directories(${target}_test PUBLIC 
    "${PROJECT_BINARY_DIR}"
    ../include
)

include(GoogleTest)

gtest_discover_tests( ${target}_test 
  DISCOVERY_TIMEOUT 600  
  DISCOVERY_MODE PRE_TEST
)

add_dependencies(${target}_test
  ${target}
)
