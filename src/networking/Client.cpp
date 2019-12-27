


#define CINTERFACE

#include "Client.hpp"
#include "Server.hpp"
#include "Eagle.hpp"
#include "Nilorea.h"



Client::Client() :
      clinet(0),
      destIP(""),
      destPORT("888"),
      CLIENT_CONNECTED(false),
      CLIENT_RUNNING(false)
{
   
}



Client::~Client() {
   Disconnect();
}



void Client::Disconnect() {
   if (CLIENT_RUNNING) {
      netw_stop_thr_engine(clinet);
      CLIENT_RUNNING = false;
   }
   if (CLIENT_CONNECTED) {
      netw_close(&clinet);
      clinet = 0;
      CLIENT_CONNECTED = false;
   }
}



bool Client::Connect(const Server* serv) {
   return Connect(serv->GetIPStr() , serv->GetPORTStr());
}



bool Client::Connect(std::string IP , std::string PORT) {
   Disconnect();
///int netw_connect( NETWORK **netw, char *host, char *port, int ip_version );
   CLIENT_CONNECTED = netw_connect(&clinet , &IP[0] , &PORT[0] , NETWORK_IPV4);
   if (CLIENT_CONNECTED) {
      EagleInfo() << StringPrintF("Client is connected to %s on port %s\n" , IP.c_str() , PORT.c_str());
      CLIENT_RUNNING = netw_start_thr_engine(clinet);
      EagleInfo() << "Client is " << (CLIENT_RUNNING?"running.":"not running.") << std::endl;
   }
   else {
      EagleInfo() << "Client is not connected." << std::endl;
   }
   return CLIENT_CONNECTED && CLIENT_RUNNING;
}




