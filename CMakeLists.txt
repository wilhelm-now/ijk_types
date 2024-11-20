cmake_minimum_required(VERSION 3.26) # probably works on older versions but I haven't tested

project(ijk_types)

add_library(ijk INTERFACE)
target_include_directories(ijk INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(ijk INTERFACE cxx_std_20)

if (PROJECT_IS_TOP_LEVEL)
	enable_testing()
	add_subdirectory(tests)
endif()