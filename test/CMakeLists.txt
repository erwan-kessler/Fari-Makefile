add_executable(testSet ../snow/snow.h ../highwayhash/highway.h ../highwayhash/highway.c ../src/Set.h ../src/Set.c testSet.c)
add_executable(testParser ../snow/snow.h ../highwayhash/highway.h ../highwayhash/highway.c ../src/Set.h ../src/Set.c ../src/Parser.h ../src/Parser.c testParser.c)
add_executable(testCommand ../snow/snow.h ../highwayhash/highway.h ../highwayhash/highway.c ../src/Set.h ../src/Set.c ../src/Parser.h ../src/Parser.c ../src/Checker.c ../src/Commands.c
		testCommand.c)
add_executable(testLogs ../highwayhash/highway.c ../src/Set.c ../src/Parser.c ../src/Checker.c ../src/Commands.c ../src/launcher.c ../src/json_logs.c ../cJson/cJSON.c testLogs.c)

file(COPY ${CMAKE_SOURCE_DIR}/test/testFileForParser DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/test/testForFari DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/test/testForLogs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/test/testFileForCommands DESTINATION ${CMAKE_CURRENT_BINARY_DIR})