cmake_minimum_required(VERSION 3.5)

set(target home-management-client)
project(${target} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network REQUIRED)

set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(source_dir ${PROJECT_SOURCE_DIR}/src)

set(header_files 
    ${include_dir}/mainwindow.h
)

set(source_files
    ${source_dir}/mainwindow.cpp
)

set(form_files
    ${source_dir}/mainwindow.ui
)

set(PROJECT_SOURCES
    ${source_dir}/main.cpp
    ${header_files}
    ${source_files}
    ${form_files}
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${target}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${target}
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(${target} 
    PRIVATE 
        Qt${QT_VERSION_MAJOR}::Widgets 
        Qt${QT_VERSION_MAJOR}::Network
    PRIVATE
        calendar
)

target_include_directories(${target} PRIVATE include)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${target})
endif()

enable_testing()
add_subdirectory(libs)
