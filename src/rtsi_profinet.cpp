#include "rtsi_pnet/profinet.h"
#include "rtsi_pnet/eli_profinet.h"

#include "eli_common/eli_thread.h"

#include "osal/osal.h"
#include "p-net/pnet.h"

// #include <thread>
// #include <mutex>

#include <pthread.h>

#define PNET_ETH_NAME "eth1"

#define PNET_ETH_NAME_x86 "ens33"

#define PNET_EVENT_STOP BIT(1)
#define PNET_EVENT_START BIT(2)

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

static PNET_STATE pnet_station_get_state() { return s_pnet.state; }

static int pnet_station_set_state(PNET_STATE state) {
  LOCK_MUTEX(s_pnet.mutex);
  s_pnet.state = state;
  UNLOCK_MUTEX(s_pnet.mutex);
  return 0;
}

PNET_STATE eli_pnet_current_state() { return pnet_station_get_state(); }

PNET_LOSE_MODULE_ACTION eli_pnet_lose_module_action(PNET_MODULE module) { return s_pnet.lose_module_action[module]; }

bool eli_pnet_is_module_plug(PNET_MODULE module) { return s_pnet.is_module_plug[module]; }

void eli_pnet_set_lose_module_act(PNET_MODULE module, PNET_LOSE_MODULE_ACTION act) {
  LOCK_MUTEX(s_pnet.mutex);
  s_pnet.lose_module_action[module] = act;
  UNLOCK_MUTEX(s_pnet.mutex);
}

void eli_pnet_start(const char *station_name) {
  LOCK_MUTEX(s_pnet.mutex);
  memset(s_pnet.station_name, 0, sizeof(s_pnet.station_name));
  memcpy(s_pnet.station_name, station_name, strlen(station_name));
  UNLOCK_MUTEX(s_pnet.mutex);
  if(pnet_station_get_state() == PNET_STATE_STOP) {
      pnet_station_set_state(PNET_STATE_START);
      os_event_set(s_pnet.event, PNET_EVENT_START);
  }
}

void eli_pnet_pause(void) {
  if (pnet_station_get_state() != PNET_STATE_START) {
      os_event_set(s_pnet.event, PNET_EVENT_STOP);
  }
}

void eli_pnet_init(void)
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
