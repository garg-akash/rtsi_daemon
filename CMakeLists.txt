cmake_minimum_required(VERSION 3.5)

project(daemon)

find_package(cs_rtsi REQUIRED)

add_executable(rdaemon rtsi_daemon)
target_link_libraries(rdaemon csrtsi) 