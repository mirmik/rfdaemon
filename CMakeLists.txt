cmake_minimum_required(VERSION 3.18)
project(rfdaemon)
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

if(NOT COMMAND ircc_resources)
    include("${CMAKE_CURRENT_SOURCE_DIR}/../ircc/cmake/IrccResources.cmake")
endif()
ircc_resources(IRCC_GENERATED_CPP)

file(GLOB SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

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

target_link_libraries(rfdaemon PRIVATE
  pthread
  igris
  nos
  util
  ${SYSTEMD_LIBRARY}
)

target_link_libraries(rfdaemonctl PRIVATE
  igris
  nos
)

# Tests
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS 
  tests/*.cpp)

if(BUILD_TESTING AND TEST_SOURCES)
  add_executable(rfdaemon_tests
    "${IRCC_GENERATED_CPP}"
    ${TEST_SOURCES}
    ${SOURCES}
  )

  target_include_directories(rfdaemon_tests PRIVATE src tests)

  target_link_libraries(rfdaemon_tests PRIVATE
    pthread
    igris
    nos
    util
    ${SYSTEMD_LIBRARY}
  )

  add_test(NAME rfdaemon_tests COMMAND rfdaemon_tests)
endif()

if(TARGET rfdaemon)
    if(TARGET ircc_headers)
        target_link_libraries(rfdaemon PRIVATE ircc_headers)
    endif()
    target_include_directories(rfdaemon PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../ircc/src")
    if(TARGET rxcpp)
        target_link_libraries(rfdaemon PRIVATE rxcpp)
    endif()
    if(TARGET httplib_static)
        target_link_libraries(rfdaemon PRIVATE httplib_static)
    endif()
endif()

if(TARGET rfdaemonctl)
    if(TARGET rxcpp)
        target_link_libraries(rfdaemonctl PRIVATE rxcpp)
    endif()
    if(TARGET httplib_static)
        target_link_libraries(rfdaemonctl PRIVATE httplib_static)
    endif()
endif()

if(TARGET rfdaemon_tests)
    if(TARGET ircc_headers)
        target_link_libraries(rfdaemon_tests PRIVATE ircc_headers)
    endif()
    target_include_directories(rfdaemon_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../ircc/src")
    if(TARGET rxcpp)
        target_link_libraries(rfdaemon_tests PRIVATE rxcpp)
    endif()
    if(TARGET httplib_static)
        target_link_libraries(rfdaemon_tests PRIVATE httplib_static)
    endif()
endif()
