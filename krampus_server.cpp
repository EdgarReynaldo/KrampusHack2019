


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
   
   set_log_file("NiloreaServerLog.txt");
   set_log_level(LOG_DEBUG);
   
   SendOutputToFile("EagleLog.txt" , "KRAMPUS SERVER\n" , false);
   
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   const int sysflags = EAGLE_FULL_SETUP;
   if (!(sysflags & a5sys->Initialize(sysflags))) {
      return -1;
   }
   
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("offscreen window" , sw , sh , EAGLE_WINDOWED);
   
   
   Server* server = new Server(a5sys , "8888" , 12);
   
   EagleEventHandler* q = a5sys->GetSystemQueue();//CreateEventHandler();
   
   q->ListenTo(server);

   
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
            if (e.type == EAGLE_EVENT_KEY_CHAR) {
               EagleInfo() << Indenter() << 
                  StringPrintF("CHAR event : allegro key (%d){%d} name : %s" , 
                               e.keyboard.keycode , e.keyboard.unicode , al_keycode_to_name(e.keyboard.keycode)) << std::endl;
            }
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
            std::string message = (const char*)e.network->data;
            clog.AddEntry(message);
            BinStream dat;
            dat << message;
            server->SendPacket(dat);
         }
         gui.HandleEvent(e);
         if (e.type == EAGLE_EVENT_TIMER) {
            gui.Update(e.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         if (IsNetworkEvent(e)) {
            delete e.network;
         }
      } while (!a5sys->UpToDate());
   }

   delete server;
   
   return 0;
}






























