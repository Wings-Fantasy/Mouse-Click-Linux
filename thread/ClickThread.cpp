#include <log4cpp/Category.hh>
#include <random>
#include "ClickThread.h"

ClickThread::ClickThread() {
    mouse = new Mouse();

    // 构造函数中就启动鼠标点击线程
    clickThread = std::thread(&ClickThread::run, this);
    clickThread.detach();
}

ClickThread::~ClickThread() {
    // 在析构函数中结束鼠标点击线程
    this->flag = false;
    delete mouse;
}

void ClickThread::setDelay(const int m_delay) {
    this->delay = m_delay;
}

void ClickThread::setDelayType(const bool m_isFixedDelay) {
    this->isFixedDelay = m_isFixedDelay;
}

void ClickThread::setButton(const int m_button) {
    this->button = m_button;
}

void ClickThread::start() {
    log4cpp::Category &log = log4cpp::Category::getInstance("ClickThread");
    log.info("Start clicking.");
    running = true;
}

void ClickThread::stop() {
    running = false;
    log4cpp::Category &log = log4cpp::Category::getInstance("ClickThread");
    log.info("Stop clicking.");
}

void ClickThread::run() const {
    log4cpp::Category &log = log4cpp::Category::getInstance("ClickThread");
    log.info("Start the click thread.");

    // 第一层循环，控制线程是否运行,避免出现多个线程同时运行的情况
    while(flag) {
        // 第二层循环，控制开始点击
        while(running) {
            log.debug("Click！");
            if (isFixedDelay) {
                mouse->click(button);
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            } else {
                mouse->click(button);

                // C++11标准的随机数生成器
                std::random_device random;
                std::default_random_engine gen = std::default_random_engine(random());
                // 最小值为100，最大值为参数delay
                std::uniform_int_distribution<int> rand(100, delay);

                log.debug("Generate a random number as a delay: %d", rand(gen));
                std::this_thread::sleep_for(std::chrono::milliseconds(rand(gen)));
            }
        }
    }
}
