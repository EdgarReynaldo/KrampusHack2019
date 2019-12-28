



#ifndef Client_HPP
#define Client_HPP

#include <string>

#include "Network.hpp"
#include "NetworkThread.hpp"



class Server;

class Client : public Network {
   std::string destIP;
   std::string destPORT;
   bool CLIENT_CONNECTED;
   bool CLIENT_RUNNING;
   bool CLIENT_THREADED;
public :
   
   Client(EagleSystem* esys);
   virtual ~Client();

   void Disconnect();
   bool Connect(const Server* serv);
   bool Connect(std::string IP , std::string PORT);

   bool Ready() override;
};





#endif // Client_HPP
