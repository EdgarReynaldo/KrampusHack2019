

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

//#include <dwmapi.h>


#include <cstdio>
#include <cmath>


#include "krampus/Scenes.hpp"
#include "globals.hpp"
#include "krampus/Menu.hpp"
#include "networking/Server.hpp"



int main(int argc , char** argv) {



   (void)argc;
   (void)argv;

   a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   if (EAGLE_FULL_SETUP != a5sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleInfo() << "Failed to completely setup allegro 5 system." << std::endl;
      EagleInfo() << "Continuing." << std::endl;
      /// return -1;
   }

   atexit(Eagle::EagleLibrary::ShutdownEagle);
   
   
   win = dynamic_cast<Allegro5GraphicsContext*>(a5sys->CreateGraphicsContext("Krampus19" , scrw , scrh , EAGLE_WINDOWED | EAGLE_OPENGL));
   
   EAGLE_ASSERT(win && win->Valid());

   systimer = a5sys->GetSystemTimer();
   
   stage = win->LoadImageFromFile("data/images/curtains/Stage.png");
   left_curtain = win->LoadImageFromFile("data/images/curtains/FullCurtainsLeft.jpg");
   right_curtain = win->LoadImageFromFile("data/images/curtains/FullCurtainsRight.jpg");
   drop_curtain = win->LoadImageFromFile("data/images/curtains/DropCurtains.png");

   krampus_splash = win->LoadImageFromFile("data/images/Krampus19.png");
   blitzen = win->LoadImageFromFile("data/images/Blitzen.jpg");

   krampus_font_huge = win->LoadFont("data/fonts/ChristmasOnCrack.ttf" , -200);
   krampus_font36 = win->LoadFont("data/fonts/ChristmasOnCrack.ttf" , -36);
   krampus_font24 = win->LoadFont("data/fonts/ChristmasOnCrack.ttf" , -24);
   
 
   EAGLE_ASSERT(stage->Valid() &&
                left_curtain->Valid() &&
                right_curtain->Valid() &&
                drop_curtain->Valid() &&
                krampus_splash->Valid() &&
                blitzen->Valid() &&
                krampus_font24->Valid() &&
                krampus_font36->Valid() &&
                krampus_font_huge->Valid()
                );


   systimer->Start();

//*1
   EagleInfo() << "Scene 1" << std::endl;
   Intro intro1(5.0 , 0 , 0 , scrw , scrh);
   intro1.Run();
   
   EagleInfo() << "Scene 2" << std::endl;
   Intro2 intro2(5.0 , 0 , 0 , scrw , scrh);
   intro2.Run();
   
   EagleInfo() << "Scene 3" << std::endl;
   Intro3 intro3(5.0 , 0 , 0 , scrw , scrh);
   intro3.Run();
   
   EagleInfo() << "Menu scene" << std::endl;
   MenuScene menu1(-1.0 , 0 , 0 , scrw , scrh);
   menu1.Run();
//   */
   /**
   TextIconButton btn(krampus_font_huge , "Press ME" , "TextIconButton" , "our button");
   WidgetHandler gui(win);
   gui.SetWidgetArea(Rectangle(0 , 0 , scrw , scrh) , false);
   gui.SetupBuffer(scrw , scrh , win);
   gui.AddWidget(&btn);
   btn.SetWidgetArea(Rectangle(scrw/4 , scrh/4 , scrw/2 , scrh/2) , false);
   EagleImage* btnimage = win->LoadImageFromFile("data/images/buttons/button.png");
   btn.SetImages(btnimage , btnimage , btnimage , btnimage);
   while (!quit) {
      if (redraw) {
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e;
         e = a5sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_TIMER) {redraw = true;}
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         int ret = gui.HandleEvent(e);
         (void)ret;
         if (gui.Flags().FlagOn(NEEDS_REDRAW)) {
            redraw = true;
         }
         while (gui.HasMessages()) {
            WidgetMsg msg = gui.TakeNextMessage();
            EagleInfo() << msg << std::endl;
         }
      } while (!a5sys->UpToDate());
   }
*/
   
   
   return 0;
}



