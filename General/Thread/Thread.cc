//
// Thread.cc
//

#include "Thread.hh"

using namespace MT;

void Thread::Start() {
   pthread_t thread;
   pthread_attr_t attr;
   ::pthread_attr_init(&attr);
   ::pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   ::pthread_create(&thread, (pthread_attr_t *)nullptr, &Thread::Run, this);
   ::pthread_attr_destroy(&attr);
} // Thread::Start()

void *Thread::Run(void *parameter) {
   Thread *thread = (Thread *)parameter;
   thread->Run();
   delete thread;
   return nullptr;
} // Thread::Run(parameter)
