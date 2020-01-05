



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
   EagleThread* recv_thread;
   
   
   Network(EagleSystem* esys);

public :
   
   virtual ~Network() {Close();}
   
   void Close();
   
   NETWORK* GetNetwork() {return net;}
   std::string GetOurIP();
   std::string GetOurPORT();
   
   virtual bool Ready() {return false;}
   
   bool SendPacket(const BinStream& bin);
   bool SendPacket(const void* data , unsigned int SZ);
   
   friend void* ReceiverThread(EagleThread* thread , void* data);
};


void* ReceiverThread(EagleThread* thread , void* data);



#endif // Network_HPP
