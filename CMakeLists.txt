cmake_minimum_required(VERSION 3.13)
project(bcLib VERSION 0.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/dependencies.cmake")

add_library(bcl)
target_sources(bcl
  PRIVATE
    src/messagethread.cc
    src/messagethreadpool.cc
    src/threadpool.cc
    src/consolehandler.cc
    src/consolesecret.cc
    src/fileutil.cc
    src/logutil.cc
    src/executil.cc
    src/socketutil.cc
    src/serversocketutil.cc
    src/stringutil.cc
    src/singleton.cc
    src/pathutil.cc
    src/firewallcontrol.cc
    src/filewatcher.cc
    src/servicecontrol.cc
    src/ipaddress.cc
    src/socketaddress.cc
  PUBLIC
    include/bcl/channel.h
    include/bcl/messagethread.h
    include/bcl/messagethreadpool.h
    include/bcl/threadpool.h
    include/bcl/consolehandler.h
    include/bcl/consolesecret.h
    include/bcl/fileutil.h
    include/bcl/logutil.h
    include/bcl/priorityqueueex.h
    include/bcl/executil.h
    include/bcl/socketutil.h
    include/bcl/serversocketutil.h
    include/bcl/stringutil.h
    include/bcl/singleton.h
    include/bcl/pathutil.h
    include/bcl/firewallcontrol.h
    include/bcl/filewatcher.h
    include/bcl/servicecontrol.h
    include/bcl/ipaddress.h
    include/bcl/socketaddress.h
)

target_include_directories(bcl PUBLIC
  include/
)

if (WIN32)
target_sources(bcl
  PRIVATE
    src/windowsservice.cc
  PUBLIC
    include/bcl/windowsservice.h
)
target_link_libraries(bcl PUBLIC
  Ws2_32 # socketutil
  ole32 # firewallcontrol
  oleaut32 # firewallcontrol
)
set_target_properties(bcl PROPERTIES COMPILE_DEFINITIONS
  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)
target_compile_definitions(bcl PRIVATE
  UNICODE
)
endif()

if(UNIX AND NOT APPLE)
target_link_libraries(bcl PUBLIC
  pthread # threading
  stdc++fs # filesystem
)
endif()

if (${TESTS})
add_subdirectory(tests)
endif()
