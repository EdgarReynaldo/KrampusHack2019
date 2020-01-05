



#ifndef Client_HPP
#define Client_HPP

#include <string>

#include "Network.hpp"


class Client : public Network {
   std::string destIP;
   std::string destPORT;

   bool CLIENT_CONNECTED;
   bool CLIENT_RUNNING;
   bool CLIENT_THREADED;
   
   friend class Server;
   bool Adopt(NETWORK* newnet);

public :
   
   Client(EagleSystem* esys);
   virtual ~Client();

   void Disconnect();

   bool Connect(std::string IP , std::string PORT);
   
   bool Accept(unsigned int timeout_msec , NETWORK* network);
   
   bool Ready() override;
};





#endif // Client_HPP
