cmake_minimum_required(VERSION 3.5)

project(daemon)

set (CMAKE_CXX_STANDARD 11)

find_package(cs_rtsi REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread program_options)

add_executable(rdaemon rtsi_daemon)
target_link_libraries(rdaemon csrtsi Boost::thread) 