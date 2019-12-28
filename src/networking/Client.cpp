


#define CINTERFACE

#include "Client.hpp"
#include "Server.hpp"
#include "Eagle.hpp"
#include "Nilorea.h"



Client::Client(EagleSystem* esys) :
      Network(esys),
      destIP(""),
      destPORT(""),
      CLIENT_CONNECTED(false),
      CLIENT_RUNNING(false),
      CLIENT_THREADED(false)
{}



Client::~Client() {
   Disconnect();
}



void Client::Disconnect() {
   if (CLIENT_THREADED) {
      thread->SignalToStop();
      thread->Join();
      sys->FreeThread(thread);
      thread = 0;
      CLIENT_THREADED = false;
   }
   if (CLIENT_RUNNING) {
      netw_stop_thr_engine(net);
      CLIENT_RUNNING = false;
   }
   if (CLIENT_CONNECTED) {
      netw_close(&net);
      CLIENT_CONNECTED = false;
   }
}



bool Client::Connect(const Server* serv) {
   return Connect(serv->GetIPStr() , serv->GetPORTStr());
}



bool Client::Connect(std::string IP , std::string PORT) {
   Disconnect();
///int netw_connect( NETWORK **netw, char *host, char *port, int ip_version );
   CLIENT_CONNECTED = netw_connect(&net , &IP[0] , &PORT[0] , NETWORK_IPV4);
   if (CLIENT_CONNECTED) {
      EagleInfo() << StringPrintF("Client is connected to %s on port %s\n" , IP.c_str() , PORT.c_str());
      CLIENT_RUNNING = netw_start_thr_engine(net);
      EagleInfo() << "Client is " << (CLIENT_RUNNING?"running.":"not running.") << std::endl;
      if (CLIENT_RUNNING) {
         thread = sys->CreateThread("ClientThread" , NetworkThread , this);
         CLIENT_THREADED = thread && thread->Valid();
         if (CLIENT_THREADED) {
            thread->Start();
            CLIENT_THREADED = CLIENT_THREADED && thread->Running();
         }
         EagleInfo() << "Client is " << (CLIENT_THREADED?"threaded":"not threaded") << std::endl;
      }
   }
   else {
      EagleInfo() << "Client failed to connect." << std::endl;
   }
   return Ready();
}



bool Client::Ready() {
   return CLIENT_CONNECTED && CLIENT_RUNNING && CLIENT_THREADED;
}

