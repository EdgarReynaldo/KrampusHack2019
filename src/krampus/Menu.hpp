



#ifndef Menu_HPP
#define Menu_HPP

#include "Scenes.hpp"
#include "Eagle.hpp"



enum MENU_OPTION {
   MENU_HOSTGAME = 0,
   MENU_JOINGAME = 1,
   MENU_INTRO    = 2,
   MENU_CREDITS  = 3,
   MENU_QUIT     = 4,
   NUM_MENU_OPTIONS = 5
};

extern const char* menu_strs[NUM_MENU_OPTIONS];

extern const char* menu_keystrs[NUM_MENU_OPTIONS];

extern const int menu_keys[NUM_MENU_OPTIONS];



class Menu : public WidgetBase {

   
protected :
   WidgetHandler gui;
   
   RelativeLayout relative_layout;
   ClassicMenuLayout menu_layout;
   ClassicMenuLayout kmenu_layout;
   
   TextIconButton* btns[NUM_MENU_OPTIONS];
   TextIconButton* kbtns[NUM_MENU_OPTIONS];

   TextIconButton* focus_btn;
   
   int option_selected;
   int zscroll;
   
   EagleFont* btnfont;
   EagleImage* btnimage;
   EagleImage* kbtnimage;
   
   MENU_OPTION choice;
   STATUS status;
   
   /// Private interface, override to define behavior
   virtual int PrivateHandleEvent(EagleEvent ee);///< Virtual function that controls how widgets react to events
   virtual int PrivateCheckInputs();///< Virtual function that controls how widgets react to polled input
   virtual int PrivateUpdate(double dt);///< Virtual function that controls what happens when time passes for a widget
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);///< Virtual function that controls how a widget is drawn

   /// Callbacks, overload if you need to
   virtual void OnAreaChanged();///< Override to react to changes in this widget's area
   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);///< Override to react to attribute changes
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);///< Override to react to flag changes
   virtual void OnColorChanged();///< Override to react to color changes

   
   
   void SetupMenuLayouts();
   void ClearMenuLayouts();
   
public :
   Menu(std::string classname = "KrampusMenu" , std::string objname = "MainMenu");
         
   bool Setup();
   void Cleanup();
   
   STATUS GetStatus();
   MENU_OPTION GetChoice();
};



class MenuScene : public Scene {
   
protected :
   Menu menu;
   
   Intro3 bg_scene;

public :
   MenuScene(double length , int xpos , int ypos , int width , int height);
   virtual ~MenuScene() {}
   
   virtual STATUS HandleEvent(EagleEvent e);
   virtual void Display();
   virtual bool Setup();
   virtual void Cleanup();
   

   MENU_OPTION GetChoice();
};


#endif // Menu_HPP
