



#ifndef Network_HPP
#define Network_HPP

#include "Eagle/Events.hpp"
#include "Eagle/Exception.hpp"

#include <string>

#include "../eagle/BinStream.hpp"



class EagleSystem;
class EagleThread;
struct NETWORK;



class Network : public EagleEventSource , public EagleObject {
protected :
   NETWORK* net;
   EagleSystem* sys;
   EagleThread* recv_thread;
   
   
   Network(EagleSystem* esys);

public :
   
   
   virtual ~Network() {}/// Does not close! Override!
   
   virtual void Close();
   
   NETWORK* GetNetwork() {return net;}
   virtual std::string GetOurIP();
   virtual std::string GetOurPORT();
   virtual std::string GetDestIP();
   virtual std::string GetDestPORT();
   
   virtual bool Ready() {return false;}
   
   virtual bool SendPacket(const BinStream& bin);
   virtual bool SendPacket(const void* data , unsigned int SZ);
   
   friend void* ReceiverThread(EagleThread* thread , void* data);
};


void* ReceiverThread(EagleThread* thread , void* data);



#endif // Network_HPP
