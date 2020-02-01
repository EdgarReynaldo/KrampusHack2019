



#include "eagle/ConsoleWidget.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/InputHandler.hpp"




void EditText::DeleteSelection(int selLineStart , int selLineStop , int selLeft , int selRight) {
   std::string::iterator itLeft = text.begin();
   std::string::iterator itRight = text.begin();
   if (GetSelectionIterators(text , &itLeft , &itRight , selLineStart , selLineStop , selLeft , selRight)) {
      text.erase(itLeft , itRight);
      select_line_start = select_line_close = -1;
      select_left = select_right = 0;
      caret_line = selLineStart;
      caret_pos = selLeft;
   }
}



void EditText::Insert(std::string instr , int caretLine , int caretPos) {
   std::string::iterator it = text.begin();
   if (GetPositionIterator(text , &it , caretLine , caretPos)) {
      text.insert(it , instr.begin() , instr.end());
   }
   /// Don't change the caret position
}



void EditText::Overwrite(std::string instr , int caretLine , int caretPos) {
   if (select_line_start != select_line_close || select_left != select_right) {
      DeleteSelection(select_line_start , select_line_close , select_left , select_right);
   }
   else {
      DeleteSelection(caret_line , caret_line , caret_pos , caret_pos + 1);
   }
   Insert(instr , caret_line , caret_pos);
   MoveCaretLeftOrRight(EAGLE_KEY_RIGHT , false);
}



EditText::EditText(std::string classname , std::string objname) :
      SelectText(classname , objname),
      insert_mode(true)
{}
   


int EditText::PrivateHandleEvent(EagleEvent e) {
   
   int ret = SelectText::PrivateHandleEvent(e);
   if (ret & DIALOG_INPUT_USED) {
      return ret;
   }
   
   if (e.type == EAGLE_EVENT_KEY_CHAR) {
      /// Make sure it is an ascii character
      std::string newstr = "";
      newstr.push_back((char)e.keyboard.keycode);
      if (e.keyboard.unicode <= 27) {
         /// Control key, or invisible
         if (e.keyboard.keycode == EAGLE_KEY_TAB) {
            newstr = "";
            for (unsigned int i = 0 ; i < NTABSPACES ; ++i) {
               newstr += " ";
            }
         }
      }
      if (Insert()) {
         /// Insert mode
         Insert(newstr , caret_line , caret_pos);
      }
      else {
         /// Overwrite
         Overwrite(newstr , caret_line , caret_pos);
      }
   }
}



int EditText::PrivateCheckInputs() {
   return SelectText::PrivateCheckInputs();
}



void EditText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   SelectText::PrivateDisplay(win , xpos , ypos - (int)pixscroll);
}



int EditText::PrivateUpdate(double tsec) {
   return SelectText::PrivateUpdate(tsec);
}



void EditText::OnAreaChanged() {
   
}



void EditText::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   SelectText::OnFlagChanged(f , on);
}





