add_library(rtsiPnet rtsi_profinet.cpp)
target_link_libraries(rtsiPnet Threads::Threads ${CMAKE_SOURCE_DIR}/lib/libosal.a ${RTLIB} ${CMAKE_SOURCE_DIR}/lib/libpn_dev.a)