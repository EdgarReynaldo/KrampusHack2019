



#include "eagle/ConsoleWidget.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/InputHandler.hpp"



const unsigned int TOPIC_CONSOLE = NextFreeTopicId();



int ConsoleLog::PrivateHandleEvent(EagleEvent ee) {
   return DIALOG_OKAY;
}



void ConsoleLog::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   int x = InnerArea().X();
   int y = InnerArea().BRY();
   const int lspace = 4;
   for (unsigned int i = 0 ; i < loglines.size() ; ++i) {
      win->DrawTextString(font , loglines[i] , xpos + x , ypos + y , GetColor(TXTCOL) , HALIGN_LEFT , VALIGN_BOTTOM);
      y -= (lspace + font->Height());
   }
}



ConsoleLog::ConsoleLog(std::string classname , std::string objname) :
      WidgetBase(classname , objname),
      loglines(),
      NHISTORY(-1),
      font(0)
{}



void ConsoleLog::AddEntry(std::string s) {
   if (NHISTORY < 0 || (NHISTORY > 0 && loglines.size() < NHISTORY)) {
      loglines.push_front(s);
   }
   if (NHISTORY > 0 && loglines.size() > NHISTORY) {
      loglines.pop_back();
   }
   SetBgRedrawFlag();
}



void ConsoleLog::Clear() {
   loglines.clear();
   SetBgRedrawFlag();
}




/// --------------------------     Console entry      ------------------------------




int ConsoleEntry::PrivateHandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_KEY_CHAR && e.keyboard.keycode == EAGLE_KEY_ENTER) {
      QueueUserMessage(WidgetMsg(this , TOPIC_CONSOLE , CONSOLE_ENTER_PRESSED));
      return DIALOG_INPUT_USED;
   }
   return EditText::PrivateHandleEvent(e);
}
