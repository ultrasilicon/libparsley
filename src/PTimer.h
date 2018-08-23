#ifndef TIMER_H
#define TIMER_H

/*
 * Maintainance:
 * 1. Remove any duplicated initializations or allocations
 * 2. Check API usage, correct wrong usages.
 * 3. Correct naming convention.
 */

#include <functional>

#include "Parsley.h"

//#include "Log.h"


PARSLEY_NAMESPACE_BEGIN
class TimerUtils;
class Timer;

class TimerUtils
    : public PObject<uv_timer_t, Timer>
{
public:
  TimerUtils(Loop *l) : PObject(l){}
protected:
  static void timeoutCb(uv_timer_t *handle);

};

class Timer
    : public TimerUtils
{
public:
  Timer(const uint64_t &timeout, const uint64_t &repeat, Loop *l);
  Timer(Loop *l);

  void callTimeout();
  Callback<void, Timer*> onTimedOut;

  int start(const uint64_t &timeout, const uint64_t &repeat);
  bool start();
  void stop();


private:
  uint64_t delay = 0;
  uint64_t interval = 0;
};


PARSLEY_NAMESPACE_END
#endif // TIMER_H
