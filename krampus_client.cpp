


#define NOGDI


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "networking/Client.hpp"
#include "networking/Server.hpp"

#include "eagle/ConsoleWidget.hpp"




int main(int argc , char** argv) {
   
   int sw = 800;
   int sh = 600;
   
   (void)argc;
   (void)argv;
   
   set_log_file("NiloreaClientLog.txt");
   set_log_level(LOG_DEBUG);
   
   SendOutputToFile("EagleClientLog.txt" , "KRAMPUS CLIENT\n" , false);
   
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   const int sysflags = EAGLE_FULL_SETUP;
   if (!(sysflags & a5sys->Initialize(sysflags))) {
      return -1;
   }
   
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("offscreen window" , sw , sh , EAGLE_WINDOWED);
   
   EagleEventHandler* q = a5sys->GetSystemQueue();//CreateEventHandler();
   
   Client* client = new Client(a5sys);
   
   std::string servip = "192.168.1.59";
   if (argc > 1) {
      servip = argv[1];
   }
   else {
      EagleInfo() << "No ip address specified. Can't connect." << std::endl;
      return -1;
   }
   EagleInfo() << StringPrintF("Connecting to %s " , servip.c_str()) << std::endl;

   bool success = client->Connect(servip.c_str() , "8888");

   EagleInfo() << "Connect was " << (success?"successful":"not successful") << std::endl;
   
   if (!success) {
      delete client;
      return -1;
   }
   
   q->ListenTo(client);

   
   EagleFont* fnt = win->LoadFont("Verdana.ttf" , -24);
   EAGLE_ASSERT(fnt && fnt->Valid());
   
   
   WidgetHandler gui(win , "GUI" , "NEMOGUI");
   
   gui.SetupBuffer(sw , sh , win);
   gui.SetWidgetArea(Rectangle(0,0,sw,sh) , false);
   
   RelativeLayout rlayout;
   
   rlayout.Resize(2);
   
   ConsoleEntry console;
   
   console.SetText("" , fnt);
   WidgetColorset& colors = *(console.GetWidgetColorset());
   colors[BGCOL] = EagleColor(0,64,64);
   colors[TXTCOL] = EagleColor(255,255,0);
   
   q->ListenTo(&console);
   
   ConsoleLog clog;
   clog.SetFont(fnt);
   
   rlayout.PlaceWidget(&console , 0 , LayoutRectangle(0.0 , 0.8 , 1.0 , 0.2));
   rlayout.PlaceWidget(&clog    , 1 , LayoutRectangle(0.0 , 0.0 , 1.0 , 0.8));
   
   gui.SetRootLayout(&rlayout);
   
   a5sys->GetSystemTimer()->Start();

   bool redraw = true;
   bool quit = false;
   
   while (!quit) {
         
      if (redraw) {
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
         
      do {
         EagleEvent e = a5sys->WaitForSystemEventAndUpdateState();
         if (e.type != EAGLE_EVENT_MOUSE_AXES && e.type != EAGLE_EVENT_TIMER) {
            EagleInfo() << EagleEventName(e.type) << std::endl;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
            break;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
            break;
         }
         if (e.type == EAGLE_EVENT_NETWORK_RECV_MSG) {
            clog.AddEntry((std::string)(const char*)e.network->data);
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ENTER) {
            BinStream data;
            data << console.GetText();
            console.SetText("");
            client->SendPacket(data);
         }
         else {
            gui.HandleEvent(e);
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            gui.Update(e.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         if (IsNetworkEvent(e)) {
            delete e.network;
         }
      } while (!a5sys->UpToDate());
   }
   
   delete client;
   
   return 0;
}






























