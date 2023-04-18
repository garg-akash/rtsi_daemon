#ifndef __ELI_LOG_DEFINE_H__
#define __ELI_LOG_DEFINE_H__

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

enum LOG_LEVEL {
  LOG_NONE = -1,
  LOG_ERROR = 0,
  LOG_WARNING = 1,
  LOG_INFO = 2,
  LOG_DEBUG = 3
};

#define INFO_LOG(format, args...) eli_log_info(__func__, __LINE__, false, NULL, LOG_INFO, format, ##args)
#define ERROR_LOG(format, args...) eli_log_info(__func__, __LINE__, false, NULL, LOG_ERROR, format, ##args)
#define WARNING_LOG(format, args...) eli_log_info(__func__, __LINE__, false, NULL, LOG_WARNING, format, ##args)
#define DEBUG_LOG(format, args...) eli_log_info(__func__, __LINE__, false, NULL, LOG_DEBUG, format, ##args)

void eli_log_info(const char *func_name, int line, bool debug, const char *catalog, LOG_LEVEL level, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif