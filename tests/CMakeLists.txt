foreach(TESTABLE
	directions
	quat)
	add_executable(test_${TESTABLE} "${TESTABLE}.test.cpp")
	target_link_libraries(test_${TESTABLE} ijk)
	add_test(
		NAME ${TESTABLE}
		WORKING_DIRECTORY $<TARGET_FILE_DIR:test_${TESTABLE}>
		COMMAND $<TARGET_FILE_NAME:test_${TESTABLE}>
	)
endforeach()
