cmake_minimum_required(VERSION 3.10)
project(project C)

# Detect architecture and platform
message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")

# Compiler flags for all builds
set(COMMON_FLAGS "-Wall -Wextra -g -lraylib")
set(CMAKE_C_STANDARD 99)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows (x86_64 assumed)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -fsanitize=address")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} ")
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.c src/*.h)

add_executable(project ${SRC_FILES})

