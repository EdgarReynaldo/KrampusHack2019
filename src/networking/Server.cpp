/**
   CREDITS, ATTRIBUTION, ETC.

   GetLocalWin32IP uses code taken from https://cboard.cprogramming.com/c-programming/149613-ip-address-using-getaddrinfo.html

   GetLocalLinuxIP uses code taken from https://www.includehelp.com/c-programs/get-ip-address-in-linux.aspx
*/


#define _WIN32_WINNT 0x0600

#include "Server.hpp"
#include "Eagle.hpp"


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



Server::Server() :
      servnet(0),
      ourIP(""),
      ourPORT("888"),
      SERVER_LISTENING(false),
      SERVER_RUNNING(false)
{
   /// int netw_make_listening( NETWORK **netw, char *addr, char *port, int nbpending, int ip_version );

   SERVER_LISTENING = netw_make_listening(&servnet , NULL , (char*)ourPORT.c_str() , 12 , NETWORK_IPV4);
   EagleInfo() << "Server is " << (SERVER_LISTENING?"listening":"not listening") << std::endl;
   ourIP = GetLocalIP();
   SERVER_RUNNING = netw_start_thr_engine(servnet);
   EagleInfo() << "Server is " << (SERVER_RUNNING?"running":"not running") << std::endl;
}



Server::~Server() {
   if (servnet && SERVER_RUNNING) {
      netw_stop_thr_engine(servnet);
      SERVER_RUNNING = false;
   }
   if (servnet && SERVER_LISTENING) {
      netw_close(&servnet);
      servnet = 0;
      SERVER_LISTENING = false;
   }
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
    memcpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
     
    /// Accessing network interface information by passing address using ioctl
    ioctl(fd, SIOCGIFADDR, &ifr);
    /// closing fd
    close(fd);
     
    /// Extract IP Address
    std::string ip_str = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
     
    EagleInfo() << StringPrintF("System IP Address is: %s\n",ip_str.c_str());
    
    return ip_str;
}
#endif // EAGLE_LINUX