



#ifndef Server_HPP
#define Server_HPP

#define CINTERFACE
#include "Eagle.hpp"
#include "Eagle/BinStream.hpp"
#include "nilorea.h"
#include <string>

#include "Network.hpp"

std::string GetLocalIP();/// Implementation defined


void* AcceptThread(EagleThread* thread , void* data);


class Client;




class ClientList {

   static CLIENTID GetNewClientID() {
      static CLIENTID cid = 0;
      return cid++;
   }

public :
   std::map<CLIENTID , Client*> client_map;
   
   void AddClient(Client* client);
   void RemoveClient(Client* client);
   Client* GetClient(CLIENTID cid);

   unsigned int NumClients();
};



class Server : public Network , public EagleEventListener {
   unsigned int MAXNCONNECTIONS;
   unsigned int NCONNECTIONS;

   bool SERVER_LISTENING;
   bool SERVER_THREADED;
   
   EagleMutex* mutex;
   EagleThread* accept_thread;
   ClientList clients;
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   void AddClient(Client* client);
   void RemoveClient(CLIENTID cid);

private :
//   Server(const Server& s); {}///< Copying a server is not allowed
   Server(const Server& s) :
         Network(s.sys , "Bad server" , "Bad copy"),
         MAXNCONNECTIONS(0),
         NCONNECTIONS(0),
         SERVER_LISTENING(false),
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
   
   
   bool BroadcastPacket(const BinStream& bin);
   bool BroadcastPacket(const void* data , unsigned int SZ);

public :
   Server(EagleSystem* sys , std::string PORT , unsigned int NCONNECTIONS);
   virtual ~Server();
   
   void Shutdown();
   
   EagleSystem* System() {return sys;}
   
   bool Ready() override;
   
   virtual bool SendPacket(const BinStream& bin);
   virtual bool SendPacket(const void* data , unsigned int SZ);

   bool SendPacket(CLIENTID cid , const BinStream& bin);
   bool SendPacket(CLIENTID cid , const void* data , unsigned int SZ);

   
   unsigned int ClientCount() {return clients.NumClients();}
   
   friend void* AcceptThread(EagleThread* thread , void* data);
};


#endif // Server_HPP



