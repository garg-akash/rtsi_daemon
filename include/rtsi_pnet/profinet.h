#ifndef __PROFINET_H__
#define __PROFINET_H__

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

enum PNET_STATE
{ 
  PNET_STATE_STOP, 
  PNET_STATE_RUN, 
  PNET_STATE_CONNECT, 
  PNET_STATE_ERROR, 
  PNET_STATE_START 
};

enum PNET_LOSE_MODULE_ACTION
{
  PNET_LOSE_MODULE_ACTION_IGNORE,
  PNET_LOSE_MODULE_ACTION_PAUSE,
  PNET_LOSE_MODULE_ACTION_STOP  
};

void rtsi_pnet_init() ;

void rtsi_pnet_uninit() ;


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif