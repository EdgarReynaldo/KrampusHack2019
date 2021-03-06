



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "networking/Client.hpp"
#include "networking/Server.hpp"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   set_log_file("NiloreaLog.txt");
   set_log_level(LOG_DEBUG);
   
   SendOutputToFile("EagleMasterServerLog.txt" , "KRAMPUS MASTER SERVER\n" , false);
   
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   const int sysflags = EAGLE_SYSTEM;
   if (!(sysflags & a5sys->Initialize(sysflags))) {
      return -1;
   }
   
   al_set_new_window_position(-10000 , -10000);
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("offscreen window" , 100 , 100 , EAGLE_WINDOWED);
   
   
   
   Server* master_server = new Server(a5sys , "8888" , 12);
   
   EagleEventHandler* q = a5sys->GetSystemQueue();//CreateEventHandler();
   
   Client* client = new Client(a5sys);
   
   bool success = client->Connect("192.168.56.1" , master_server->GetOurPORT());
//   bool success = client->Connect("10.0.2.15" , master_server->GetOurPORT());
//   bool success = client->Connect(GetLocalIP() , master_server->GetOurPORT());

   EagleInfo() << "Connect was " << (success?"successful":"not successful") << std::endl;
   
   q->ListenTo(master_server);
   q->ListenTo(client);

      
   a5sys->Rest(1.0);
   n_log(LOG_DEBUG , "Rested 1.0 seconds");

   BinStream bdat;
   BinStream bdat2;
   bdat << StringPrintF("Testing message to server");
   bdat2 << StringPrintF("Testing message to client");
   
   if (!client->SendPacket(bdat)) {
      EagleInfo() << "Failed to send packet 1." << std::endl;
   }
   if (!master_server->SendPacket(bdat2)) {
      EagleInfo() << "Failed to send packet 2." << std::endl;
   }
   
   bool quit = false;
   
   while (!quit) {
      EagleEvent e = a5sys->WaitForSystemEventAndUpdateState();
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
   }


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
