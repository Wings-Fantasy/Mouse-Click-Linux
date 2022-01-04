#ifndef MOUSE_CLICK_CLICKTHREAD_H
#define MOUSE_CLICK_CLICKTHREAD_H

#include <thread>
#include "../util/Mouse.h"

class ClickThread {
public:
    ClickThread();
    ~ClickThread();
    void setDelay(int m_delay);
    void setDelayType(bool m_isFixedDelay);
    void setButton(int m_button);
    void start();
    void stop();
private:
    Mouse *mouse;
    int isFixedDelay = true;
    int delay = 10000;
    int button = 1;
    bool running = false;
    bool flag = true;
    std::thread clickThread;
    void run() const;
};

#endif //MOUSE_CLICK_CLICKTHREAD_H
