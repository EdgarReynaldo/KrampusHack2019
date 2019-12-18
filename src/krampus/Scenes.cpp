



#include "Scenes.hpp"
#include "../globals.hpp"
#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


Scene::Scene(double length , int xpos , int ypos , int width , int height) :
      duration(length),
      elapsed(-1.0),
      pct(0.0),
      inc(systimer->SPT()),
      dest(xpos,ypos,width,height)
{}



STATUS Scene::Run() {
   STATUS status = STATUS_OKAY;
   redraw = true;
   if (quit) {return STATUS_QUIT;}
   if (!Setup()) {
      return STATUS_QUIT;
   }
   while (!quit && (status == STATUS_OKAY)) {
      if (redraw) {
         Transform t = win->GetTransformer()->GetViewTransform();
      
         t.Scale(scrw/dest.W() , scrh/dest.H());
         t.Translate(dest.X() , dest.Y());
         
         win->GetTransformer()->PushViewTransform(t);
         Display();
         win->GetTransformer()->PopViewTransform();
         
         win->FlipDisplay();
         
         redraw = false;
      }
      do {
         EagleEvent e = a5sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
            break;
         }
         status = HandleEvent(e);
         if (status == STATUS_COMPLETE) {break;}
      } while (!a5sys->UpToDate());
   }
   Cleanup();
   if (quit) {
      return STATUS_QUIT;
   }
   return STATUS_COMPLETE;
}



/// --------------------------     INTRO      -------------------------------



Intro::Intro(double duration , int xpos , int ypos , int width , int height) :
      Scene(duration , xpos , ypos , width , height),
      tr_clip(Pos2D(width/2.0 , 0.0) , Pos2D(0.0 , 0.0) , Pos2D(0,height) , Pos2D(width,height)),
      tr_drop(Pos2D(0.0 , 0.0) , Pos2D(0.0 , -height)),
      tr_left(Pos2D(0.0 , 0.0) , Pos2D(-width/2.0 , 0.0)),
      tr_right(Pos2D(width/2.0 , 0.0) , Pos2D(width , 0.0)),
      pause_timer(3.0)
{}



bool Intro::Setup() {
   return true;
}



void Intro::Cleanup() {
   
}



STATUS Intro::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_TIMER) {
      if (pause_timer < 0.0) {
         if (elapsed >= 0.0) {
            elapsed += inc;
            pct = elapsed/duration;
            if (pct >= 1.0) {pct = 1.0;}
            tr_clip.Recalc(pct);
            tr_drop.Recalc(pct*pct);
            tr_left.Recalc(pct);
            tr_right.Recalc(pct);
            redraw = true;
         }
      }
      else {
         pause_timer -= inc;
      }
   }
   if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
      return STATUS_QUIT;
   }
   else if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
      return STATUS_COMPLETE;
   }
   else if (e.type == EAGLE_EVENT_KEY_DOWN) {
      if (elapsed < 0.0) {
         elapsed = 0.0;
         pause_timer = -1.0;
         redraw = true;
      }
      else {
         return STATUS_COMPLETE;
      }
   }
   else if (e.type == EAGLE_EVENT_KEY_CHAR) {
      if (e.keyboard.keycode == EAGLE_KEY_LEFT || e.keyboard.keycode == EAGLE_KEY_RIGHT) {
         elapsed += ((e.keyboard.keycode == EAGLE_KEY_LEFT)?-1:1)*inc;
         if (elapsed < 0.0) {elapsed = 0.0;}
         if (elapsed > duration) {elapsed = duration;}
         pct = elapsed/duration;
         redraw = true;
         pause_timer = 3.0;
      }
   }
      
   return STATUS_OKAY;
}



void Intro::Display() {
   win->DrawStretched(krampus_splash , Rectangle(0,0,dest.W() , dest.H()));
   win->DrawStretched(left_curtain , Rectangle(tr_left.cpt.X() , tr_left.cpt.Y() , dest.W()/2.0 , dest.H()));
   win->DrawStretched(right_curtain , Rectangle(tr_right.cpt.X() , tr_right.cpt.Y() , dest.W()/2.0 , dest.H()));
   win->DrawStretched(drop_curtain , Rectangle(tr_drop.cpt.X() , tr_drop.cpt.Y() , dest.W() , dest.H()));
}



/// --------------------------------      INTRO2       ---------------------------------------



