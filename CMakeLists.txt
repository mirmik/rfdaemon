cmake_minimum_required(VERSION 3.5.1)
project(igris)
enable_testing()
set(CMAKE_VERBOSE_MAKEFILE on)

add_compile_options(
  "-g3"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CC_STANDARD 11)

find_library(SYSTEMD_LIBRARY NAMES systemd libsystemd.so.0)
if(NOT SYSTEMD_LIBRARY)
  find_file(SYSTEMD_LIBRARY
    NAMES libsystemd.so.0
    PATHS /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu /lib /lib64 /lib/x86_64-linux-gnu
  )
endif()
if(NOT SYSTEMD_LIBRARY)
  message(FATAL_ERROR "libsystemd is required to build rfdaemon")
endif()

execute_process(COMMAND ircc resources.txt -o ircc_resources.gen.cpp --sources-cmake
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE RESOURCE_LIST)
set(IRCC_GENERATED_CPP "${CMAKE_CURRENT_BINARY_DIR}/ircc_resources.gen.cpp")
add_custom_command(
    OUTPUT "${IRCC_GENERATED_CPP}"
    COMMAND ircc resources.txt -o "${IRCC_GENERATED_CPP}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS ${RESOURCE_LIST}
)

file(GLOB SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

file(GLOB SOURCES_RFDAEMONCTL CONFIGURE_DEPENDS
  src/rfdaemonctl/*.cpp
)

add_executable(rfdaemon 
  "${IRCC_GENERATED_CPP}"
  ${SOURCES}
  src/app/main.cpp)
target_include_directories(rfdaemon PRIVATE src)

add_executable(rfdaemonctl 
  ${SOURCES_RFDAEMONCTL})
target_include_directories(rfdaemonctl PRIVATE src)

target_link_libraries(rfdaemon
  pthread
  igris
  nos
  util
  ${SYSTEMD_LIBRARY}
)

target_link_libraries(rfdaemonctl
  igris
  nos
)

# Tests
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS 
  tests/*.cpp)

if(TEST_SOURCES)
  add_executable(rfdaemon_tests
    "${IRCC_GENERATED_CPP}"
    ${TEST_SOURCES}
    ${SOURCES}
  )

  target_include_directories(rfdaemon_tests PRIVATE src tests)

  target_link_libraries(rfdaemon_tests
    pthread
    igris
    nos
    util
    ${SYSTEMD_LIBRARY}
  )

  add_test(NAME rfdaemon_tests COMMAND rfdaemon_tests)
endif()
