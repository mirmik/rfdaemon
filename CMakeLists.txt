cmake_minimum_required(VERSION 3.5.1)
project(igris)
set( CMAKE_VERBOSE_MAKEFILE on )

add_compile_options(
  "-g3"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CC_STANDARD 11)

file(GLOB SOURCES 
	main.cpp
	Device.cpp
	DeviceManager.cpp
	parameter.cpp
	AppManager.cpp
	RFDaemon.cpp
	RFDaemonServer.cpp
	string_constants.cpp
	config_json_default.cpp
) 

add_executable(rfdaemon ${SOURCES})
target_include_directories(rfdaemon PRIVATE .)


target_link_libraries(rfdaemon pthread)
target_link_libraries(rfdaemon igris)
target_link_libraries(rfdaemon jsoncpp)


