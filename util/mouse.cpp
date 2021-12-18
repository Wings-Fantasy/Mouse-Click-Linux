#include <log4cpp/Category.hh>
#include <X11/extensions/XTest.h>
#include "mouse.h"

Mouse::Mouse() {
    log4cpp::Category &log = log4cpp::Category::getInstance("MouseClick");
    display = XOpenDisplay (nullptr);
    if (display == nullptr) {
        log.error("Can't open display!");
    } else {
        log.info("open display successfully.");
    }
}

Mouse::~Mouse() {
    XCloseDisplay (display);
    display = nullptr;
    log4cpp::Category &log = log4cpp::Category::getInstance("MouseClick");
    log.info("close display.");
}

/*
// 点击方案无效
void Mouse::click(const int button) {
    log4cpp::Category &log = log4cpp::Category::getInstance("MouseClick");
    log.debug("button:"+std::to_string(button));
    XEvent event;
    memset (&event, 0, sizeof (event));
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    event.xbutton.subwindow = DefaultRootWindow (display);
    while (event.xbutton.subwindow) {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer (display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow,
                       &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                       &event.xbutton.state);
    }

    event.type = ButtonPress;
    if (XSendEvent (display, PointerWindow, True, ButtonPressMask, &event) == 0) {
        log.error("send event failed.");
    }
    event.type = ButtonRelease;
    if (XSendEvent (display, PointerWindow, True, ButtonReleaseMask, &event) == 0) {
        log.error("send event failed.");
    }
    XFlush (display);
}
*/

void Mouse::click(int button) {
    // 按下按键
    XTestFakeButtonEvent(display, button, True, 0);
    // 松开按键
    XTestFakeButtonEvent(display, button, False, 0);
    // 更新缓冲区（向Xorg发送操作）
    XFlush(display);
}