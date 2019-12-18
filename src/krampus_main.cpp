

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

//#include <dwmapi.h>


#include <cstdio>
#include <cmath>


#include "krampus/Scenes.hpp"
#include "Globals.hpp"


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

   EagleInfo() << "Scene 1" << std::endl;
   Intro intro1(5.0 , 0 , 0 , scrw , scrh);
   intro1.Run();
   
   EagleInfo() << "Scene 2" << std::endl;
   Intro2 intro2(5.0 , 0 , 0 , scrw , scrh);
   intro2.Run();
   
   EagleInfo() << "Scene 3" << std::endl;
   Intro3 intro3(5.0 , 0 , 0 , scrw , scrh);
   intro3.Run();
   
   Eagle::EagleLibrary::ShutdownEagle();

   return 0;
}



