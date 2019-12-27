



#ifndef Server_HPP
#define Server_HPP

#define CINTERFACE
#include "Eagle.hpp"
#include "Nilorea.h"
#include <string>


std::string GetLocalIP();/// Implementation defined




class Server {
   NETWORK* servnet;
   std::string ourIP;
   std::string ourPORT;
   bool SERVER_LISTENING;
   bool SERVER_RUNNING;
   
//   std::string GetIP();
public :
   Server();
   virtual ~Server();

   std::string GetIPStr() const {return ourIP;}
   std::string GetPORTStr() const {return ourPORT;}
};


#endif // Server_HPP



