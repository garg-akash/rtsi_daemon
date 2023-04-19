add_library(rtsiPnet SHARED rtsi_profinet.cpp profinet_module.cpp)
# set_property(TARGET rtsiPnet PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(rtsiPnet Threads::Threads ${CMAKE_SOURCE_DIR}/lib/libosal.a ${RTLIB} ${CMAKE_SOURCE_DIR}/lib/libpn_dev.a ${CMAKE_SOURCE_DIR}/lib/libprofinet.a ${CMAKE_SOURCE_DIR}/lib/libeli_log.a)