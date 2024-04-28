cmake_minimum_required(VERSION 3.5)

set(target home-management-client)
project(${target} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network REQUIRED)

#set(TS_FILES home-management-client_fr_FR.ts)
set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(source_dir ${PROJECT_SOURCE_DIR}/src)

set(header_files 
    ${include_dir}/mainwindow.h
    ${include_dir}/apiconnector.h
    ${include_dir}/storage.h
    ${include_dir}/agenda.h
    ${include_dir}/event.h
    ${include_dir}/rrules.h
    ${include_dir}/utils.h
)

set(source_files
    ${source_dir}/mainwindow.cpp
    ${source_dir}/apiconnector.cpp
    ${source_dir}/storage.cpp
    ${source_dir}/agenda.cpp
    ${source_dir}/event.cpp
    ${source_dir}/rrules.cpp
    ${source_dir}/utils.cpp
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
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET home-management-client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    #qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${target} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${target}
            ${PROJECT_SOURCES}
        )
    endif()

    #qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(${target} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(${target} PRIVATE include)

set_target_properties(${target} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${target})
endif()

enable_testing()
add_subdirectory(test)
