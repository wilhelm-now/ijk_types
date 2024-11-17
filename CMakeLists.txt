cmake_minimum_required(VERSION 3.26) # probably works on older versions but I haven't tested

project(quat_ijk)

add_library(quat INTERFACE)
target_include_directories(quat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(quat INTERFACE cxx_std_20)

if (PROJECT_IS_TOP_LEVEL)
	enable_testing()
	add_subdirectory(tests)
endif()