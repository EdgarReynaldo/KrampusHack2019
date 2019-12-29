



#ifndef Network_HPP
#define Network_HPP

#include "Eagle/Events.hpp"
#include "Eagle/Exception.hpp"
#include <string>
#include "../eagle/BinStream.hpp"



class EagleSystem;
struct NETWORK;
class EagleThread;



class Network : public EagleEventSource {
protected :
   NETWORK* net;
   EagleSystem* sys;
   EagleThread* thread;
   std::string ourIP;
   std::string ourPORT;
   
   
   Network(EagleSystem* esys);

public :
   
   NETWORK* GetNetwork() {return net;}
   std::string GetOurIP() {return ourIP;}
   std::string GetOurPORT() {return ourPORT;}
   
   virtual bool Ready() {return false;}
   
   bool SendPacket(const BinStream& bin);
   bool SendPacket(const void* data , unsigned int SZ);
   
   friend void* NetworkThread(EagleThread* thread , void* data);
};


void* NetworkThread(EagleThread* thread , void* data);



#endif // Network_HPP
