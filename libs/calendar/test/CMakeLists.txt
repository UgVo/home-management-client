cmake_minimum_required(VERSION 3.14)

include(CTest)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network) 

add_executable(
  calendar_test
  event_test.cpp
  agenda_test.cpp
  rrules_test.cpp
  utils_test.cpp
)
target_link_libraries(
  calendar_test
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  GTest::gtest_main
  calendar
)

include(GoogleTest)

gtest_discover_tests(calendar_test 
  DISCOVERY_TIMEOUT 600  
  DISCOVERY_MODE PRE_TEST
)

add_dependencies(calendar_test
  calendar
)
