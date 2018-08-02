#ifndef PARSLEY_H
#define PARSLEY_H

#if defined(_WIN32)
#include "../libuv/win32/include/uv.h"
#else
#include <uv.h>
#endif

#include <stdlib.h>
#include <functional>
#include <map>


#define PARSLEY_NAMESPACE_BEGIN namespace Parsley {
#define PARSLEY_NAMESPACE_END }


PARSLEY_NAMESPACE_BEGIN

enum Mode {
  AsyncMode = 0,
  SyncMode = 1
};
using Buffer = uv_buf_t;

class LoopUtils;
class Loop;
template <typename UvHandle, typename PHandle>
class PObject;

template<class T, typename Ret, typename... Args>
struct FunctionPointer {
  typedef Ret(T::*F)(Args...);
  FunctionPointer(T* t, F f) {
    obj = t;
    fp = f;
  }
  T* obj;
  F fp;
  int call(Args... args) {
      return (obj->*fp)(args...);
  }
};

template<class T, typename Ret, typename... Args>
static FunctionPointer<T, Ret, Args...> bind(T *t, Ret(T::*f)(Args...))
{
  FunctionPointer<T, Ret, Args...> fp(t, f);
  return fp;
}

class LoopUtils
{
public:
  static Loop *defaultLoop();

private:
  static Loop default_loop;
};

class Loop
    : public LoopUtils
{
public:
  Loop();
  Loop(uv_loop_t* l);

  int run(const uv_run_mode &mode);
  void close();
  int tryClose();
  uv_loop_t* uvHandle();

private:
  uv_loop_t* loop;
};


template <typename UvHandle, typename PHandle>
class PObject
{
public:
  PObject(Loop *l);
  ~PObject();
  static void addInstance(UvHandle *uvHandle, PHandle *pHandle);
  static void removeInstance(UvHandle *uvHandle);
  static PHandle *getInstance(UvHandle *uvHandle);

  template <typename T>
  bool tryCall(T funct);
  UvHandle *getUvHandle();
  Loop *getLoop();

protected:
  UvHandle *uv_handle;
  Loop *loop;

private:
  static std::map<UvHandle*, PHandle*> instance_map;

};

template <typename UvHandle, typename PHandle>
std::map<UvHandle*, PHandle*> PObject<UvHandle, PHandle>::instance_map;

template<typename UvHandle, typename PHandle>
PObject<UvHandle, PHandle>::PObject(Loop *l)
  : loop(l)
{
  uv_handle = (UvHandle*) malloc(sizeof(UvHandle));
}

template<typename UvHandle, typename PHandle>
PObject<UvHandle, PHandle>::~PObject()
{
  removeInstance(uv_handle);
//  free(uv_handle); //! Do or Don't?
}

template<typename UvHandle, typename PHandle>
void PObject<UvHandle, PHandle>::addInstance(UvHandle *uvHandle, PHandle *pHandle)
{
  instance_map.insert({ uvHandle, pHandle });
}

template<typename UvHandle, typename PHandle>
void PObject<UvHandle, PHandle>::removeInstance(UvHandle *uvHandle)
{
  instance_map.erase(instance_map.find(uvHandle));
}

template<typename UvHandle, typename PHandle>
PHandle *PObject<UvHandle, PHandle>::getInstance(UvHandle *uvHandle)
{
  return instance_map[uvHandle];
}

template<typename UvHandle, typename PHandle>
template <typename T>
bool PObject<UvHandle, PHandle>::tryCall(T funct)
{
  if (!funct) {
      return false;
    }
  funct();
  return true;
}

template<typename UvHandle, typename PHandle>
UvHandle *PObject<UvHandle, PHandle>::getUvHandle()
{
  return uv_handle;
}

template<typename UvHandle, typename PHandle>
Loop *PObject<UvHandle, PHandle>::getLoop()
{
  return loop;
}









PARSLEY_NAMESPACE_END
#endif // PARSLEY_H
