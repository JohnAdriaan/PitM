//
// Service.cc
//

#include <netdb.h>
#include <string.h>

#include "Service.hh"

using namespace BSD;

static BSD::Ports ports;

static BSD::Services services;

const BSD::Ports &Service::Ports() {
   if (!ports.empty()) {
      return ports;
   } // if

   ::setservent(true); // Stay open
   for (;;) {
      ::servent *entry = ::getservent();
      if (entry==nullptr) {
         break;
      } // if

      BSD::Port port = ntohs(entry->s_port);
      const auto &p = ports.find(port);
      BSD::Services &services = p==ports.end() ?
                                ports[port] = BSD::Services() : // Need new one
                                p->second;                      // Use existing one
      Add(services, *entry);
   } // for
   ::endservent();
   return ports;
} // Service::Ports()

const BSD::Services &Service::Services() {
   if (!services.empty()) {
      return services;
   } // if

   ::setservent(true); // Stay open
   for (;;) {
      ::servent *entry = ::getservent();
      if (entry==nullptr) {
         break;
      } // if

      Add(services, *entry);
   } // for
   ::endservent();
   return services;
} // Service::Services()

void Service::Add(BSD::Services &services, const ::servent &entry) {
   bool tcp = ::strcmp(entry.s_proto,"tcp")==0;
   bool udp = ::strcmp(entry.s_proto,"udp")==0;
   bool ddp = ::strcmp(entry.s_proto,"ddp")==0;
   if (!tcp && !udp && !ddp) {
      return; // Unknown protocol
   } // if

   BSD::Port port = ntohs(entry.s_port);

#define isAlias (alias!=&entry.s_name) // Helper calculation
   for (const char * const *alias = &entry.s_name;
        alias!=nullptr && *alias!=nullptr;
        (isAlias ? ++alias : (alias=entry.s_aliases))) {
      String name = *alias;
      const auto &s = services.find(name);
      Service &service = s==services.end() ?
                         services[name] = Service(name,port,isAlias) : // Need new one
                         s->second;                                    // Use existing one
      if (service.port!=port) {
         continue; // Attempt to use already-defined name!
      } // if
      service.tcp = service.tcp || tcp;
      service.udp = service.udp || udp;
      service.ddp = service.ddp || ddp;
   } // for
} // Service::Add(Services, entry)

Service::Service() :
         name(),
         port(NoPort),
         alias(false),
         tcp(false),
         udp(false),
         ddp(false) {
} // Service::Service()

Service::Service(const String &name, BSD::Port port, bool alias) :
         name(name),
         port(port),
         alias(alias),
         tcp(false),
         udp(false),
         ddp(false) {
} // Service::Service(name, port, alias)
