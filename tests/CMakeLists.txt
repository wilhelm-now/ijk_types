add_executable(basic basic.cpp)
target_link_libraries(basic quat)
add_test(
	NAME basic
	WORKING_DIRECTORY $<TARGET_FILE_DIR:basic>
	COMMAND $<TARGET_FILE_NAME:basic>
)