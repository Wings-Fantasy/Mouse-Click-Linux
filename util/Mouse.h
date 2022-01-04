#ifndef MOUSE_CLICK_MOUSE_H
#define MOUSE_CLICK_MOUSE_H

#include <X11/Xlib.h>

extern Display *display;

class Mouse {
public:
    Mouse();
    ~Mouse();
    void click(int button);
private:
//    Display *display;
};

#endif //MOUSE_CLICK_MOUSE_H
