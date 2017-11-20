//
// Thread.cc
//

#include "Thread.Attr.hh"

using namespace MT;

static Thread::Attr attr(Thread::Attr::Detached);

Thread::Thread() :
        thread(-1) {
} // Thread::Thread()

bool Thread::Start() {
   return ::pthread_create(&thread, attr, &Thread::Run, this)==0;
} // Thread::Start()

void *Thread::Run(void *parameter) {
   Thread *thread = (Thread *)parameter;
   return thread->Run();
} // Thread::Run(parameter)
