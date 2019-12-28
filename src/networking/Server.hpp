



#ifndef Server_HPP
#define Server_HPP

#define CINTERFACE
#include "Eagle.hpp"
#include "Nilorea.h"
#include <string>

#include "Network.hpp"

std::string GetLocalIP();/// Implementation defined




class Server : public Network {
   unsigned int NCONNECTIONS;
   std::string ourIP;
   std::string ourPORT;
   bool SERVER_LISTENING;
   bool SERVER_RUNNING;
   bool SERVER_THREADED;
   
private :
//   Server(const Server& s); {}///< Copying a server is not allowed
   Server(const Server& s) :
         Network(0),
         NCONNECTIONS(0),
         ourIP(""),
         ourPORT(""),
         SERVER_LISTENING(false),
         SERVER_RUNNING(false),
         SERVER_THREADED(false)
   {(void)s;}///< Copying a server is not allowed

protected :
   Server(EagleSystem* esys);
   
public :
   static Server* CreateServer(EagleSystem* sys , std::string PORT , unsigned int NCONNECTIONS);
   virtual ~Server();
   
   virtual void Shutdown();
   
   std::string GetIPStr() const {return ourIP;}
   std::string GetPORTStr() const {return ourPORT;}

   bool Ready() override;
   
};


#endif // Server_HPP



