/**
   CREDITS, ATTRIBUTION, ETC.

   GetLocalWin32IP uses code taken from https://cboard.cprogramming.com/c-programming/149613-ip-address-using-getaddrinfo.html

   GetLocalLinuxIP uses code taken from https://www.includehelp.com/c-programs/get-ip-address-in-linux.aspx
*/


#define _WIN32_WINNT 0x0600

#include "Server.hpp"
#include "Eagle.hpp"
#undef CreateMutex

#ifdef EAGLE_WIN32

   std::string GetLocalWin32IP();

   #include <ws2tcpip.h>

   #include <cstdio>
   #include <unistd.h>
   #include <cerrno>
   #include <cstring>
//   #include <netdb.h>
   #include <sys/types.h>
//   #include <sys/socket.h>
//   #include <netinet/in.h>
//   #include <arpa/inet.h>
#endif

#ifdef EAGLE_LINUX

   std::string GetLocalLinuxIP();

   #include <cstdio>
   #include <unistd.h>
   #include <cstring>
    
   #include <sys/socket.h>
   #include <sys/ioctl.h>
   #include <netinet/in.h>
   #include <net/if.h>
   #include <arpa/inet.h>

#endif // EAGLE_LINUX




#include "Client.hpp"



void* AcceptThread(EagleThread* thread , void* data) {
   Server* server = (Server*)data;
   EAGLE_ASSERT(server);
   EAGLE_ASSERT(thread);
   
   
   Client* client = new Client(server->System());
   
   while (!thread->ShouldStop()) {
      ThreadLockMutex(thread , server->mutex);
      if (server->NCONNECTIONS <= server->MAXNCONNECTIONS) {
         ThreadUnLockMutex(thread , server->mutex);
         if (client->Accept(1000 /* MS */ , server->GetNetwork())) {/// 250 MS timeout on accept
            ThreadLockMutex(thread , server->mutex);
            server->clients.AddClient(client);
            client = new Client(server->System());
            server->ListenTo(client);
            ThreadUnLockMutex(thread , server->mutex);
         }
      }
      else {
         ThreadUnLockMutex(thread , server->mutex);
      }
      int STATE = 0;
      int ENGINE_STATE = 0;
      if (netw_get_state(server->GetNetwork() , &STATE , &ENGINE_STATE)) {
         EagleInfo() << StringPrintF("Server state : %d , engine state : %d" , STATE , ENGINE_STATE) << std::endl;
         if (STATE != NETW_RUN) {
            EagleCritical() << "Network down!" << std::endl;
            return 0;
         }
//         if (ENGINE_STATE != NETW_THR_ENGINE_STARTED) {
//            EagleError() << "Network thread engine stopped!" << std::endl;
//            return 0;
//         }
      }
   }
   delete client;
   
   return data;
}



/// ------------------------     ClientList     -----------------------



void ClientList::AddClient(Client* client) {
   EAGLE_ASSERT(client);
   if (!client) {
      return;
   }
   
   std::map<std::string , Client*>::iterator it = client_map.find(client->GetNetwork()->link.ip);
   if (it == client_map.end()) {
      client_map[client->GetNetwork()->link.ip] = client;
   }
}



void ClientList::RemoveClient(Client* client) {
   EAGLE_ASSERT(client);
   if (!client) {
      return;
   }
   
   std::map<std::string , Client*>::iterator it = client_map.find(client->GetNetwork()->link.ip);
   if (it != client_map.end()) {
      delete it->second;
      client_map.erase(it);
   }
}



Client* ClientList::GetClient(std::string IP) {
   std::map<std::string , Client*>::iterator it = client_map.find(IP);
   if (it != client_map.end()) {
      return it->second;
   }
   return 0;
}



unsigned int ClientList::NumClients() {return client_map.size();}



/// ------------------------     Server     -----------------------



void Server::RespondToEvent(EagleEvent e , EagleThread* thread) {
   std::string evname = EagleEventName(e.type);
   if (e.type != EAGLE_EVENT_NETWORK_RECV_MSG) {
      return;
   }
   Client* client = dynamic_cast<Client*>(e.network->srcNETWORK);
   EagleInfo() << StringPrintF("Received message %s from client at %s :\n%s" ,
                                evname.c_str() , client->net->link.ip , (const char*)e.network-> data) << std::endl;
   EmitEvent(e,thread);
}



