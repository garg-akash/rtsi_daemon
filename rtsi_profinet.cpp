#include <rtsi_pnet/profinet.h>

#include "osal/osal.h"
#include "profinet/pnet.h"

// #include <thread>
// #include <mutex>

#include <pthread.h>

#define PNET_ETH_NAME "eth1"

#define PNET_ETH_NAME_x86 "ens33"

struct pnet_t {
  char *eth_name;    
  PNET_STATE state;         
  int error_code;           
  bool is_data_big_endian;  
  pthread_t th;         
  os_event_t *event;
  PNET_LOSE_MODULE_ACTION lose_module_action[PNET_MODULE_INDEX_MAX];  // Actions when a module is missing
  bool is_module_plug[PNET_MODULE_INDEX_MAX];                         // Whether the module is inserted
  pthread_mutex_t mutex;
  char station_name[128];
};

static struct pnet_t s_pnet = {0};

void rtsi_pnet_init(void)
{
#ifdef __x86_64__
  s_pnet.eth_name = PNET_ETH_NAME_x86;
#elif __arm__
  s_pnet.eth_name = PNET_ETH_NAME;
#endif
  s_pnet.is_data_big_endian = true;
  s_pnet.event = os_event_create();
  pthread_mutex_init(&s_pnet.mutex, NULL);
}

int main(int argc, char const *argv[])
{
  /* code */
  return 0;
}
