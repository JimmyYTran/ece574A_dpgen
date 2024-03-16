cmake_minimum_required(VERSION 3.18.1)
project(dpgen_examples)

function (add_example NAME)
message("Added example ${NAME}_example.")
  add_executable(${NAME}_example ${NAME}.cpp)
  target_link_libraries(${NAME}_example dpgen_lib)
endfunction()