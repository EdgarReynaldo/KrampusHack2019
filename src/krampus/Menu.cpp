



#include "Menu.hpp"
#include "../globals.hpp"


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



const int menu_keys[NUM_MENU_OPTIONS] = {
   EAGLE_KEY_H,
   EAGLE_KEY_J,
   EAGLE_KEY_I,
   EAGLE_KEY_C,
   EAGLE_KEY_Q
};



/// Private interface, override to define behavior
int Menu::PrivateHandleEvent(EagleEvent ee) {
   
   if (ee.type == EAGLE_EVENT_KEY_DOWN) {
      /// Menu cycles by pressing up or down
      if (ee.keyboard.keycode == EAGLE_KEY_UP || ee.keyboard.keycode == EAGLE_KEY_DOWN) {
         zscroll += (ee.keyboard.keycode == EAGLE_KEY_DOWN)?-1:1;
         zscroll = zscroll % NUM_MENU_OPTIONS;
///         focus_btn = ;
         SetRedrawFlag();
         return DIALOG_OKAY;
      }
      /// Menu activates by pressing enter
      if (ee.keyboard.keycode == EAGLE_KEY_ENTER) {
         if (focus_btn) {
            status = STATUS_COMPLETE;
///            choice = ;
         }
      }
      if (ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         status = STATUS_COMPLETE;
      }
   }
   int ret = gui.HandleEvent(ee);
   
   while (gui.HasMessages()) {
      WidgetMsg msg = gui.TakeNextMessage();
      if (msg.topic == TOPIC_BUTTON_WIDGET) {
         if (msg.msgs == BUTTON_CLICKED) {
///            choice = 
         }
      }
   }
   
   /// 
   if (gui.Flags().FlagOn(NEEDS_REDRAW)) {
      redraw = true;
   }
   
   return ret;
}



int Menu::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



int Menu::PrivateUpdate(double dt) {
   return gui.Update(dt);
}



void Menu::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   gui.Display(win,xpos,ypos);
}



/// Callbacks, overload if you need to
void Menu::OnAreaChanged() {
   gui.SetWidgetArea(InnerArea() , false);
}



void Menu::OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v) {
   gui.SetAttribute(a,v);
}



void Menu::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   (void)f;
   (void)on;
   gui.SetWidgetFlags(Flags());
}



void Menu::OnColorChanged() {
   gui.SetWidgetColorset(this->WidgetColors());
}



Menu::Menu(std::string classname , std::string objname) :
      WidgetBase(classname , objname),
      gui(win),
      relative_layout(),
      menu_layout(),
      kmenu_layout(),
      btns(),
      kbtns(),
      focus_btn(0),
      option_selected(NUM_MENU_OPTIONS),
      zscroll(0),
      btnfont(0),
      btnimage(0),
      kbtnimage(0),
      choice(NUM_MENU_OPTIONS),
      status(STATUS_OKAY)
{}



void Menu::SetupMenuLayouts() {
   ClearMenuLayouts();
   kmenu_layout.ResizeMenu(NUM_MENU_OPTIONS , MENU_VERTICAL);
   menu_layout.ResizeMenu(NUM_MENU_OPTIONS , MENU_VERTICAL);
   kmenu_layout.SetGlobalPadding(10,10);
   menu_layout.SetGlobalPadding(5,10);
   for (int n = 0 ; n < NUM_MENU_OPTIONS ; ++n) {
      menu_layout.PlaceWidget(btns[n] , (n + zscroll)%NUM_MENU_OPTIONS);
      kmenu_layout.PlaceWidget(kbtns[n] , (n + zscroll)%NUM_MENU_OPTIONS);
   }
}



void Menu::ClearMenuLayouts() {
   kmenu_layout.ClearWidgets();
   menu_layout.ClearWidgets();
}



bool Menu::Setup() {
   Cleanup();
   status = STATUS_QUIT;
   btnfont = win->LoadFont("data/fonts/ChristmasOnCrack.ttf" , 72);
   btnimage = win->LoadImageFromFile("data/images/buttons/button.png");
   kbtnimage = win->LoadImageFromFile("data/images/buttons/key.png");
   
   if (!btnfont || !btnimage || !kbtnimage) {
      EAGLE_ASSERT(btnfont && btnimage && kbtnimage);
      Cleanup();
      return false;
   }
   if (!gui.SetupBuffer(InnerArea().W() , InnerArea().H() , win)) {
      Cleanup();
      return false;
   }
   gui.SetBackgroundColor(EagleColor(0,0,0,0));
   gui.SetRootLayout(&relative_layout);
   relative_layout.Resize(2);
   relative_layout.PlaceWidget(&kmenu_layout , 0 , LayoutRectangle(0,0,0.15,1.0));
   relative_layout.PlaceWidget(&menu_layout , 1 , LayoutRectangle(0.15,0,0.85,1.0));
   for (int n = 0 ; n < NUM_MENU_OPTIONS ; ++n) {
      TextIconButton* tib = new TextIconButton(btnfont , menu_strs[n]);
      btns[n] = tib;
      tib->SetImages(btnimage , btnimage , btnimage , btnimage);
      TextIconButton* ktib = new TextIconButton(btnfont , menu_keystrs[n]);
      kbtns[n] = ktib;
      ktib->SetInputGroup(input_key_press(menu_keys[n]));
      ktib->SetImages(kbtnimage , kbtnimage , kbtnimage , kbtnimage);
   }
   SetupMenuLayouts();
   status = STATUS_OKAY;
   EAGLE_DEBUG(
      EagleInfo() << gui << std::endl;
   );
   return true;
}



void Menu::Cleanup() {
   gui.ClearLayout();
   relative_layout.ClearWidgets();
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



STATUS Menu::GetStatus() {
   return status;
}



MENU_OPTION Menu::GetChoice() {
   return choice;
}



/// -------------------------    MenuScene     ----------------------------



MenuScene::MenuScene(double length , int xpos , int ypos , int width , int height) :
      Scene(length , xpos , ypos , width , height),
      menu(),
      bg_scene(-1.0 , xpos , ypos , width , height)
{}



STATUS MenuScene::HandleEvent(EagleEvent e) {
   int ret = menu.HandleEvent(e);
   ret |= bg_scene.HandleEvent(e);
   if (ret == DIALOG_CLOSE) {
      return STATUS_QUIT;
   }
   
   return menu.GetStatus();
}



void MenuScene::Display() {
   bg_scene.Display();
   menu.Display(win , 0 , 0);
}



bool MenuScene::Setup() {
   Rectangle r(dest.X() + dest.W()/4 , dest.Y() + dest.H()/4 , dest.W()/2 , dest.H()/2);
   menu.SetWidgetArea(r , false);
   bool ret = menu.Setup() && bg_scene.Setup();
   bg_scene.SetPercent(1.0);
   return ret;
}



void MenuScene::Cleanup() {
   menu.Cleanup();
   bg_scene.Cleanup();
}



MENU_OPTION MenuScene::GetChoice() {
   return menu.GetChoice();
}
