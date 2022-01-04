#include <log4cpp/Category.hh>
#include "HotKeyThread.h"

HotKeyThread::HotKeyThread() {
    X_KEY = XK_F7;
    root = DefaultRootWindow(display);
    keycode = XKeysymToKeycode(display,X_KEY);
    XGrabKey(display, keycode, modifiers, root, False, GrabModeAsync, GrabModeAsync);

    // 在构造函数中启动快捷键线程
    hotKeyThread = std::thread(&HotKeyThread::run, this);
    hotKeyThread.detach();
}

HotKeyThread::~HotKeyThread() {
    this->X_KEY = 0;
    this->root = 0;
    this->flag = false;
    XUngrabKey(display,keycode,modifiers,root);
}

void HotKeyThread::changeHotKey(long m_X_KEY) {
    switch (m_X_KEY) {
        case 1:
            X_KEY = XK_F1;
            break;
        case 2:
            X_KEY = XK_F2;
            break;
        case 3:
            X_KEY = XK_F3;
            break;
        case 4:
            X_KEY = XK_F4;
            break;
        case 5:
            X_KEY = XK_F5;
            break;
        case 6:
            X_KEY = XK_F6;
            break;
        case 7:
            X_KEY = XK_F7;
            break;
        case 8:
            X_KEY = XK_F8;
            break;
        case 9:
            X_KEY = XK_F9;
            break;
        case 10:
            X_KEY = XK_F10;
            break;
        case 11:
            X_KEY = XK_F11;
            break;
        case 12:
            X_KEY = XK_F12;
            break;
        default:
            X_KEY = XK_F7;
            break;
    }
    XUngrabKey(display,keycode,modifiers,root);
    keycode = XKeysymToKeycode(display,X_KEY);
    XGrabKey(display, keycode, modifiers, root, False, GrabModeAsync, GrabModeAsync);
}

void HotKeyThread::run() const {
    log4cpp::Category &log = log4cpp::Category::getInstance("HotKeyThread");
    while(flag) {
        XEvent event;
        XNextEvent(display, &event);
        XSelectInput(display, root, KeyPressMask );
        if(event.type == KeyPress) {
            if(event.xkey.keycode == XKeysymToKeycode(display,X_KEY)) {
                log.debug("HotKey pressed");
                if (isClicking) {
                    stop_onClick();
                } else {
                    start_onClick();
                }
            }
        }
    }
}
