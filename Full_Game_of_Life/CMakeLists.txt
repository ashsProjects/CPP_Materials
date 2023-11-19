cmake_minimum_required(VERSION 3.20)
project(HW5)
set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES ${PROJECT_NAME}.tar)

# Using -Wall & -Wfatal-errors are required:
add_compile_options(-Wall -Wfatal-errors)

# These compile flags are highly recommended, but not required:
add_compile_options(-Wextra -Wpedantic)

# Optional super-strict mode:
# add_compile_options(-fmessage-length=80 -fno-diagnostics-show-option)
# add_compile_options(-fstack-protector-all -g -O3 -Walloc-zero)
# add_compile_options(-Walloca -Wctor-dtor-privacy -Wduplicated-cond -Wno-unused-parameter)
# add_compile_options(-Wduplicated-branches -Werror -Winit-self)
# add_compile_options(-Wlogical-op -Wold-style-cast -Wshadow)
# add_compile_options(-Wunused-const-variable=1 -Wzero-as-null-pointer-constant)

# add_compile_options must be BEFORE add_executable or add_library.

add_library(${PROJECT_NAME} game_of_life.cc) 

#These next 2 lines allow you to define
# add_executable(game_test game_test.cc)
# target_link_libraries(game_test ${PROJECT_NAME})

# Create a tar file every time:
add_custom_target(submission COMMAND tar -cf ${PROJECT_NAME}.tar game_of_life.cc *.h CMakeLists.txt)