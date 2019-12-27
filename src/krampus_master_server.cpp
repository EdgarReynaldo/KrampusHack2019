



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "networking/Client.hpp"
#include "networking/Server.hpp"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Server* master_server = new Server;
   
   Client* client = new Client;
   
   EagleInfo() << "Connect was " << (client->Connect(master_server)?"successful":"not successful") << std::endl;
   
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
