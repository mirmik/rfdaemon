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
                OUTPUT_VARIABLE RESOURCE_LIST)
add_custom_command(
    OUTPUT ircc_resources.gen.cpp
    COMMAND ircc resources.txt -o ircc_resources.gen.cpp
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
  ircc_resources.gen.cpp
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
  ircc_resources.gen.cpp
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
