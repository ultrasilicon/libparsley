#include "PAsync.h"

using namespace Parsley;

AsyncUtils::AsyncUtils(Loop *l)
  : PObject(l)
{
}

void AsyncUtils::executeCb(uv_async_t *r)
{
  getInstance(r)->onCalled.call();
}


Async::Async(Loop *l)
  : AsyncUtils(l)
{
  uv_async_init(m_loop->uvHandle(), m_uv_handle, executeCb);
  regInstance(m_uv_handle, this);
}

int Async::send()
{
  return uv_async_send(m_uv_handle);
}
