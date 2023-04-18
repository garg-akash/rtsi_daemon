#include "rtsi_pnet/profinet.h"
#include "rtsi_pnet/eli_profinet.h"
#include "rtsi_pnet/profinet_module.h"

#include "eli_common/eli_thread.h"
#include "eli_common/eli_sys_utils.h"
// #include "eli_log/eli_log_define.h"

#include "osal/osal.h"
#include "p-net/pnet.h"

// #include <thread>
// #include <mutex>

#include <iostream>
#include <pthread.h>
#include <unistd.h>

#define PNET_ETH_NAME "eth1"

#define PNET_ETH_NAME_x86 "ens33"

#define PNET_FORBIDDEN_IP "6.0.0.9"

#define PNET_EVENT_STOP BIT(1)
#define PNET_EVENT_START BIT(2)

struct pnet_t {
  char *eth_name;    
  PNET_STATE state;         
  int error_code;           
  bool is_data_big_endian;  
  pthread_t thread;         
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

void eli_pnet_pause() {
  if (pnet_station_get_state() != PNET_STATE_START) {
      os_event_set(s_pnet.event, PNET_EVENT_STOP);
  }
}

static void *pnet_thread(void *arg);

void eli_pnet_init()
{
#ifdef __x86_64__
  s_pnet.eth_name = PNET_ETH_NAME_x86;
#elif __arm__
  s_pnet.eth_name = PNET_ETH_NAME;
#endif
  s_pnet.is_data_big_endian = true;
  s_pnet.event = os_event_create();
  pthread_mutex_init(&s_pnet.mutex, NULL);
  pthread_create(&s_pnet.thread, NULL, pnet_thread, NULL);
}

void eli_pnet_uninit()
{
  pthread_cancel(s_pnet.thread);
  pthread_join(s_pnet.thread, NULL);
}

static void pnet_recvdata_change_callback(STATION_MODULE_TYPE nModuleID_, u32 nOffset_, u32 nLen_, const byte *pData_)
{
  pnet_module_recvdata(nModuleID_, nOffset_, nLen_, pData_, s_pnet.is_data_big_endian);
}

static int pnet_station_init()
{
  LOCK_MUTEX(s_pnet.mutex);
  if(s_pnet.eth_name == NULL)
    s_pnet.eth_name = PNET_ETH_NAME;
  
  char eth_ip[32] = {0};
  sys_utils_getip(s_pnet.eth_name, eth_ip);
  if(!strcmp(eth_ip, PNET_FORBIDDEN_IP))
  {
    // TODO : ERROR_LOG
    UNLOCK_MUTEX(s_pnet.mutex);
    return -1;
  }

  if(!Station_GetIsInit())
  {
    if(Station_Init(s_pnet.eth_name, s_pnet.station_name))
      std::cout << "OK";
      // INFO_LOG("Profinet init successful, eth : %s", s_pnet.eth_name);
    else
    {
      // TODO : ERROR_LOG
      UNLOCK_MUTEX(s_pnet.mutex);
      return -1;
    }
  }
  else
    // TODO : INFO_LOG

  if(!Station_SetRevBufferChangeHook(pnet_recvdata_change_callback))
  {
    // TODO
    UNLOCK_MUTEX(s_pnet.mutex);
    return -1;
  }

  UNLOCK_MUTEX(s_pnet.mutex);
  sleep(5); // Without this delay, it will collapse when switching switches frequently
  return 0;
}

static int pnet_station_uninit()
{
  if(Station_GetIsInit())
  {
    if(!Station_UnInit())
    {
      //TODO: ERROR_LOG
      return -1;
    }
  }

  s_pnet.is_module_plug[PNET_MODULE_STATE] = false;
  s_pnet.is_module_plug[PNET_MODULE_IO] = false;
  s_pnet.is_module_plug[PNET_MODULE_JOINTS] = false;
  s_pnet.is_module_plug[PNET_MODULE_TCP] = false;
  s_pnet.is_module_plug[PNET_MODULE_BIT] = false;
  s_pnet.is_module_plug[PNET_MODULE_INT] = false;
  s_pnet.is_module_plug[PNET_MODULE_FLOAT] = false;
  s_pnet.is_module_plug[PNET_MODULE_O_IO] = false;
  s_pnet.is_module_plug[PNET_MODULE_O_REG1] = false;
  s_pnet.is_module_plug[PNET_MODULE_O_REG2] = false;
  //TODO
  return 0;
}

static void pnet_loop()
{
  uint32_t event = 0;
  // INFO_LOG("Profinet wait connect");

}

static void pnet_run()
{
  if(pnet_station_init())
  {
    pnet_station_set_state(PNET_STATE_STOP);
    // TODO : ERROR_LOG
  }
  else
    pnet_station_set_state(PNET_STATE_RUN);

  // If initialization fails, the loop is not entered
  if(pnet_station_get_state() == PNET_STATE_RUN)
    pnet_loop();
  
  // If it is not in the stop state, it may have been initialized, so uninstall it
  if(pnet_station_get_state() != PNET_STATE_STOP)
  {
    if(pnet_station_uninit())
      // TODO : ERROR_LOG

    pnet_station_set_state(PNET_STATE_STOP);
  }
}

static void *pnet_thread(void *arg)
{
  uint32_t event = 0;
  pthread_setname_np(pthread_self(), "pnet-thread-app");
  while(true)
  {
    os_event_wait(s_pnet.event, PNET_EVENT_START | PNET_EVENT_STOP, &event, OS_WAIT_FOREVER);
    if(event == PNET_EVENT_START)
    {
      os_event_clr(s_pnet.event, PNET_EVENT_START);
      pnet_run();
    }
    else
      os_event_clr(s_pnet.event, PNET_EVENT_STOP);
  }

// TODO: Info_log
return NULL;
}