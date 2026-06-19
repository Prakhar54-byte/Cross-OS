cmake_minimum_required(VERSION 3.16)
project(cross-os VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# THE FIX: Tell CMake to process Qt macros (like Q_OBJECT) and resource files automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network)

set(PROJECT_SOURCES
    main.cpp
    backend.h
    backend.cpp
)

if(WIN32)
    list(APPEND PROJECT_SOURCES resources.rc)
endif()

qt_add_executable(cross-os WIN32
    ${PROJECT_SOURCES}
)

qt_add_qml_module(cross-os
    URI Main
    VERSION 1.0
    QML_FILES main.qml
)

target_link_libraries(cross-os PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
)