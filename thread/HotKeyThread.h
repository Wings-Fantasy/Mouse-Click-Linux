#ifndef MOUSE_CLICK_HOTKEYTHREAD_H
#define MOUSE_CLICK_HOTKEYTHREAD_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <thread>

extern Display *display;
extern bool isClicking;
extern void start_onClick();
extern void stop_onClick();

class HotKeyThread {
public:
    HotKeyThread();
    ~HotKeyThread();
    void changeHotKey(long m_X_KEY);
private:
    long X_KEY;
    int modifiers = Mod2Mask; // Num Lock打开小键盘
    int keycode;
    Window root;
    bool flag = true;
    std::thread hotKeyThread;
    void run() const;
};

#endif //MOUSE_CLICK_HOTKEYTHREAD_H