Intro2::Intro2(double duration , int xpos , int ypos , int width , int height) :
      Scene(duration,xpos,ypos,width,height)
{}



bool Intro2::Setup() {
   elapsed = 0.0;
   return true;
}



void Intro2::Cleanup() {
   
}



STATUS Intro2::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_TIMER) {
      elapsed += inc;
      pct = elapsed/duration;
//      tr_bottom.Recalc(pct);
      if (pct > 1.0) {
         return STATUS_COMPLETE;
      }
      redraw = true;
   }
   if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
      return STATUS_QUIT;
   }
   if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
      return STATUS_COMPLETE;
   }
   return STATUS_OKAY;
}



void Intro2::Display() {
/**
   win->DrawStretched(stage , Rectangle(0,0,dest.W(),dest.H()));
   
   Transform t = win->GetTransformer()->GetViewTransform();
   t.Scale((double)dest.W()/krampus_splash->W() , (double)dest.H()/krampus_splash->H());
   win->GetTransformer()->PushViewTransform(t);
   
   win->DrawTinted(krampus_splash , 0 , 0 , EagleColor(1.0f,1.0f,1.0f,pct));
   
   win->GetTransformer()->PopViewTransform();
*/
   Transform t = win->GetTransformer()->GetViewTransform();
   t.Scale((double)dest.W()/krampus_splash->W() , (double)dest.H()/krampus_splash->H());
   win->GetTransformer()->PushViewTransform(t);
   
   win->Draw(krampus_splash , 0 , 0);
   win->DrawTinted(krampus_splash , 0 , 0 , EagleColor(1.0,0.0f,0.0f,pct*pct));
   
   win->GetTransformer()->PopViewTransform();
}



/// --------------------------------      INTRO2       ---------------------------------------



Intro3::Intro3(double duration , int xpos , int ypos , int width , int height) :
      Scene(duration,xpos,ypos,width,height),
      tr_clip(Pos2D(width/2 , 0) , Pos2D(0 , 0) , Pos2D(0,height) , Pos2D(width , height))
{}



bool Intro3::Setup() {
   elapsed = 0.0;
   return true;
}



void Intro3::Cleanup() {
   
}



STATUS Intro3::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_TIMER) {
      elapsed += inc;
      pct = elapsed/duration;
      if (pct > 1.0) {
         pct = 1.0;
      }
      tr_clip.Recalc(pct);
      redraw = true;
   }
   if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
      quit = true;
      return STATUS_QUIT;
   }
   if (e.type == EAGLE_EVENT_KEY_DOWN) {
      return STATUS_COMPLETE;
   }
   return STATUS_OKAY;
}



void Intro3::Display() {
   Transform t = win->GetTransformer()->GetViewTransform();
   t.Scale((double)dest.W()/krampus_splash->W() , (double)dest.H()/krampus_splash->H());
   win->GetTransformer()->PushViewTransform(t);
   
   win->DrawTinted(krampus_splash , 0 , 0 , EagleColor(1.0,0.0f,0.0f,1.0));
   
   win->GetTransformer()->PopViewTransform();
   
   Clipper clip(win->GetBackBuffer() , Rectangle(tr_clip.X() , tr_clip.Y() , tr_clip.W() , tr_clip.H()));
   
   win->DrawFilledRectangle(tr_clip.Rect() , EagleColor(255,255,255));
   win->DrawRectangle(tr_clip.Rect() , 10.0 , EagleColor(0,0,0));
   
   win->DrawImageFit(blitzen , Rectangle(0,0,dest.W(),dest.H()));
   
   win->DrawTextString(krampus_font_huge , "Blitzen's" , dest.W()/4 + 5 , dest.H()/2 + 5 , EagleColor(64,0,0) , HALIGN_CENTER , VALIGN_CENTER);
   win->DrawTextString(krampus_font_huge , "Betrayal" , 3*dest.W()/4 + 5 , dest.H()/2 + 5 , EagleColor(64,0,0) , HALIGN_CENTER , VALIGN_CENTER);
   win->DrawTextString(krampus_font_huge , "Blitzen's" , dest.W()/4 , dest.H()/2 , EagleColor(192,0,0) , HALIGN_CENTER , VALIGN_CENTER);
   win->DrawTextString(krampus_font_huge , "Betrayal" , 3*dest.W()/4 , dest.H()/2 , EagleColor(192,0,0) , HALIGN_CENTER , VALIGN_CENTER);
   
}



