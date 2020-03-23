


#define NOGDI


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "networking/Client.hpp"
#include "networking/Server.hpp"

#include "eagle/ConsoleWidget.hpp"




int main(int argc , char** argv) {
   
   int sw = 1024;
   int sh = 768;
   
   (void)argc;
   (void)argv;
   
   set_log_file("NiloreaServerClientLog.txt");
   set_log_level(LOG_DEBUG);
   
   SendOutputToFile("EagleClientServerLog.txt" , "KRAMPUS SERVER AND CLIENT TEST\n" , false);
   
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

   Client* client = new Client(a5sys);
   
   std::string servip = "192.168.1.59";
   if (argc > 1) {
      servip = argv[1];
   }
   else {
      EagleInfo() << "No ip address specified. Can't connect." << std::endl;
      system("pause");
      return -1;
   }
   EagleInfo() << StringPrintF("Connecting to %s " , servip.c_str()) << std::endl;

   bool success = client->Connect(servip.c_str() , "8888");

   EagleInfo() << "Connect was " << (success?"successful":"not successful") << std::endl;
   
   if (!success) {
      delete client;
      delete server;
      return -11;
   }
   
   q->ListenTo(client);
   
   EagleFont* fnt = win->LoadFont("Verdana.ttf" , -24);
   EAGLE_ASSERT(fnt && fnt->Valid());
   
   WidgetHandler gui(win , "GUI" , "NEMOGUI");
   
   gui.SetupBuffer(sw , sh , win);
   gui.SetWidgetArea(Rectangle(0,0,sw,sh) , false);
   
   RelativeLayout rlayout;
   
   rlayout.Resize(4);
   
   ConsoleEntry console;
   ConsoleEntry sconsole;
   
   console.SetText("" , fnt);
   sconsole.SetText("" , fnt);

   WidgetColorset colors = *(console.GetWidgetColorset());
   colors[BGCOL] = EagleColor(0,0,127);
   colors[TXTCOL] = EagleColor(255,255,0);
   console.SetWidgetColorset(colors);
   
   WidgetColorset scolors = *(sconsole.GetWidgetColorset());
   scolors[BGCOL] = EagleColor(0,127,0);
   scolors[TXTCOL] = EagleColor(0,255,255);
   sconsole.SetWidgetColorset(scolors);


   q->ListenTo(&console);
   q->ListenTo(&sconsole);
   ConsoleLog clog;
   ConsoleLog slog;

   clog.SetFont(fnt);
   
   slog.SetFont(fnt);

   rlayout.PlaceWidget(&console , 0 , LayoutRectangle(0.0 , 0.8 , 0.5 , 0.2));
   rlayout.PlaceWidget(&clog    , 1 , LayoutRectangle(0.0 , 0.0 , 0.5 , 0.8));
   rlayout.PlaceWidget(&sconsole , 2 , LayoutRectangle(0.5 , 0.8 , 0.5 , 0.2));
   rlayout.PlaceWidget(&slog    , 3 , LayoutRectangle(0.5 , 0.0 , 0.5 , 0.8));
   
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
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ENTER) {
            BinStream data;
            Network* pclient = (console.Flags().FlagOn(HASFOCUS)?(Network*)client:(Network*)server);
            ConsoleEntry* pconsole = (console.Flags().FlagOn(HASFOCUS)?&console:&sconsole);
            data << pconsole->GetText();
            pconsole->SetText("");
            pclient->SendPacket(data);
         }
         if (e.type == EAGLE_EVENT_NETWORK_RECV_MSG) {
            if (e.network->srcNETWORK == server) {
               std::string message = StringPrintF("%1.8lf '%s' received." , (double)ProgramTime::Elapsed() , (const char*)e.network->data);
               slog.AddEntry(message);
               BinStream dat;
               dat << message;
               server->SendPacket(dat);
            }
            if (e.network->srcNETWORK == client) {
               clog.AddEntry((std::string)(const char*)e.network->data);
               redraw = true;
            }
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



//https://pastebin.com/3ypk0AwS


























