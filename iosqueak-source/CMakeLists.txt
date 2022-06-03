# CMake Config (MousePaw Media Build System)
# Version: 3.2.1

# CHANGE: Name your project here
project("IOSqueak")

# Specify the verison being used.
cmake_minimum_required(VERSION 3.8)

# Import user-specified library path configuration
message("Using ${CONFIG_FILENAME}.config")
include(${CMAKE_HOME_DIRECTORY}/../${CONFIG_FILENAME}.config)

# CHANGE: Specify output binary name
set(TARGET_NAME "iosqueak")

# SELECT: Project artifact type
set(ARTIFACT_TYPE "library")
#set(ARTIFACT_TYPE "executable")

# CHANGE: Find dynamic library dependencies.

# CHANGE: Include headers of dependencies.
set(INCLUDE_LIBS
    ${ARCTICTERN_DIR}/include
    ${EVENTPP_DIR}/include
)

# CHANGE: Include files to compile.
set(FILES
    include/iosqueak/stringify/anything.hpp
    include/iosqueak/stringify/exception.hpp
    include/iosqueak/stringify/function.hpp
    include/iosqueak/stringify/logic.hpp
    include/iosqueak/stringify/memory.hpp
    include/iosqueak/stringify/numbers.hpp
    include/iosqueak/stringify/types.hpp
    include/iosqueak/stringify/utilities.hpp

    include/iosqueak/tools/memlens.hpp
    include/iosqueak/tools/typemap.hpp

    include/iosqueak/utilities/bitfield.hpp

    include/iosqueak/channel.hpp
    include/iosqueak/ioctrl.hpp
    include/iosqueak/iodecor.hpp
    include/iosqueak/ioformat.hpp
    include/iosqueak/stringify.hpp
    include/iosqueak/stringy.hpp
    include/iosqueak/window.hpp

    src/channel.cpp
    src/iodecor.cpp
    src/ioformat.cpp
    src/stringy.cpp
    src/window.cpp
)

# CHANGE: Link against dependencies.
set(LINK_LIBS
)

# Imports build script. (Change if necessary to point to build.cmake)
include(${CMAKE_HOME_DIRECTORY}/../build_system/build.cmake)
