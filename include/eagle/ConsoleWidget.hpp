



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
   
   /// SelectText
   virtual void DrawCaret(EagleGraphicsContext* win , int xpos , int ypos);

   /// EditText
   void DrawOverwriteCaret(EagleGraphicsContext* win , int xpos , int ypos);
   
   void Insert(std::string instr , int caretLine , int caretPos);

   void DeleteSelection(int selLineStart , int selLineStop , int selLeft , int selRight);
   void Overwrite(std::string instr , int selLineStart , int selLineStop , int selLeft , int selRight);
   
public :
   
   EditText(std::string classname = "EditText" , std::string objname = "Nemo");
   
   bool Insert();
   bool Overwrite();
   
   void SetTabSpacing(unsigned int ntabspaces);
};


class ConsoleLog {
   std::deque<std::string> loglines;
};

#endif // ConsoleWidget_HPP
