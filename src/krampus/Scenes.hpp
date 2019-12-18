



#ifndef Scenes_HPP
#define Scenes_HPP

#include "../eagle/Transitions.hpp"

enum STATUS {
   STATUS_QUIT = -1,
   STATUS_OKAY = 0,
   STATUS_COMPLETE = 1
};

class Scene {
   
public :
   double duration;
   double elapsed;
   double pct;
   double inc;
   Rectangle dest;
   
   Scene(double length , int xpos , int ypos , int width , int height);
   virtual ~Scene() {}
   
   virtual STATUS HandleEvent(EagleEvent e)=0;
   virtual void Display()=0;
   virtual bool Setup()=0;
   virtual void Cleanup()=0;
   
   STATUS Run();
};


class Intro : public Scene {
   
   Transition tr_clip;
   Translate2 tr_drop;
   Translate2 tr_left;
   Translate2 tr_right;
   double pause_timer;
   
public :

   Intro(double duration , int xpos , int ypos , int width , int height);

   virtual bool Setup();
   virtual void Cleanup();

   virtual STATUS HandleEvent(EagleEvent e);
   virtual void Display();
   
};


class Intro2 : public Scene {

public :

   Intro2(double duration , int xpos , int ypos , int width , int height);
   
   
   virtual bool Setup();
   virtual void Cleanup();

   virtual STATUS HandleEvent(EagleEvent e);
   virtual void Display();
   
};


class Intro3 : public Scene {

   Transition tr_clip;

public :

   Intro3(double duration , int xpos , int ypos , int width , int height);
   
   
   virtual bool Setup();
   virtual void Cleanup();

   virtual STATUS HandleEvent(EagleEvent e);
   virtual void Display();
   
};

#endif // Scenes_HPP
