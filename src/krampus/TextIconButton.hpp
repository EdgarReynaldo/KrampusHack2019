



#ifndef TextIconButton_HPP
#define TextIconButton_HPP

class TextIconButton : public IconButton {
   
   std::string label;
   EagleFont* label_font;

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   
public :
   TextIconButton(EagleFont* font , std::string text , std::string classname = "TextIconButton" , std::string objname = "Nemo");

   void DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos));
   virtual void DisplayText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor tint = EagleColor(255,255,255));
};
};


#endif // TextIconButton_HPP
