


#define CINTERFACE

#include "Client.hpp"
#include "Server.hpp"
#include "Eagle.hpp"
#include "nilorea.h"



bool Client::Adopt(NETWORK* newnet) {
   net = newnet;
   if (CLIENT_CONNECTED) {
      CLIENT_RUNNING = netw_start_thr_engine(net);
      EagleInfo() << "Client is " << (CLIENT_RUNNING?"running.":"not running.") << std::endl;
      if (CLIENT_RUNNING) {
         recv_thread = sys->CreateThread("ClientThread" , ReceiverThread , this);
         CLIENT_THREADED = recv_thread && recv_thread->Valid();
         if (CLIENT_THREADED) {
            recv_thread->Start();
            CLIENT_THREADED = CLIENT_THREADED && recv_thread->Running();
         }
         EagleInfo() << "Client is " << (CLIENT_THREADED?"threaded":"not threaded") << std::endl;
      }
   }
   return Ready();
}



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
   Network::Close();
   CLIENT_THREADED = false;
   CLIENT_RUNNING = false;
   CLIENT_CONNECTED = false;
}



bool Client::Connect(std::string IP , std::string PORT) {
   Disconnect();
///int netw_connect( NETWORK **netw, char *host, char *port, int ip_version );
///int netw_connect_ex( &(*netw), host, port, 0, 0, 0, 0, 0, ip_version );

   NETWORK* newnet = 0;
   CLIENT_CONNECTED = netw_connect_ex(&newnet , &IP[0] , &PORT[0] , 0 , 1 , 1 , 1 , 1 , NETWORK_IPV4);
   if (!CLIENT_CONNECTED) {
      EagleInfo() << "Client failed to connect." << std::endl;
      return false;
   }
   EagleInfo() << StringPrintF("Client is connected to %s on port %s\n" , IP.c_str() , PORT.c_str());

   return Adopt(newnet);
}



bool Client::Accept(unsigned int timeout_msecs , NETWORK* network) {
   Disconnect();
   
   EAGLE_ASSERT(network);
   if (!network) {
      return false;
   }

   int ret = 0;
   NETWORK* newnet = netw_accept_from_ex(network , 0 , 0 , 0 , 0 , 0 , timeout_msecs , &ret);
   if (!newnet) {
      if (!(ret & ETIMEDOUT)) {
         EagleInfo() << StringPrintF("netw_accept_from_ex failed with return value %d" , ret) << std::endl;
      }
      return false;
   }
   EagleInfo() << StringPrintF("Accepted client connection from %s" , newnet->link.ip) << std::endl;
   CLIENT_CONNECTED = true;
   
   return Adopt(newnet);
}



bool Client::Ready() {
   return CLIENT_CONNECTED && CLIENT_RUNNING && CLIENT_THREADED;
}

