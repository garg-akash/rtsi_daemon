
#ifndef __PROTECTIVE_STOP_ALARM_H__
#define __PROTECTIVE_STOP_ALARM_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void register_pn_protective_stop_alarm(void);

void set_pnet_io_lost(bool is_lost, bool is_paused);
void set_pnet_reg1_lost(bool is_lost, bool is_paused);
void set_pnet_reg2_lost(bool is_lost, bool is_paused);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __PROTECTIVE_STOP_ALARM_H__
