



#include "Entity.hpp"



bool Overlaps(const Entity& e1 , const Entity& e2) {
   double dx,dy,dz;
   double tw,th,td;
   dx = fabs(e2.cx - e1.cx);
   dy = fabs(e2.cy - e1.cy);
   dz = fabs(e2.cz - e1.cz);
   tw = e1.w + e2.w;
   th = e1.h + e2.h;
   td = e1.d + e2.d;
   return ((dx <= tw) &&
           (dy <= th) &&
           (dz <= td));
}



bool Overlaps(const Entity& e1 , const Entity& e2 , double dt) {
   Entity e3 = e1;
   Entity e4 = e2;
   e3.MoveBy(dt);
   e4.MoveBy(dt);
   return Overlaps(e3,e4);
}



/// -------------------------------     Entity      ----------------------------



Entity::Entity(double xpos , double ypos , double zpos , double width , double height , double depth) :
      cx(xpos),
      cy(ypos),
      cz(zpos),
      vx(0.0),
      vy(0.0),
      vz(0.0),
      w(width),
      h(height),
      d(depth)
{}



Entity::Entity(const Entity& e) :
      cx(e.cx),
      cy(e.cy),
      cz(e.cz),
      vx(e.vx),
      vy(e.vy),
      vz(e.vz),
      w(e.w),
      h(e.h),
      d(e.d)
{}



void Entity::MoveBy(double dt) {
   cx += dt*vx;
   cy += dt*vy;
   cz += dt*vz;
}
