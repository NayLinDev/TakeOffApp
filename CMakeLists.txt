cmake_minimum_required(VERSION 3.20)

project(MyPdfApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_PREFIX_PATH
    "${CMAKE_SOURCE_DIR}/dependencies/pdfium_win_x64"
)

# frameless OFF as needed by SARibbon DO NOT CHANGE IT
option(SARIBBON_USE_FRAMELESS_LIB "Using the QWindowKit library as a frameless solution" OFF)

# DO NOT CHANGE IT
add_definitions(-DSARIBBON_USE_3RDPARTY_FRAMELESSHELPER=0)

# Find SARibbon
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SARibbonBar_DIR ${CMAKE_SOURCE_DIR}/dependencies/saribbon/lib/cmake/SARibbonBar)
else()
    set(SARibbonBar_DIR ${CMAKE_SOURCE_DIR}/dependencies/saribbon/lib/cmake/SARibbonBar)
endif()

find_package(SARibbonBar REQUIRED)

if(NOT SARibbonBar_FOUND)
    message(FATAL_ERROR "SARibbonBar not found")
endif()

# find Qt package
find_package(Qt6 6.8 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# PoDoFo from vcpkg
find_package(PoDoFo CONFIG REQUIRED)

# PDFium from the precompiled binaries
find_package(PDFium REQUIRED)

qt_add_executable(MyPdfApp
    WIN32 MACOSX_BUNDLE

    testfunctions/test_pdfium.h
    testfunctions/test_podofo.h
    mainwindow.h
    mainwindow.cpp
    ApplicationWidget.h
    ApplicationWidget.cpp
    main.cpp
    resouce.qrc
    pdfviewer.h
    pdfviewer.cpp
)

# For Windows, if a console is needed to see debug output, set WIN32_EXECUTABLE to OFF
set_target_properties(MyPdfApp PROPERTIES WIN32_EXECUTABLE OFF)

target_link_libraries(MyPdfApp PRIVATE podofo::podofo_shared
    pdfium
    Qt::Core
    Qt::Widgets
    SARibbonBar::SARibbonBar
)

add_custom_command(TARGET MyPdfApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<IF:$<CONFIG:Debug>,"${CMAKE_SOURCE_DIR}/dependencies/saribbon/bin/SARibbonBard.dll","${CMAKE_SOURCE_DIR}/dependencies/saribbon/bin/SARibbonBar.dll">
    $<TARGET_FILE_DIR:MyPdfApp>
)

add_custom_command(TARGET MyPdfApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/dependencies/pdfium_win_x64/bin/pdfium.dll"
    $<TARGET_FILE_DIR:MyPdfApp>
)
