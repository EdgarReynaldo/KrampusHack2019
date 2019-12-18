



#include "Menu.hpp"



const char* menu_strs[NUM_MENU_OPTIONS] = {
   "Host a game",
   "Join a game",
   "Introduction",
   "Credits",
   "Quit"
};



const char* menu_keystrs[NUM_MENU_OPTIONS] {
   "H",
   "J",
   "I",
   "C",
   "Q"
};



int menu_keys[NUM_MENU_OPTIONS] = {
   EAGLE_KEY_H,
   EAGLE_KEY_J,
   EAGLE_KEY_I,
   EAGLE_KEY_C,
   EAGLE_KEY_Q
};



/// Private interface, override to define behavior
int Menu::PrivateHandleEvent(EagleEvent ee) {
   
   /// Menu cycles by pressing up or down
   if (ee.type == EAGLE_EVENT_KEY_DOWN) {
      if (ee.keyboard.keycode == EAGLE_KEY_UP || ee.keyboard.keycode == EAGLE_KEY_DOWN) {
         zscroll += (ee.keyboard.keycode == EAGLE_KEY_DOWN)-1:1;
         zscroll = zscroll % NUM_MENU_OPTIONS;
         return DIALOG_OKAY;
      }
      if (ee.keyboard.keycode == EAGLE_KEY_ENTER) {
         
      }
   }
   int ret = gui.HandleEvent(ee);
   
   /// 
   
   
   
}



int Menu::PrivateUpdate(double dt) {
   return gui.Update(dt);
}



void Menu::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   gui.Display(win,xpos,ypos);
}



/// Callbacks, overload if you need to
void Menu::OnAreaChanged() {
   gui.SetWidgetArea(GetWidgetArea() , false);
}



void Menu::OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v) {
   gui.SetAttribute(a,v);
}



void Menu::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   gui.SetWidgetFlags(Flags());
}



void Menu::OnColorChanged() {
   gui.SetWidgetColorset(this->WidgetColors());
}



void Menu::SetupMenuLayouts() {
   ClearMenuLayouts();
   for (int n = 0 ; n < NUM_MENU_OPTIONS ; ++n) {
      menu_layout.PlaceWidget(btns[n] , (n + zscroll)%NUM_MENU_OPTIONS);
      kmenu_layout.PlaceWidget(kbtns[n] , (n + zscroll)%NUM_MENU_OPTIONS);
   }
}



void Menu::ClearMenuLayouts() {
   kmenu_layout->ClearWidgets();
   menu_layout->ClearWidgets();
}



bool Menu::Setup() {
   Cleanup();
   
   btnfont = win->LoadFont("data/fonts/ChristmasOnCrack.ttf" , 36);
   btnimage = win->LoadImageFromFile("data/images/buttons/button.png");
   kbtnimage = win->LoadImageFromFile("data/images/buttons/key.png");
   
   if (!btnfont || !btnimage || !kbtnimage) {
      EAGLE_ASSERT(btnfont && btnimage && kbtnimage);
      Cleanup();
      return false;
   }
   
   gui.SetupBuffer(InnerArea().W() , InnerArea().H() , win);
   gui.SetRootLayout(&relative_layout);
   relative_layout.Resize(2);
   relative_layout.SetLayoutRectangle(&kmenu_layout , LayoutRectangle(0,0,0.15,1.0));
   relative_layout.SetLayoutRectangle(&menu_layout , LayoutRectangle(0.15,0,0.85,1.0));
   kmenu_layout.Resize(NUM_MENU_OPTIONS , MENU_VERTICAL);
   menu_layout.Resize(NUM_MENU_OPTIONS , MENU_VERTICAL);
   for (int n = 0 ; n < NUM_MENU_OPTIONS ; ++n) {
      TextIconButton* tib = new TextIconButton(btnfont , menu_strs[n]);
      btns[n] = tib;
      tib->SetImages(btnimage , btnimage , btnimage , btnimage);
      TextIconButton* ktib = new TextIconButton(btnfont , menu_keystrs[n]);
      kbtns[n] = ktib;
      ktib->SetInputGroup(input_key_press(menu_keys[i]));
      ktib->SetImages(kbtnimage , kbtnimage , kbtnimage , kbtnimage);
   }
   SetupMenuLayouts();
   return true;
}



void Menu::Cleanup() {
   gui.ClearLayout();
   relative_layout->ClearWidgets();
   ClearMenuLayouts();

   for (int n = 0 ; n < NUM_MENU_OPTIONS ; ++n) {
      if (btns[n]) {delete btns[n];}
      if (kbtns[n]) {delete kbtns[n];}
      btns[n] = kbtns[n] = 0;
   }
   focus_btn = 0;
   option_selected = NUM_MENU_OPTIONS;
   zscroll = 0;
   
   win->FreeFont(btnfont);
   win->FreeImage(btnimage);
   win->FreeImage(kbtnimage);
   btnfont = 0;
   btnimage = 0;
   kbtnimage = 0;
}
