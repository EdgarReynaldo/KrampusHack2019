



#ifndef ConsoleWidget_HPP
#define ConsoleWidget_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Text/EditText.hpp"
#include "Eagle/Font.hpp"

#include <deque>



class ConsoleLog : public WidgetBase {
   std::deque<std::string> loglines;
   int NHISTORY;
   EagleFont* font;
   
   /// Private interface, override to define behavior
   virtual int PrivateHandleEvent(EagleEvent ee);
//   virtual int PrivateCheckInputs();
//   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   /// Callbacks, override and/or call if you need to
//   virtual void OnAreaChanged();
   
public :
   ConsoleLog(std::string classname = "Console Log" , std::string objname = "Nemo");
   
   virtual void SetRedrawFlag() {SetBgRedrawFlag();}
   
   void AddEntry(std::string s);
   void Clear();
   void SetFont(EagleFont* fnt) {font = fnt;}
};

extern const unsigned int TOPIC_CONSOLE;

enum CONSOLE_MSGS {
   CONSOLE_ENTER_PRESSED = 0
};



class ConsoleEntry : public EditText {
   
   
   int PrivateHandleEvent(EagleEvent e);
};



#endif // ConsoleWidget_HPP





