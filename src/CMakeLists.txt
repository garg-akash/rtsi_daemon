add_library(rtsiPnet SHARED rtsi_profinet.cpp)
target_link_libraries(rtsiPnet Threads::Threads ${CMAKE_SOURCE_DIR}/lib/libosal.a ${RTLIB})