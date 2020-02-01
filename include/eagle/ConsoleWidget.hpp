



#ifndef ConsoleWidget_HPP
#define ConsoleWidget_HPP


#include "Eagle/Gui/Text/SelectText.hpp"



class EditText : public SelectText {
   
protected :
   
   bool insert_mode;
   unsigned int pixscroll;
   unsigned int NTABSPACES;
   
   
   
   /// WIDGETBASE
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnAreaChanged();
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
   
   /// EditText
   
   void DeleteSelection(int selectLineStart , int selectLineStop , int selectLeft , int selectRight);
   void Insert(std::string instr , int caretLine , int caretPos);
   void Overwrite(std::string instr , int caretLine , int caretPos);

public :
   
   EditText(std::string classname = "EditText" , std::string objname = "Nemo");
   
   bool Insert() {return insert_mode;}
   bool Overwrite() {return !insert_mode;}
};


class ConsoleLog {
   std::deque<std::string> loglines;
};

#endif // ConsoleWidget_HPP
