cmake_minimum_required(VERSION 3.28)

set(target home-management-client)
project(${target} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
qt_standard_project_setup()

set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(source_dir ${PROJECT_SOURCE_DIR}/src)
set(resources_dir ${PROJECT_SOURCE_DIR}/resources)

set(header_files 
    ${include_dir}/mainwindow.h
    ${include_dir}/eventdaywidget.h
)

set(source_files
    ${source_dir}/mainwindow.cpp
    ${source_dir}/eventdaywidget.cpp
)

set(form_files
    ${source_dir}/mainwindow.ui
    ${source_dir}/eventdaywidget.ui
)

set(PROJECT_SOURCES
    ${source_dir}/main.cpp
    ${resources_dir}/resources.qrc
    ${header_files}
    ${source_files}
    ${form_files}
    ${TS_FILES}
)

qt_add_executable(${target}
    ${PROJECT_SOURCES}
)

target_link_libraries(${target} 
    PRIVATE 
        Qt6::Widgets 
        Qt6::Network
        calendar
)

target_include_directories(${target} PRIVATE include)


enable_testing()
add_subdirectory(libs)
