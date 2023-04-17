#ifndef __PROFINET_MODULE_H__
#define __PROFINET_MODULE_H__

#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

void pnet_module_send(bool state, bool io, bool j, bool tcp, bool b, bool i, bool f, bool is_big_endian);

void pnet_module_recvdata(uint32_t module_id, uint32_t offset, uint32_t len, const unsigned char *data, bool is_big_endian);


#ifdef __cplusplus
}
#endif

#endif