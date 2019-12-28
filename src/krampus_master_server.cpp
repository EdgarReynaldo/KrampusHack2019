



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "networking/Client.hpp"
#include "networking/Server.hpp"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   const int sysflags = EAGLE_SYSTEM;
   if (!(sysflags & a5sys->Initialize(sysflags))) {
      return -1;
   }
   
   
   Server* master_server = Server::CreateServer(a5sys , "888" , 12);
   
   Client* client = new Client(a5sys);
   
   bool success = client->Connect(master_server);

   EagleInfo() << "Connect was " << (success?"successful":"not successful") << std::endl;
   
   delete client;
   
   delete master_server;
   
   return 0;
   

   /**
   
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   int ret = a5sys->Initialize(EAGLE_FULL_SETUP);
   if (ret != EAGLE_FULL_SETUP) {
      EagleInfo() << "Failed to initialize some subsystems. Attempting to continue." << std::endl;
   }
   
   */
   
}
