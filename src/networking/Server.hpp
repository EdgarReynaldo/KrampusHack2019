



#ifndef Server_HPP
#define Server_HPP

#define CINTERFACE
#include "Eagle.hpp"
#include "nilorea.h"
#include <string>

#include "Network.hpp"

std::string GetLocalIP();/// Implementation defined


void* AcceptThread(EagleThread* thread , void* data);


class Client;

class ClientList {
public :
   std::map<std::string , Client*> client_map;
   
   void AddClient(Client* client);
   void RemoveClient(Client* client);
   Client* GetClient(std::string IP);

   unsigned int NumClients();
};



class Server : public Network , public EagleEventListener {
   unsigned int MAXNCONNECTIONS;
   unsigned int NCONNECTIONS;

   bool SERVER_LISTENING;
   bool SERVER_RUNNING;
   bool SERVER_THREADED;
   
   EagleMutex* mutex;
   EagleThread* accept_thread;
   ClientList clients;
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

private :
//   Server(const Server& s); {}///< Copying a server is not allowed
   Server(const Server& s) :
         Network(0),
         MAXNCONNECTIONS(0),
         NCONNECTIONS(0),
         SERVER_LISTENING(false),
         SERVER_RUNNING(false),
         SERVER_THREADED(false),
         mutex(0),
         accept_thread(0),
         clients()
   {(void)s;}///< Copying a server is not allowed

   Server& operator=(const Server& s) {
      (void)s;
      return *this;
   }
   
protected :
   Server(EagleSystem* esys);
   
public :
   Server(EagleSystem* sys , std::string PORT , unsigned int NCONNECTIONS);
   virtual ~Server();
   
   void Shutdown();
   
   EagleSystem* System() {return sys;}

   bool Ready() override;
   
   friend void* AcceptThread(EagleThread* thread , void* data);
};


#endif // Server_HPP



