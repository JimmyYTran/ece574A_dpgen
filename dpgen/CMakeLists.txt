set(PROJECT_SOURCES
    Data.cpp
    fileio.cpp
    Operation.cpp
    parser.cpp
)

add_library(dpgen_lib ${PROJECT_SOURCES})
add_executable(dpgen dpgen.cpp)
target_link_libraries(dpgen dpgen_lib)