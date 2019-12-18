



#include "globals.hpp"
#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


bool quit = false;
bool redraw = true;
int scrw = 1024;
int scrh = 768;
Allegro5System* a5sys = 0;
EagleGraphicsContext* win = 0;
EagleTimer* systimer = 0;

EagleImage* stage = 0;
EagleImage* left_curtain = 0;
EagleImage* right_curtain = 0;
EagleImage* drop_curtain = 0;

EagleImage* krampus_splash = 0;
EagleImage* blitzen = 0;

EagleFont* krampus_font_huge = 0;
EagleFont* krampus_font36 = 0;
EagleFont* krampus_font24 = 0;
