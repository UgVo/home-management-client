cmake_minimum_required(VERSION 3.28)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fmodules-ts")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network REQUIRED)

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
        #FILE_SET CXX_MODULES FILES
        #${SOURCE_DIR}/utils.cpp
    ${INCLUDE_FILES}
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
