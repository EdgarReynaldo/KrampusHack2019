



#ifndef Network_HPP
#define Network_HPP

#include "Eagle/Events.hpp"
#include "Eagle/Exception.hpp"

#include <string>

#include "../eagle/BinStream.hpp"



class EagleSystem;
class EagleThread;
struct NETWORK;

typedef unsigned int CLIENTID;
extern const CLIENTID BADCLIENT;



class Network : public EagleEventSource , public EagleObject {
protected :
   NETWORK* net;
   EagleSystem* sys;
   EagleThread* recv_thread;
   
   
   Network(EagleSystem* esys , std::string classname = "Eagle Network" , std::string name = "Nemo");

   
   void ShutdownThread(EagleThread** thread);
   
public :
   
   
   virtual ~Network() {}/// Does not close! Override!
   
   virtual void Close();
   
   NETWORK* GetNetwork() {return net;}
   std::string GetOurIP();
   std::string GetOurPORT();
   
   virtual bool Ready() {return false;}
   
   virtual bool SendPacket(const BinStream& bin);
   virtual bool SendPacket(const void* data , unsigned int SZ);
   
   friend void* ReceiverThread(EagleThread* thread , void* data);
};


void* ReceiverThread(EagleThread* thread , void* data);



#endif // Network_HPP
