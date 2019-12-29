



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
   
   EagleEventHandler* q = a5sys->CreateEventHandler();
   
   Client* client = new Client(a5sys);
   
   bool success = client->Connect(master_server);

   EagleInfo() << "Connect was " << (success?"successful":"not successful") << std::endl;
   
   q->ListenTo(master_server);
   q->ListenTo(client);

   BinStream bdat;
   bdat << StringPrintF("Testing message to server");
   
   client->SendPacket(bdat);

   do {
      EagleEvent e;
      q->WaitForEvent(MAIN_THREAD);
      if (e.type == EAGLE_EVENT_NETWORK_RECV_MSG) {
         if (e.network->srcNETWORK == master_server) {
            EagleInfo() << StringPrintF("Server received message :\n%s" , (const char*)e.network->data) << std::endl;
         }
      }
      if (IsNetworkEvent(e)) {
         delete e.network;
      }
   } while (q->HasEvent());


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
