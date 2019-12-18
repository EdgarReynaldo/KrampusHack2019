



#include "TextIconButton.hpp"



void TextIconButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DisplayIcon(win , btn_state , xpos , ypos);
   DisplayText(win , xpos , ypos , GetColor(TXTCOL));
}



TextIconButton::TextIconButton(EagleFont* font , std::string text , std::string classname = "TextIconButton" , std::string objname = "Nemo") :
      IconButton(classname , objname),
      label(text),
      font(label_font)
{}



void TextIconButton::DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos) {
   switch (state) {
   case BUTTON_UP :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(FGCOL));
      break;
   case BUTTON_DOWN :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(BGCOL));
      break;
   case BUTTON_HOVER_UP :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(HLCOL));
      break;
   case BUTTON_HOVER_DOWN :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(MGCOL));
      break;
   default :
      break;
   };
}



void TextIconButton::DisplayText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor tint) {
   Transformer t = win->GetTransformer()->GetViewTransform();
   
   t.Scale(0.8*label_font->Width(label)/InnerArea().W(),
           0.8*label_font->Height()/InnerArea().H());
   t.Translate(xpos + InnerArea().CX() , ypos + InnerArea().CY());
   
   win->GetTransformer()->PushViewTransform(t);
   
   win->DrawTextString(label_font , label , 0 , 0 , tint , HALIGN_CENTER , VALIGN_CENTER);
   
   win->GetTransformer()->PopViewTransform();
}



