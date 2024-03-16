cmake_minimum_required(VERSION 3.18.1)

project(dpgen)

message("Started cmake on ${PROJECT_NAME}.")

if ( CMAKE_COMPILER_IS_GNUCC )
    message("GNUCC compiler flags enabled...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -std=gnu++17")
endif( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    message("MSVC compiler flags enabled...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -std=gnu++17")
endif( MSVC )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TOP_LEVEL_DIR ${PROJECT_SOURCE_DIR})
set(TOP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/dpgen)
set(TOP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/dpgen/include)
set(TOP_EXAMPLES_DIR ${PROJECT_SOURCE_DIR}/examples)

include_directories(dpgen/include)

add_subdirectory(dpgen)

if ( ENABLE_EXAMPLES )
    message("Examples enabled...")
    add_subdirectory(examples)
endif( ENABLE_EXAMPLES )