



#ifndef Entity_HPP
#define Entity_HPP



class Entity {
   
protected :
   double cx,cy,cz;
   double vx,vy,vz;
   double w,h,d;
   
   virtual void VHandleEvent(EagleEvent e)=0;
   virtual void VUpdate(double dt)=0;
   virtual void VDisplay(EagleGraphicsContext* win);
public :
   
   Entity(double xpos , double ypos , double zpos , double width , double height , double depth);
   Entity(const Entity& e);



   void HandleEvent(EagleEvent e);
   void Update(double dt);
   void Display(EagleGraphicsContext* win);
   
   void MoveBy(double dt);
   
   friend bool Overlaps(const Entity& e1 , const Entity& e2);
   friend bool Overlaps(const Entity& e1 , const Entity& e2 , double dt);
};

bool Overlaps(const Entity& e1 , const Entity& e2);
bool Overlaps(const Entity& e1 , const Entity& e2 , double dt);

#endif // Entity_HPP
