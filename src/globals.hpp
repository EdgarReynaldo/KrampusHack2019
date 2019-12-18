



#ifndef GLOBALS_HPP
#define GLOBALS_HPP

extern bool quit;
extern bool redraw;

extern int scrw;
extern int scrh;

class Allegro5System;
extern Allegro5System* a5sys;

class EagleGraphicsContext;
extern EagleGraphicsContext* win;

class EagleTimer;
extern EagleTimer* systimer;

class EagleImage;

extern EagleImage* stage;
extern EagleImage* left_curtain;
extern EagleImage* right_curtain;
extern EagleImage* drop_curtain;

extern EagleImage* krampus_splash;
extern EagleImage* blitzen;

class EagleFont;
extern EagleFont* krampus_font_huge;
extern EagleFont* krampus_font36;
extern EagleFont* krampus_font24;

#endif // GLOBALS_HPP
