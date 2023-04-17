cmake_minimum_required(VERSION 3.5)

project(daemon)

set (CMAKE_CXX_STANDARD 11)

find_package(cs_rtsi REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread program_options)
find_package(Threads REQUIRED)

find_library(RTLIB rt)
message(+++++++ ${RTLIB})

include_directories(include)
include_directories(include/thirdparty)
# add_subdirectory(include/thirdparty/osal)
# add_subdirectory(include/thirdparty/p-net)
add_subdirectory(src)

add_executable(rdaemon rtsi_daemon)
target_link_libraries(rdaemon csrtsi Boost::thread rtsiPnet) 