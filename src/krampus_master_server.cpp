



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "networking/Client.hpp"
#include "networking/Server.hpp"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   set_log_file("NiloreaLog.txt");
   set_log_level(LOG_DEBUG);
   
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   const int sysflags = EAGLE_SYSTEM;
   if (!(sysflags & a5sys->Initialize(sysflags))) {
      return -1;
   }
   
   
   Server* master_server = new Server(a5sys , "888" , 12);
   
   EagleEventHandler* q = a5sys->CreateEventHandler();
   
   Client* client = new Client(a5sys);
   
   bool success = client->Connect(GetLocalIP() , master_server->GetOurPORT());

   EagleInfo() << "Connect was " << (success?"successful":"not successful") << std::endl;
   
   q->ListenTo(master_server);
   q->ListenTo(client);

   BinStream bdat;
   BinStream bdat2;
   bdat << StringPrintF("Testing message to server");
   bdat2 << StringPrintF("Testing message to client");
   
   EAGLE_ASSERT(client->SendPacket(bdat));
   EAGLE_ASSERT(master_server->SendPacket(bdat2));
   
   do {
      EagleEvent e;
      q->WaitForEvent(MAIN_THREAD);
      EagleInfo() << EagleEventName(e.type) << std::endl;
      if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
         break;
      }
      if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         break;
      }
      if (e.type == EAGLE_EVENT_NETWORK_RECV_MSG) {
         if (e.network->srcNETWORK == master_server) {
            EagleInfo() << StringPrintF("Server received message :\n%s" , (const char*)e.network->data) << std::endl;
         }
         if (e.network->srcNETWORK == client) {
            EagleInfo() << StringPrintF("Client received message :\n%s" , (const char*)e.network->data) << std::endl;
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
