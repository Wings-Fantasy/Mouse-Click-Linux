#ifndef MOUSE_CLICK_MAIN_H
#define MOUSE_CLICK_MAIN_H

#include <X11/Xlib.h>

void set_control_status(bool status);
void start_onClick();
void stop_onClick();
Display *display;
bool isClicking = false;

#endif //MOUSE_CLICK_MAIN_H
