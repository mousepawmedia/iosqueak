# CMake Build Script (MousePaw Media Build System)
# Version: 3.2.1

# LICENSE
# Copyright (c) 2021 MousePaw Media.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
# CONTRIBUTING
# See https://www.mousepawmedia.com/developers for information
# on how to contribute to our projects.

# Compiler and Version check...
# Allow Clang 3.4
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.4")
        message(FATAL_ERROR "Clang is too old. >= v3.4 required. Aborting build.")
    endif()
    set(COMPILERTYPE "clang")
    message("Clang 3.4 or later detected. Proceeding...")
# Allow GCC 5.x
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5")
        message(FATAL_ERROR "GCC is too old. >= v5.x required. Aborting build.")
    endif()
    set(COMPILERTYPE "gcc")
    message("GCC 5.x or later detected. Proceeding...")
# Allow compilers that simulate GCC 5.x.
elseif(CMAKE_CXX_SIMULATE_ID STREQUAL "GNU")
    if(CMAKE_CXX_SIMULATE_VERSION VERSION_LESS "5")
        message(FATAL_ERROR "Not simulating GCC 5.x. Aborting build.")
    endif()
    set(COMPILERTYPE "gcc")
    message("GCC (5.x or later) simulation detected. Proceeding...")
else()
    message(FATAL_ERROR "Not using or simulating a compatible compiler (minimum GCC 5.x, Clang 3.4). Other compilers are not yet supported. Aborting build.")
endif()

# Target C++17
set(CMAKE_CXX_STANDARD 17)
# Disable extensions (turns gnu++14 to c++14)
set(CMAKE_CXX_EXTENSIONS OFF)

# Target 32-bit or 64-bit, based on ARCH variable.
if(ARCH EQUAL 32)
    message("Triggered 32-bit build (-m32).")
    add_definitions(-m32)
elseif(ARCH EQUAL 64)
    message("Triggered 64-bit build (-m64).")
    add_definitions(-m64)
# If 32-bit or 64-bit wasn't specified, just use the default.
elseif(NOT ARCH)
    message("Building for default architecture.")
else()
    message(FATAL_ERROR "Invalid architecture (${ARCH}) specified. Aborting build.")
endif()

# Our global compiler flags.
add_definitions(-Wall -Wextra -Werror)
#add_definitions(-Wall -Wextra)

if(COMPILERTYPE STREQUAL "gcc")
    # Set debug flags. -g is a default.
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ")
    # Set release flags. -O3 is a default.
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
elseif(COMPILERTYPE STREQUAL "clang")
    # Use libc++ if requested.
    if(LLVM)
        add_definitions(-stdlib=libc++)
        message("Using LLVM libc++...")
    endif()
    # Set debug flags. -g is a default.
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ")
    # Set release flags. -O3 is a default.
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ")
endif()

# Select appropriate output (lib or bin)
if(ARTIFACT_TYPE STREQUAL "library")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../lib/${CMAKE_BUILD_TYPE}")
elseif(ARTIFACT_TYPE STREQUAL "executable")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../bin/${CMAKE_BUILD_TYPE}")
endif()

include_directories(include)

include_directories(${INCLUDE_LIBS})

if(ARTIFACT_TYPE STREQUAL "library")
    add_library(${TARGET_NAME} ${FILES})
elseif(ARTIFACT_TYPE STREQUAL "executable")
    add_executable(${TARGET_NAME} ${FILES})
endif()

target_link_libraries(${TARGET_NAME} ${LINK_LIBS})

if(COMPILERTYPE STREQUAL "clang")
    if(SAN STREQUAL "address")
        add_definitions(-O1 -fsanitize=address -fno-optimize-sibling-calls -fno-omit-frame-pointer)
        set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=address")
        message("Compiling with AddressSanitizer.")
    elseif(SAN STREQUAL "leak")
        add_definitions(-fsanitize=leak)
        set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=leak")
        message("Compiling with LeakSanitizer.")
    elseif(SAN STREQUAL "memory")
        add_definitions(-O1 -fsanitize=memory -fno-optimize-sibling-calls -fno-omit-frame-pointer)
        set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=memory")
        message("Compiling with MemorySanitizer.")
    elseif(SAN STREQUAL "thread")
        add_definitions(-O1 -fsanitize=thread)
        set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=thread")
        message("Compiling with ThreadSanitizer.")
    elseif(SAN STREQUAL "undefined")
        add_definitions(-fsanitize=undefined)
        set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=undefined")
        message("Compiling with UndefiniedBehaviorSanitizer.")
    endif()
elseif(NOT SAN MATCHES "")
    message("${SAN} is not available with compiler ${COMPILERTYPE}.")
endif()
