# CMake Config (MousePaw Media Build System)
# Version: 3.2.1

# CHANGE: Name your project here
project("IOSqueak Tester")

# Specify the verison being used.
cmake_minimum_required(VERSION 3.8)

# Import user-specified library path configuration
message("Using ${CONFIG_FILENAME}.config")
include(${CMAKE_HOME_DIRECTORY}/../${CONFIG_FILENAME}.config)

# CHANGE: Specify output binary name
set(TARGET_NAME "iosqueak-tester")

# SELECT: Project artifact type
#set(ARTIFACT_TYPE "library")
set(ARTIFACT_TYPE "executable")

# CHANGE: Find dynamic library dependencies.
#set(CURSES_NEED_NCURSES TRUE)
#find_package(Curses)

# CHANGE: Include headers of dependencies.
set(INCLUDE_LIBS
    ${CMAKE_HOME_DIRECTORY}/../iosqueak-source/include
    ${ARCTICTERN_DIR}/include
    ${EVENTPP_DIR}/include
#    ${CURSES_INCLUDE_DIRS}
)

# CHANGE: Include files to compile.
set(FILES
    main.cpp
)

# CHANGE: Link against dependencies.
set(LINK_LIBS
    ${CMAKE_HOME_DIRECTORY}/../iosqueak-source/lib/${CMAKE_BUILD_TYPE}/libiosqueak.a
#    ${CURSES_LIBRARIES}
)

# Imports build script. (Change if necessary to point to build.cmake)
include(${CMAKE_HOME_DIRECTORY}/../build_system/build.cmake)
