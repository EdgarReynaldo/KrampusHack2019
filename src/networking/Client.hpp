



#ifndef Client_HPP
#define Client_HPP

struct NETWORK;

#include <string>

class Server;

class Client {
   NETWORK* clinet;
   std::string destIP;
   std::string destPORT;
   bool CLIENT_CONNECTED;
   bool CLIENT_RUNNING;
public :
   
   Client();
   virtual ~Client();
   void Disconnect();
   bool Connect(const Server* serv);
   bool Connect(std::string IP , std::string PORT);

};





#endif // Client_HPP
