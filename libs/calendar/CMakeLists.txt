cmake_minimum_required(VERSION 3.25)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

add_library(calendar SHARED)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INCLUDE_FILES 
    ${INCLUDE_DIR}/apiconnector.h
    ${INCLUDE_DIR}/storage.h
    ${INCLUDE_DIR}/agenda.h
    ${INCLUDE_DIR}/event.h
    ${INCLUDE_DIR}/rrules.h
    ${INCLUDE_DIR}/utils.h
)

set(SOURCES_FILES
    ${SOURCE_DIR}/apiconnector.cpp
    ${SOURCE_DIR}/storage.cpp
    ${SOURCE_DIR}/agenda.cpp
    ${SOURCE_DIR}/event.cpp
    ${SOURCE_DIR}/rrules.cpp
    ${SOURCE_DIR}/utils.cpp
)

target_sources(calendar
    PUBLIC
    ${INCLUDE_FILES}
    PRIVATE
    ${SOURCES_FILES}
)

target_link_libraries(calendar 
    PRIVATE 
        Qt${QT_VERSION_MAJOR}::Widgets 
        Qt${QT_VERSION_MAJOR}::Network
)

include_directories(include)
target_include_directories(calendar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)
