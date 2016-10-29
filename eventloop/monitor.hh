
#ifndef __MONITOR_HH__
#define __MONITOR_HH__

#include <stdint.h>

struct status_monitor
{
  uint64_t _events;
  uint64_t _multi_events;
  uint64_t _errors;
};

extern status_monitor _status;

#endif/*__MONITOR_HH__*/
