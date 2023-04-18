#include "rtsi_pnet/profinet_module.h"

#include "p-net/pnet.h"

static void pnet_module_o_io(const unsigned char *data, uint32_t len, bool is_big_endian)
{

}

static void pnet_module_o_reg1(const unsigned char *data, uint32_t len, bool is_big_endian)
{

}

static void pnet_module_o_reg2(const unsigned char *data, uint32_t len, bool is_big_endian)
{

}

// Receive data from the PLC to set up the robot
void pnet_module_recvdata(STATION_MODULE_TYPE module_id, uint32_t offset, uint32_t len, const unsigned char *data, bool is_big_endian)
{
  switch(module_id)
  {
    case PNSTATION_MODULE_O_IO:
      pnet_module_o_io(data, len, is_big_endian);
      break;
    case PNSTATION_MODULE_O_REG1:
      pnet_module_o_reg1(data, len, is_big_endian);
      break;
    case PNSTATION_MODULE_O_REG2:
      pnet_module_o_reg2(data, len, is_big_endian);
      break;
    default: 
      break;
  }
}