


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
   
   set_log_file("NiloreaLog.txt");
   set_log_level(LOG_DEBUG);
   
   SendOutputToFile("EagleLog.txt" , "KRAMPUS SERVER\n" , false);
   
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   const int sysflags = EAGLE_FULL_SETUP;
   if (!(sysflags & a5sys->Initialize(sysflags))) {
      return -1;
   }
   
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("offscreen window" , sw , sh , EAGLE_WINDOWED);
   
   
   Server* master_server = new Server(a5sys , "8888" , 12);
   
   EagleEventHandler* q = a5sys->GetSystemQueue();//CreateEventHandler();
   
   Client* client = new Client(a5sys);
   
   bool success = client->Connect("192.168.56.1" , master_server->GetOurPORT());

   EagleInfo() << "Connect was " << (success?"successful":"not successful") << std::endl;
   
   q->ListenTo(master_server);
   q->ListenTo(client);

   
   EagleFont* fnt = win->LoadFont("Verdana.ttf" , -32);
   EAGLE_ASSERT(fnt && fnt->Valid());
   
   
   WidgetHandler gui(win , "GUI" , "NEMOGUI");
   
   gui.SetupBuffer(sw , sh , win);
   gui.SetWidgetArea(Rectangle(0,0,sw,sh) , false);
   
   RelativeLayout rlayout;
   
   rlayout.Resize(1);
   
   EditText seltext;
   
   seltext.SetText("Hello, this is some\nmulti-line text\nso there\n" , fnt);

   rlayout.PlaceWidget(&seltext , 0 , LayoutRectangle(0.3 , 0.4 , 0.4 , 0.2));
   
   gui.SetRootLayout(&rlayout);
   
   a5sys->GetSystemTimer()->Start();

//   seltext.Refresh();
   
   EagleInfo() << seltext << std::endl;
      
   bool redraw = true;
   bool quit = false;
   
   while (!quit) {
         
      if (redraw) {
         win->Clear();
         int start = 0 , close = 0 , left = 0 , right = 0;
         seltext.GetSelection(&start , &close , &left , &right);
         int selLine = 0 , selPos = 0 , caretLine = 0 , caretPos = 0;
         seltext.GetCaretAttributes(&selLine , &selPos , &caretLine , &caretPos);
         
         gui.SetRedrawAllFlag();
         gui.Display(win , 0 , 0);
         
         win->DrawTextString(fnt , StringPrintF("Start = %d,%d , Close = %d,%d" , start , left , close , right) ,
                             10 , 10 , EagleColor(255,255,255));
         
         win->DrawTextString(fnt , StringPrintF("Caret = L%d P%d Selection = L%d P%d" , caretLine , caretPos , selLine , selPos),
                             10 , 50 , EagleColor(255,255,255));
         
         win->DrawTextString(fnt , StringPrintF("FOCUS = %s" , seltext.FlagOn(HASFOCUS)?"true":"false") ,
                             10 , sh - 80 , EagleColor(255,255,255)); 
         win->DrawTextString(fnt , StringPrintF("SHIFT = %s" , (input_key_held(EAGLE_KEY_ANY_SHIFT)?"true":"false")) ,
                             10 , sh - 40 , EagleColor(255,255,0));
         
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


   delete client;
   
   delete master_server;
   
   return 0;
}





