Server::Server(EagleSystem* esys , std::string PORT , unsigned int NUMCONNECTIONS) :
      Network(esys),
      MAXNCONNECTIONS(NUMCONNECTIONS),
      NCONNECTIONS(0),
      SERVER_LISTENING(false),
      SERVER_RUNNING(false),
      SERVER_THREADED(false),
      mutex(0),
      accept_thread(0),
      clients()
{
   EAGLE_ASSERT(esys);
   mutex = esys->CreateMutex("Server mutex" , false , false);
   EAGLE_ASSERT(mutex && mutex->Valid());

//   recv_thread = sys->CreateThread("ServerThread" , ReceiverThread , this);
   accept_thread = sys->CreateThread("AcceptThread" , AcceptThread , this);
   if (!(recv_thread && accept_thread && recv_thread->Valid() && accept_thread->Valid())) {
   if (!(accept_thread && accept_thread->Valid()) {
//      EAGLE_ASSERT(recv_thread && recv_thread->Valid());
      EAGLE_ASSERT(accept_thread && accept_thread->Valid());
      return;
   }
   
   /// int netw_make_listening( NETWORK **netw, char *addr, char *port, int nbpending, int ip_version );

   SERVER_LISTENING = netw_make_listening(&net , NULL , &PORT[0] , NUMCONNECTIONS , NETWORK_IPV4);
   EagleInfo() << "Server is " << (SERVER_LISTENING?"listening":"not listening") << std::endl;
   if (SERVER_LISTENING) {
      SERVER_RUNNING = true;///netw_start_thr_engine(net);
      EagleInfo() << "Server is " << (SERVER_RUNNING?"running":"not running") << std::endl;
      if (SERVER_RUNNING) {
         recv_thread->Start();
         accept_thread->Start();
         SERVER_THREADED = recv_thread->Running() && accept_thread->Running();
         EagleInfo() << "Server is " << (SERVER_THREADED?"threaded":"not threaded") << std::endl;
      }
   }
}



Server::~Server() {
   Shutdown();
   if (mutex) {
      if (sys) {
         sys->FreeMutex(mutex);
      }
   }
};
   
   

void Server::Shutdown() {
   
   if (accept_thread) {
      accept_thread->SignalToStop();
      accept_thread->Join();
      sys->FreeThread(accept_thread);
      accept_thread = 0;
   }
   
   if (SERVER_THREADED) {
      recv_thread->SignalToStop();
      recv_thread->Join();
      sys->FreeThread(recv_thread);
      recv_thread = 0;
      SERVER_THREADED = false;
   }
   ThreadLockMutex(0 , mutex);
   std::map<std::string , Client*>::iterator it = clients.client_map.begin();
   while (it != clients.client_map.end()) {
      delete it->second;
   }
   clients.client_map.clear();
   ThreadUnLockMutex(0 , mutex);

   if (net && SERVER_RUNNING) {
///      netw_stop_thr_engine(net);
      SERVER_RUNNING = false;
   }
   if (net && SERVER_LISTENING) {
      netw_close(&net);
      net = 0;
      SERVER_LISTENING = false;
   }
}



bool Server::Ready() {
   return SERVER_LISTENING && SERVER_RUNNING && SERVER_THREADED;
}











std::string GetLocalIP() {
#if defined EAGLE_WIN32
   return GetLocalWin32IP();
#elif defined EAGLE_LINUX
   return GetLocalLinuxIP();
#else
   #error GetLocalIP not defined for this platform!
   return std::string("127.0.0.1");
#endif // EAGLE_WIN32
}


#ifdef EAGLE_WIN32

std::string GetLocalWin32IP() {
   int status = -1; 
   struct addrinfo hints;
   struct addrinfo* p = 0; 
   struct addrinfo* servinfo = 0; 
   char ipstr[INET_ADDRSTRLEN] = {0};
   char hostname[1024] = {0};
    
   memset(&hints, 0, sizeof hints); 
   hints.ai_family   = AF_UNSPEC;    
   hints.ai_socktype = SOCK_STREAM;  
   hints.ai_flags    = AI_PASSIVE;  
 
   gethostname(hostname, 1023);

   if ((status = getaddrinfo(hostname, NULL, &hints, &servinfo)) == -1) { 
      EagleError() << StringPrintF("getaddrinfo error: %s\n", gai_strerror(status)); 
      return "";
   }       
 
   for (p=servinfo; p!=NULL; p=p->ai_next) { 
      struct in_addr  *addr;  
      if (p->ai_family == AF_INET) { 
         struct sockaddr_in *ipv = (struct sockaddr_in *)p->ai_addr; 
         addr = &(ipv->sin_addr);  
      }
      else { 
         struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr; 
         addr = (struct in_addr *) &(ipv6->sin6_addr); 
      }
      InetNtop(p->ai_family, addr, ipstr, sizeof ipstr); 
      EagleInfo() << StringPrintF("Address found: %s\n", ipstr);
   } 
   std::string ip = ipstr;
   freeaddrinfo(servinfo); 
   return ip;
}

#endif // EAGLE_WIN32



#ifdef EAGLE_LINUX
std::string GetLocalLinuxIP() {

    char ip_address[1024] = {0};
    int fd = -1;
    struct ifreq ifr = {0};
     
    /// AF_INET - to define network interface IPv4
    /// Creating soket for it
    fd = socket(AF_INET, SOCK_DGRAM, 0);
     
    /// AF_INET - to define IPv4 Address type
    ifr.ifr_addr.sa_family = AF_INET;
     
    ///eth0 - define the ifr_name - port name where network attached
    memcpy(ifr.ifr_name, "enp0s3", IFNAMSIZ-1);
     
    /// Accessing network interface information by passing address using ioctl
    ioctl(fd, SIOCGIFADDR, &ifr);
    /// closing fd
    close(fd);
     
    /// Extract IP Address
    std::string ip_str = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
     
    EagleInfo() << StringPrintF("System IP Address is: %s\n",ip_str.c_str());
    
    return ip_str;
}
#endif // EAGLE_LINUX
